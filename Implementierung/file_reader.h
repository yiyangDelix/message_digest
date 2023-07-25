#ifndef FILE_READER_H
#define FILE_READER_H

#include <sys/stat.h> // struct stat statbuf;
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t *read_file(const char *filePath, size_t *contentLength);

#endif
