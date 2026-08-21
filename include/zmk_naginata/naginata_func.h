#pragma once
#include <zmk_naginata/naginata_config.h>

void naginata_on(void);
void naginata_set_timestamp(int64_t ts);
int64_t naginata_get_timestamp(void);
// void naginata_off(void);
void nofunc(void);
void switch_to_hex_input(void);
void return_to_kana_input(void);
void press_compose_key(void);
void release_compose_key(void);
void input_unicode_hex(int, int, int, int);

void ng_T(void);
void ng_Y(void);
void ng_ST(void);
void ng_SY(void);
void ngh_JKQ(void);
void ngh_JKW(void);
void ngh_JKE(void);
void ngh_JKR(void);
void ngh_JKT(void);
void ngh_JKA(void);
void ngh_JKS(void);
void ngh_JKD(void);
void ngh_JKF(void);
void ngh_JKG(void);
void ngh_JKZ(void);
void ngh_JKX(void);
void ngh_JKC(void);
void ngh_JKV(void);
void ngh_JKB(void);
void ngh_DFY(void);
void ngh_DFU(void);
void ngh_DFI(void);
void ngh_DFO(void);
void ngh_DFP(void);
void ngh_DFH(void);
void ngh_DFJ(void);
void ngh_DFK(void);
void ngh_DFL(void);
void ngh_DFSCLN(void);
void ngh_DFN(void);
void ngh_DFM(void);
void ngh_DFCOMM(void);
void ngh_DFDOT(void);
void ngh_DFSLSH(void);
void ngh_MCQ(void);
void ngh_MCW(void);
void ngh_MCE(void);
void ngh_MCR(void);
void ngh_MCT(void);
void ngh_MCA(void);
void ngh_MCS(void);
void ngh_MCD(void);
void ngh_MCF(void);
void ngh_MCG(void);
void ngh_MCZ(void);
void ngh_MCX(void);
void ngh_MCC(void);
void ngh_MCV(void);
void ngh_MCB(void);
void ngh_CVY(void);
void ngh_CVU(void);
void ngh_CVI(void);
void ngh_CVO(void);
void ngh_CVP(void);
void ngh_CVH(void);
void ngh_CVJ(void);
void ngh_CVK(void);
void ngh_CVL(void);
void ngh_CVSCLN(void);
void ngh_CVN(void);
void ngh_CVM(void);
void ngh_CVCOMM(void);
void ngh_CVDOT(void);
void ngh_CVSLSH(void);
void ng_cut(void);
void ng_copy(void);
void ng_paste(void);
void ng_up(uint8_t);
void ng_down(uint8_t);
void ng_left(uint8_t);
void ng_right(uint8_t);
void ng_next_row(void);
void ng_prev_row(void);
void ng_next_char(void);
void ng_prev_char(void);
void ng_home(void);
void ng_end(void);
void ng_katakana(void);
void ng_save(void);
void ng_hiragana(void);
void ng_redo(void);
void ng_undo(void);
void ng_saihenkan(void);
void ng_eof(void);

// ここから下：システム③（Colemak+薙刀式）のL1・R6用に追加。
// ＜＞はngh_JK*系に既存の実装が無かったため新規追加。
// （）「」『』はngh_JKG/JKB/JKF/JKV/JKS/JKXを再利用するのでここでの追加は不要。
void ng_lt(void);  // ＜ U+FF1C
void ng_gt(void);  // ＞ U+FF1E
void ng_colon_full(void);   // ： U+FF1A
void ng_slash_full(void);   // ／ U+FF0F（ngh_JKWと同一内容。層③記号レイヤーから単独で呼べるように別名で追加）
void ng_question_noenter(void); // ？（確定改行なし版。ngh_JKDは末尾に確定改行が入るため、
                                 // 文中でも使えるようこちらは改行なしにした）
void ng_exclaim_noenter(void);  // ！（同上、改行なし版）

// L6/R1ホールド中に見せかけのIME force-set（絶対指定）を送る。
// OSごとの分岐はnaginata_config.osを流用。iPadOSでの絶対指定挙動は
// 実機未検証のためLANG1/LANG2をそのまま送るのみ（システム③設計ガイド4-3参照）。
void ng_os_ime_japanese(void);
void ng_os_ime_english(void);
