# このリポジトリ自身をソースコード提供モジュールとしても使うための
# 追加分。既存のシールド定義(zephyr/module.yml の board_root)に加えて、
# 新規カスタムビヘイビア zmk,behavior-velocity-mouse-move をビルドに含める。
#
# 【重要】ZMK本体（zmk/app/CMakeLists.txt）は
#   target_include_directories(app PRIVATE include)
# という設定で、drivers/behavior.h や zmk/behavior.h が置かれている
# zmk/app/include を「appターゲット自身にしか見えないPRIVATE」として
# 追加している。このリポジトリのようなアプリ本体とは別の外部モジュール
# （zephyr_library()）からは、そのままではこのincludeパスが見えず、
# 「drivers/behavior.h: No such file or directory」のようなエラーになる。
# そのため、ZMK本体のincludeディレクトリを明示的に（グローバルな
# zephyr_include_directories() で）追加してやる必要がある。
zephyr_include_directories(${APPLICATION_SOURCE_DIR}/include)

# このリポジトリ自身のinclude/（zmk_naginata/*.h, dt-bindings/zmk_naginata/*.h）を
# 見えるようにする。上のAPPLICATION_SOURCE_DIR/includeとは別物（ZMK本体側 vs この
# モジュール自身の追加ヘッダ）なので両方必要。
zephyr_include_directories(include)

zephyr_library()
zephyr_library_sources_ifdef(
  CONFIG_ZMK_BEHAVIOR_VELOCITY_MOUSE_MOVE
  src/behaviors/behavior_velocity_mouse_move.c
)

# ここから下：システム③（Colemak Mod-DH + 薙刀式）用に追加。
# 元はeswai/zmk-naginataという別リポジトリだったが、別リポジトリに分ける必要が
# 特になかったため、このリポジトリ（あなた自身のファームウェア本体）に
# ソースごと取り込んだ。west.ymlの変更は一切不要。
# 分割キーボードの場合、薙刀式エンジンはセントラル側だけで動けばよいので、
# 元のzmk-naginataのCMakeLists.txtと同じガード条件をそのまま踏襲する。
if (CONFIG_NAGINATA AND ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL))
  zephyr_library_sources(
    src/naginata/behaviors/behavior_naginata.c
    src/naginata/behaviors/behavior_naginata_symbol.c
    src/naginata/naginata_func.c
    src/naginata/nglist.c
    src/naginata/nglistarray.c
  )
endif()
