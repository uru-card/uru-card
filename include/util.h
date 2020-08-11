#pragma once

#define FROM_BIG_ENDIAN(hi, lo) (((hi) << 8) | (lo))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void serialDumpBuffer(uint8_t *buffer, size_t len);