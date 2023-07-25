#ifndef PADDING_MD2_H
#define PADDING_MD2_H

#include <stdint.h> // uint8_t
#include <emmintrin.h>
#include <string.h> // memset()

void padding_basicOpt_V1(size_t len, uint8_t *buf);

void padding_opt_V0(size_t len, uint8_t *buf);

#endif
