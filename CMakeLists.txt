# このリポジトリ自身をソースコード提供モジュールとしても使うための
# 追加分。既存のシールド定義(zephyr/module.yml の board_root)に加えて、
# 新規カスタムビヘイビア zmk,behavior-velocity-mouse-move をビルドに含める。

zephyr_library()
zephyr_library_sources_ifdef(
  CONFIG_ZMK_BEHAVIOR_VELOCITY_MOUSE_MOVE
  src/behaviors/behavior_velocity_mouse_move.c
)
