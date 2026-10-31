#pragma once
#include <stdint.h>

#define NG_L1_R3 0x01
#define NG_L1_R4 0x02
#define NG_R6_R3 0x03
#define NG_R6_R4 0x04
#define NG_THUMB_ENTER 0x06
#define NG_R6_R2 0x07
#define NG_THUMB_COMMA 0x08
#define NG_L6_R2 0x09
#define NG_R1_R2 0x0A
#define NG_THUMB_BS 0x0B

void naginata_set_timestamp(int64_t ts);
int64_t naginata_get_timestamp(void);
void nofunc(void);
