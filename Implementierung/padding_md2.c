#include "padding_md2.h"

//------------------------------------------------ padding -------------------------------------------------

// For padding functions V0 and V1, PKCS#7 padding is applied, where the padding value is equal to the padding size
void padding_basicOpt_V1(size_t len, uint8_t *buf)
{
    // Calculate the padding size needed to align the length to a multiple of 16
    size_t paddingSize = 16 - (len % 16);

    // Fill the padding bytes with the value of paddingSize using the library function memset
    memset((buf + len), (uint8_t)paddingSize, paddingSize);
}

void padding_opt_V0(size_t len, uint8_t *buf)
{
    size_t padding_size = 16 - (len % 16);

    // Fill the padding bytes with the value of padding_size using SIMD, the extra added bytes will be overwritten in the checksum step
    __m128i padding_value = _mm_set1_epi8((uint8_t)padding_size);
    _mm_storeu_si128((__m128i *)(buf + len), padding_value);
}
