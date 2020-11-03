#pragma once

#include "config.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#ifdef DEBUG_EXCEPTIONS
#define RAISE(exception)                                                                                 \
    {                                                                                                    \
        Serial.printf("!> Raised exctption in function %s (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__); \
        throw exception;                                                                                 \
    }
#else
#define RAISE(exception) throw exception;
#endif

void serialDumpBuffer(const uint8_t *buffer, const size_t len);