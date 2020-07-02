#pragma once

#define FROM_BIG_ENDIAN(hi, lo) (((hi) << 8) | (lo))

void serialDumpBuffer(uint8_t* buffer, uint16_t len);