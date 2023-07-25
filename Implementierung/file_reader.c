#include "file_reader.h"

uint8_t *read_file(const char *filePath, size_t *contentLength)
{
    // Pointer to hold the file content
    uint8_t *input = NULL;
    FILE *file;

    // Open the file in binary read mode
    if (!(file = fopen(filePath, "rb")))
    {
        fprintf(stderr, "Error by opening the file! (possible reason: wrong path)\n");
        goto cleanup;
    }

    struct stat statbuf; // Structure to store file statistics

    // Get the file statistics
    if (fstat(fileno(file), &statbuf))
    {
        fprintf(stderr, "Error by reading stats!\n");
        goto cleanup;
    }

    // Check if the file is a regular file and has a valid size
    if (!S_ISREG(statbuf.st_mode) || statbuf.st_size < 0)
    {
        fprintf(stderr, "Error by processing file or invalid size! (for MD2: must > 0)\n");
        goto cleanup;
    }

    // Store the file size in contentLength variable
    *contentLength = statbuf.st_size;

    // Allocate memory for input, adding extra padding to ensure 16-byte alignment and also space for 16 bytes checksum
    if (!(input = malloc(statbuf.st_size + 32 - statbuf.st_size % 16)))
    {
        fprintf(stderr, "Error by allocating memory!\n");
        goto cleanup;
    }

    // Read the content of the file into the input buffer
    if ((size_t)statbuf.st_size != fread(input, 1, statbuf.st_size, file))
    {
        fprintf(stderr, "Error by transforming the inputfile into string!\n");
        free(input);
        input = NULL;
        goto cleanup;
    }

cleanup:
    if (file)
    {
        fclose(file);
    }

    // Return the file content (or NULL in case of error)
    return input;
}
