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

zephyr_library()
zephyr_library_sources_ifdef(
  CONFIG_ZMK_BEHAVIOR_VELOCITY_MOUSE_MOVE
  src/behaviors/behavior_velocity_mouse_move.c
)
