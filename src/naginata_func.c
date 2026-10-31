#include <stdint.h>
#include <zmk_naginata/naginata_func.h>

static int64_t timestamp;

void naginata_set_timestamp(int64_t ts) { timestamp = ts; }
int64_t naginata_get_timestamp(void) { return timestamp; }

void nofunc(void) {}
