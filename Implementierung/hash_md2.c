#include "hash_md2.h"
//------------------------------------------------- hash ---------------------------------------------------

void substituteAndPermuteRounds(uint8_t *md_digest)
{
    uint8_t checktmp = 0;

    // Perform 18 rounds of substitution and permutation on md_digest array
    for (uint8_t j = 0; j < 18; j++)
    {
        for (uint8_t k = 0; k < 48; k++)
        {
            // Set checktmp and md_digest[k] to (md_digest[k] xor PI_SUBST[checktmp])
            checktmp = md_digest[k] ^ PI_SUBST[checktmp];
            md_digest[k] = checktmp;
        }
        checktmp = (checktmp + j) % 256;
    }
}

void md2_hash_basicOpt_V1(size_t len, const uint8_t buf[len], uint8_t out[16])
{
    // Initialize MD digest
    uint8_t md_digest[48] = {0};

    // Process input buffer in blocks of 16 bytes
    for (size_t i = 0; i < len / 16; i++)
    {
        // Copy the current 16-byte block from the input buffer to md_digest[16:31]
        memcpy((md_digest + 16), (buf + 16 * i), 16);

        // Perform XOR operation between md_digest[0:15] and md_digest[16:31] and store the result in md_digest[32:47]
        for (size_t j = 0; j < 16; j++)
        {
            md_digest[j + 32] = md_digest[j] ^ md_digest[j + 16];
        }

        substituteAndPermuteRounds(md_digest);
    }

    // Copy md_digest[0:15], which is the hash value, to the output array using memcpy
    memcpy(out, md_digest, 16);
}

void md2_hash_opt_V0(size_t len, const uint8_t buf[len], uint8_t out[16])
{
    uint8_t md_digest[48] = {0};

    for (size_t i = 0; i < len / 16; i++)
    {
        // Load 16 bytes of input buffer and store them in md_digest[16:31] using SIMD
        __m128i input = _mm_loadu_si128((__m128i *)(buf + (16 * i)));
        _mm_storeu_si128((__m128i *)(md_digest + 16), input);

        // Perform XOR operation between md_digest[0:15] and md_digest[16:31] and store the result in md_digest[32:47] using SIMD
        __m128i temp = _mm_loadu_si128((__m128i *)(md_digest));
        __m128i result = _mm_xor_si128(temp, input);
        _mm_storeu_si128((__m128i *)(md_digest + 32), result);

        substituteAndPermuteRounds(md_digest);
    }

    // Copy md_digest[0:15], which is the hash value, to the output array using SIMD
    __m128i md_digest_vec = _mm_loadu_si128((__m128i *)md_digest);
    _mm_storeu_si128((__m128i *)out, md_digest_vec);
}
