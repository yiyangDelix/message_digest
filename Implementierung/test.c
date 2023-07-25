#include "test.h"

void print_value(uint8_t *out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%02x", out[i]);
    }
    printf("\n");
}

int test(void)
{
    // The expected hash values for each test case
    uint8_t result[6][16] = {
        {0x83, 0x50, 0xe5, 0xa3, 0xe2, 0x4c, 0x15, 0x3d, 0xf2, 0x27, 0x5c, 0x9f, 0x80, 0x69, 0x27, 0x73},
        {0xc4, 0x3a, 0x34, 0xcb, 0x46, 0xce, 0x8d, 0x6b, 0xa7, 0x37, 0xb8, 0xc8, 0x8d, 0xf8, 0x2a, 0xfd},
        {0xe3, 0x63, 0xad, 0x3a, 0x21, 0x96, 0x6f, 0x88, 0x6b, 0xee, 0xe3, 0xa3, 0x37, 0x46, 0x1e, 0x8b},
        {0x97, 0x6e, 0xcd, 0x03, 0x52, 0xee, 0xc7, 0x0d, 0x56, 0xf8, 0x6c, 0x2c, 0xa2, 0x91, 0x9c, 0x3e},
        {0xce, 0x76, 0xa6, 0x10, 0xde, 0xc7, 0xb8, 0x6a, 0x6d, 0x9a, 0x22, 0x6f, 0x22, 0x98, 0xf6, 0x21},
        {0xd8, 0xbf, 0xd6, 0x50, 0x5a, 0xfe, 0xba, 0x04, 0xef, 0x54, 0xf1, 0x81, 0x0a, 0x86, 0x55, 0x54}};

    // The file paths for each test case
    char *testFilePaths[] = {
        "test_cases/1_empty.txt",
        "test_cases/26_72000_dif.txt",
        "test_cases/27_picsTest.png",
        "test_cases/28_GRA_0505.pdf",
        "test_cases/29_benchmark.tar",
        "test_cases/30_Makefile"};

    // Allocate memory for the output hash value
    uint8_t *out = (uint8_t *)malloc(16 * sizeof(uint8_t));
    if (out == NULL)
    {
        fprintf(stderr, "Memory allocation failed for the hashvalue in the testcase!\n");
        return EXIT_FAILURE;
    }

    // Loop through each test case
    for (int i = 0; i < 6; i++)
    {
        // Read the input file for the current test case
        char *testFilePath = testFilePaths[i];
        size_t testLength = 0;
        uint8_t *input = read_file(testFilePath, &testLength);
        if (input == NULL)
        {
            return EXIT_FAILURE;
        }
        size_t inputLengthWithPadding = testLength + 16 - testLength % 16;
        size_t inputLengthWithChecksum = inputLengthWithPadding + 16;

        printf("Test Case: %d------------------------------------------------------------------------\n", i + 1);
        printf("The expected hash value is:");
        print_value(result[i], 16);

        // Test V0
        printf("Testing V0\n");
        padding_opt_V0(testLength, input);
        md2_checksum_opt_V0(inputLengthWithPadding, input);
        md2_hash_opt_V0(inputLengthWithChecksum, input, out);
        printf("The Hashing value calculated by V0 is:");
        print_value(out, 16);
        if (memcmp(out, result[i], 16) == 0)
        {
            printf("TEST PASSED: The hash value caculated by V0 matches the expected value\n");
        }
        else
        {
            printf("TEST FAILED: The hash value caculated by V0 doesn't match the expected value\n");
        }

        // Reset the value to 0 so that the value calculated by V0 will not impact the testing of V1
        memset(out, 0, 16);

        // Test V1
        printf("Testing V1\n");
        padding_basicOpt_V1(testLength, input);
        md2_checksum_basicOpt_V1(inputLengthWithPadding, input);
        md2_hash_basicOpt_V1(inputLengthWithChecksum, input, out);
        printf("The Hashing value calculated by V1 is:");
        print_value(out, 16);
        if (memcmp(out, result[i], 16) == 0)
        {
            printf("TEST PASSED: The hash value caculated by V1 matches the expected value\n");
        }
        else
        {
            printf("TEST FAILED: The hash value caculated by V1 doesn't match the expected value\n");
        }

        printf("\n");

        free(input);
        input = NULL;
    }

    free(out);
    out = NULL;
    return EXIT_SUCCESS;
}
