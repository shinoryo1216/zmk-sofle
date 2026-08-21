/*
 * システム③（Colemak+薙刀式）追加分。
 * &ng_sym behavior のparam1に渡す定数。
 * .keymapファイル側で #include <dt-bindings/zmk_naginata/symbols.h> して使う。
 */
#pragma once

#define NG_SYM_PAREN_OPEN    0  // （ U+FF08  … ngh_JKGを再利用
#define NG_SYM_PAREN_CLOSE   1  // ） U+FF09  … ngh_JKBを再利用
#define NG_SYM_KAGI_OPEN     2  // 「 U+300C  … ngh_JKFを再利用
#define NG_SYM_KAGI_CLOSE    3  // 」 U+300D  … ngh_JKVを再利用
#define NG_SYM_NIKAGI_OPEN   4  // 『 U+300E  … ngh_JKSを再利用
#define NG_SYM_NIKAGI_CLOSE  5  // 』 U+300F  … ngh_JKXを再利用
#define NG_SYM_LT            6  // ＜ U+FF1C  … 新規 ng_lt
#define NG_SYM_GT            7  // ＞ U+FF1E  … 新規 ng_gt
#define NG_SYM_IME_JAPANESE  8  // IME force-set：日本語（ng_os_ime_japanese）
#define NG_SYM_IME_ENGLISH   9  // IME force-set：英語（ng_os_ime_english）
#define NG_SYM_COLON_FULL    10 // ： U+FF1A … 新規 ng_colon_full
#define NG_SYM_SLASH_FULL    11 // ／ U+FF0F … 新規 ng_slash_full（ngh_JKWと同内容）
#define NG_SYM_QUESTION      12 // ？（改行なし） … 新規 ng_question_noenter
#define NG_SYM_EXCLAIM       13 // ！（改行なし） … 新規 ng_exclaim_noenter
