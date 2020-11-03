#include <Arduino.h>

#include "util/util.h"

/**
 * @brief Dump contents of the given buffer to the serial output
 */
void serialDumpBuffer(const uint8_t *buffer, const size_t len)
{
    const uint16_t pageSize = 40;
    uint16_t pages = (len / pageSize) + ((len % pageSize) ? 1 : 0);

    for (auto page = 0; page < pages; page++)
    {
        for (auto i = 0; i < pageSize; i++)
        {
            const uint16_t pos = page * pageSize + i;
            if (pos >= len)
                break;

            Serial.printf("%02x ", buffer[pos]);
        }
        Serial.println("");
    }
}