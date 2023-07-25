#ifndef IMPLEMENTATION_MD2_H
#define IMPLEMENTATION_MD2_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "padding_md2.h"
#include "checksum_md2.h"
#include "hash_md2.h"
#include "file_reader.h"

int implementation_md2(int timeMeasurementFlag, long impV, size_t repeat, const char *inputFilePath);

#endif
