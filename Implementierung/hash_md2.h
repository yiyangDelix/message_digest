#ifndef HASH_MD2_H
#define HASH_MD2_H

#include <stdint.h> // uint8_t
#include <emmintrin.h>
#include <string.h> // memcpy()
#include "checksum_md2.h"

void md2_hash_basicOpt_V1(size_t len, const uint8_t buf[len], uint8_t out[16]);

void md2_hash_opt_V0(size_t len, const uint8_t buf[len], uint8_t out[16]);

#endif
