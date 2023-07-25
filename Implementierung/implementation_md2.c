#include "implementation_md2.h"

void print_hashValue(uint8_t *out)
{
    printf("The hash value is: ");
    for (size_t i = 0; i < 16; i++)
    {
        printf("%02x", out[i]);
    }
    printf("\n");
}

int implementation_md2(int timeMeasurementFlag, long implementationVersion, size_t repeat, const char *inputFilePath)
{
    // Read input file
    size_t inputLength = 0;
    uint8_t *input = read_file(inputFilePath, &inputLength);

    if (input == NULL)
    {
        return EXIT_FAILURE;
    }

    // Calculate the lengths for file content with padding and checksum
    size_t inputLengthWithPadding = inputLength + 16 - inputLength % 16;
    size_t inputLengthWithChecksum = inputLengthWithPadding + 16;

    // Allocate memory for the output hash value
    uint8_t *out = (uint8_t *)malloc(16 * sizeof(uint8_t));
    if (out == NULL)
    {
        fprintf(stderr, "Memory allocation failed for the output hashvalue!\n");
        free(input);
        input = NULL;
        return EXIT_FAILURE;
    }

    //-------------------------------------- Call specified func & Print runtime ----------------------------------------
    if (timeMeasurementFlag)
    {
        switch (implementationVersion)
        {
        case 1:
        {
            // Version 1: Library-Optimized Implementation
            struct timespec start1;
            clock_gettime(CLOCK_MONOTONIC, &start1);

            for (size_t i = 0; i < repeat; i++)
            {
                padding_basicOpt_V1(inputLength, input);
                md2_checksum_basicOpt_V1(inputLengthWithPadding, input);
                md2_hash_basicOpt_V1(inputLengthWithChecksum, input, out);
            }

            struct timespec end1;
            clock_gettime(CLOCK_MONOTONIC, &end1);

            print_hashValue(out);

            // Calculate and print runtime
            double time1 = end1.tv_sec - start1.tv_sec + 1e-9 * (end1.tv_nsec - start1.tv_nsec);
            double avg1 = time1 / repeat;
            printf("The implementation version: V1 -> Library-Optimized\n");
            printf("The specified number of repetitions: %zu\n", repeat);
            printf("The total runtime: %lf\n", time1);
            printf("The average runtime of each: %lf\n", avg1);

            break;
        }
        default:
        {
            // Version 0: SIMD-Optimized Implementation
            struct timespec start0;
            clock_gettime(CLOCK_MONOTONIC, &start0);

            for (size_t i = 0; i < repeat; i++)
            {
                padding_opt_V0(inputLength, input);
                md2_checksum_opt_V0(inputLengthWithPadding, input);
                md2_hash_opt_V0(inputLengthWithChecksum, input, out);
            }

            struct timespec end0;
            clock_gettime(CLOCK_MONOTONIC, &end0);

            print_hashValue(out);

            // Calculate and print runtime
            double time0 = end0.tv_sec - start0.tv_sec + 1e-9 * (end0.tv_nsec - start0.tv_nsec);
            double avg0 = time0 / repeat;
            printf("The implementation version: V0 -> SIMD-Optimized\n");
            printf("The specified number of repetitions: %zu\n", repeat);
            printf("The total runtime: %lf\n", time0);
            printf("The average runtime of each: %lf\n", avg0);

            break;
        }
        }
    }
    else
    {
        switch (implementationVersion)
        {
        case 1:
            // Version 1: Library-Optimized Implementation
            padding_basicOpt_V1(inputLength, input);
            md2_checksum_basicOpt_V1(inputLengthWithPadding, input);
            md2_hash_basicOpt_V1(inputLengthWithChecksum, input, out);

            print_hashValue(out);
            printf("The implementation version: V1 -> Library-Optimized\n");

            break;
        default:
            // Version 0: SIMD-Optimized Implementation
            padding_opt_V0(inputLength, input);
            md2_checksum_opt_V0(inputLengthWithPadding, input);
            md2_hash_opt_V0(inputLengthWithChecksum, input, out);

            print_hashValue(out);
            printf("The implementation version: V0 -> SIMD-Optimized\n");

            break;
        }
    }

    // clean up
    free(input);
    input = NULL;
    free(out);
    out = NULL;

    return EXIT_SUCCESS;
}
