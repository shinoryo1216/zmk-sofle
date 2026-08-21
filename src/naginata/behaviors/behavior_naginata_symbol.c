/*
 * システム③（Colemak+薙刀式）追加分。
 *
 * 薙刀式エンジン本体（behavior_naginata.c）の pressed_keys は uint32_t（32bit）で、
 * 既に31キー分（A-Z, SEMI/COMMA/DOT/SLASH, SPACE=ENTER共有）が埋まっており、
 * L1・R6（行3・行4、両端の外側キー）を新しい"文字キー"としてその内部に
 * 追加する余地がない。
 *
 * そのためL1・R6の「単独タップ＝記号A」「センターシフト同時押し＝記号B」は、
 * 薙刀式エンジンの外側にある、この完全に独立した軽量ビヘイビアで実現する。
 * 中身は既存のngh_JK*系関数（（）「」『』は既存の編集モード関数をそのまま
 * 再利用、＜＞のみ新規追加）を、押した瞬間に1回だけ呼ぶだけの単純な実装。
 *
 * IME force-set（NG_SYM_IME_JAPANESE / NG_SYM_IME_ENGLISH）も同じ枠組みに
 * 相乗りさせている。これはシステム③のColemak⇔薙刀式切替や、
 * システム①②③間の切替コンボから呼ぶ。
 */

#define DT_DRV_COMPAT zmk_behavior_naginata_symbol

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk_naginata/naginata_func.h>
#include <dt-bindings/zmk_naginata/symbols.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    naginata_set_timestamp(event.timestamp);

    switch (binding->param1) {
    case NG_SYM_PAREN_OPEN:
        ngh_JKG();
        break;
    case NG_SYM_PAREN_CLOSE:
        ngh_JKB();
        break;
    case NG_SYM_KAGI_OPEN:
        ngh_JKF();
        break;
    case NG_SYM_KAGI_CLOSE:
        ngh_JKV();
        break;
    case NG_SYM_NIKAGI_OPEN:
        ngh_JKS();
        break;
    case NG_SYM_NIKAGI_CLOSE:
        ngh_JKX();
        break;
    case NG_SYM_LT:
        ng_lt();
        break;
    case NG_SYM_GT:
        ng_gt();
        break;
    case NG_SYM_IME_JAPANESE:
        ng_os_ime_japanese();
        break;
    case NG_SYM_IME_ENGLISH:
        ng_os_ime_english();
        break;
    case NG_SYM_COLON_FULL:
        ng_colon_full();
        break;
    case NG_SYM_SLASH_FULL:
        ng_slash_full();
        break;
    case NG_SYM_QUESTION:
        ng_question_noenter();
        break;
    case NG_SYM_EXCLAIM:
        ng_exclaim_noenter();
        break;
    default:
        LOG_WRN("naginata_symbol: unknown param1 %d", binding->param1);
        break;
    }

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                       struct zmk_behavior_binding_event event) {
    // すべて単発出力のため、離した時の処理は不要。
    return ZMK_BEHAVIOR_OPAQUE;
}

static int behavior_naginata_symbol_init(const struct device *dev) { return 0; }

static const struct behavior_driver_api behavior_naginata_symbol_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released};

#define NG_SYM_INST(n)                                                                           \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_naginata_symbol_init, NULL, NULL, NULL, POST_KERNEL,      \
                             CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                 \
                             &behavior_naginata_symbol_driver_api);

DT_INST_FOREACH_STATUS_OKAY(NG_SYM_INST)
