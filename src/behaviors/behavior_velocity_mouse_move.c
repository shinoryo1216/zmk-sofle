/*
 * behavior_velocity_mouse_move.c
 *
 * ロータリーエンコーダー用の「速度連動」マウスポインター移動ビヘイビア。
 *
 * 【背景・目的】
 * ロータリーエンコーダーでマウスポインターを操作する際、従来の
 * "&mmv を tap-ms で叩く" 方式では、1detentあたりの移動量が固定になり、
 *   - 移動量を確保しようとすると微調整ができなくなる
 *   - 微調整できるようにすると、大きく動かすのに何度も回す必要がある
 * という、単一の速度パラメータでは両立できないトレードオフが生じていた。
 *
 * 本ビヘイビアは、直前のトリガーからの経過時間(＝どれくらいの速さで
 * 回されているか)を実測し、それに応じて1トリガーあたりの移動量を
 * 動的に変える。ゆっくり回した単発のdetentは小さく動き(微調整向き)、
 * 素早く連続して回した場合は大きく動く(大きな移動向き)。
 *
 * &mmv (zmk,behavior-input-two-axis) のような「押しっぱなし時間で
 * 加速する」モデルとは異なり、本ビヘイビアはトリガーされた瞬間に
 * 一度だけ、計算済みの移動量をそのまま即座にHIDへ報告する
 * (hold/release の概念を持たない)。これにより、離散トリガー方式
 * (zmk,behavior-sensor-rotate等)と組み合わせても、キューの消化待ちで
 * 惰性が生じるという問題が起こらない。
 *
 * 【接続方法】
 * 本ビヘイビア自身がZephyr Input Subsystemへの入力元(input device)
 * として振る舞うため、対応する zmk,input-listener ノードで
 * device = <&このインスタンス>; と明示的に接続する必要がある
 * (mmv/mscのように暗黙のリスナーは用意されていないため)。
 *
 * 【注意】
 * このファイルは、ZMK公式ドキュメント(zmk.dev/docs/development/new-behavior)
 * および behavior_input_two_axis.c 等の実装パターンから類推して書かれた
 * 手書きのドライバであり、実機ビルドでのコンパイル確認は行えていない。
 * west update 後にローカルの
 *   <workspace>/zmk/app/src/behaviors/behavior_input_two_axis.c
 *   <workspace>/zmk/app/src/behaviors/behavior_sensor_rotate_common.c
 * を正解として突き合わせ、ビルドエラーが出た場合はAPI名
 * (device_get_binding / zmk_behavior_get_binding 等の差異、
 * BEHAVIOR_DT_INST_DEFINE のマクロ名や引数順など)を実際のバージョンに
 * 合わせて調整してほしい。
 */

#define DT_DRV_COMPAT zmk_behavior_velocity_mouse_move

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/input/input.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>

LOG_MODULE_REGISTER(behavior_velocity_mouse_move, CONFIG_ZMK_LOG_LEVEL);

struct behavior_velocity_mouse_move_config {
    uint16_t input_code;
    int32_t min_move;
    int32_t max_move;
    int32_t min_interval_ms;
    int32_t max_interval_ms;
};

struct behavior_velocity_mouse_move_data {
    int64_t last_trigger_time_ms; /* 0 = まだ一度もトリガーされていない */
};

/* トリガー間隔(ms)から、今回の移動量(絶対値・符号なし)を計算する。
 * 間隔が短い(速く回している)ほど大きい値を返す。 */
static int32_t compute_move_magnitude(const struct behavior_velocity_mouse_move_config *config,
                                       int64_t interval_ms) {
    if (interval_ms <= config->min_interval_ms) {
        return config->max_move;
    }
    if (interval_ms >= config->max_interval_ms) {
        return config->min_move;
    }

    int64_t span_ms = config->max_interval_ms - config->min_interval_ms;
    int64_t fastness = config->max_interval_ms - interval_ms; /* 0(遅い)〜span_ms(速い) */
    int32_t move_range = config->max_move - config->min_move;

    if (span_ms <= 0) {
        return config->min_move;
    }

    return config->min_move + (int32_t)((fastness * move_range) / span_ms);
}

static int behavior_velocity_mouse_move_pressed(struct zmk_behavior_binding *binding,
                                                  struct zmk_behavior_binding_event event) {
    const struct device *dev = device_get_binding(binding->behavior_dev);
    if (dev == NULL) {
        LOG_ERR("velocity-mouse-move: unknown device %s", binding->behavior_dev);
        return -ENODEV;
    }

    const struct behavior_velocity_mouse_move_config *config = dev->config;
    struct behavior_velocity_mouse_move_data *data = dev->data;

    int64_t now_ms = k_uptime_get();
    int64_t interval_ms = (data->last_trigger_time_ms == 0)
                               ? config->max_interval_ms
                               : (now_ms - data->last_trigger_time_ms);
    data->last_trigger_time_ms = now_ms;

    int32_t magnitude = compute_move_magnitude(config, interval_ms);
    /* 【重要】ZMKの struct zmk_behavior_binding の param1/param2 は
     * uint32_t（符号なし）で定義されている。devicetree側で <&vel_move_x (-1)>
     * のように負の値を渡しても、符号なし整数としては非常に大きな正の値
     * （0xFFFFFFFF = 4294967295）として格納されるため、
     * 「param1 >= 0」という判定は常にtrueになってしまい、
     * 常に正方向（右/下）に動くバグになっていた。
     * ビット列としては2の補数表現の-1がそのまま入っているので、
     * int32_tへキャストしてから符号を判定すれば正しく復元できる。 */
    int32_t signed_param1 = (int32_t)binding->param1;
    int32_t direction = (signed_param1 >= 0) ? 1 : -1;
    int32_t value = magnitude * direction;

    LOG_DBG("velocity-mouse-move: interval=%lldms magnitude=%d value=%d", interval_ms, magnitude,
            value);

    input_report_rel(dev, config->input_code, value, true, K_FOREVER);

    return ZMK_BEHAVIOR_OPAQUE;
}

static int behavior_velocity_mouse_move_released(struct zmk_behavior_binding *binding,
                                                   struct zmk_behavior_binding_event event) {
    /* 瞬間移動のみを行うビヘイビアのため、releaseでは何もしない。 */
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_velocity_mouse_move_driver_api = {
    .binding_pressed = behavior_velocity_mouse_move_pressed,
    .binding_released = behavior_velocity_mouse_move_released,
};

#define VMM_INST(n)                                                                              \
    static struct behavior_velocity_mouse_move_data behavior_velocity_mouse_move_data_##n = {    \
        .last_trigger_time_ms = 0,                                                               \
    };                                                                                            \
    static const struct behavior_velocity_mouse_move_config                                      \
        behavior_velocity_mouse_move_config_##n = {                                              \
            .input_code = DT_INST_PROP(n, input_code),                                           \
            .min_move = DT_INST_PROP(n, min_move),                                                \
            .max_move = DT_INST_PROP(n, max_move),                                                \
            .min_interval_ms = DT_INST_PROP(n, min_interval_ms),                                  \
            .max_interval_ms = DT_INST_PROP(n, max_interval_ms),                                  \
    };                                                                                            \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, &behavior_velocity_mouse_move_data_##n,                \
                             &behavior_velocity_mouse_move_config_##n, POST_KERNEL,                \
                             CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                  \
                             &behavior_velocity_mouse_move_driver_api);

DT_INST_FOREACH_STATUS_OKAY(VMM_INST)
