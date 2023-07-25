#ifndef CHECKSUM_MD2_H
#define CHECKSUM_MD2_H

#include <stdint.h> // uint8_t
#include <emmintrin.h>
#include <string.h> // memcpy()

extern const uint8_t PI_SUBST[256];

void md2_checksum_basicOpt_V1(size_t len, uint8_t *buf);

void md2_checksum_opt_V0(size_t len, uint8_t *buf);

#endif
