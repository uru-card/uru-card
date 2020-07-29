#include <Arduino.h>

#include "fido2/uuid.h"

namespace FIDO2
{
    UUID::UUID()
    {
        memset(uuid, 0, 16);
    }

    UUID::UUID(const UUID &_uuid)
    {
        memcpy(uuid, _uuid.uuid, 16);
    }

    UUID::UUID(const char *_uuid_str)
    {
        int n = 0;
        for (int i = 0; i < 36;)
        {
            if (_uuid_str[i] == '-')
                i++;
            uint8_t MSB = _uuid_str[i];
            uint8_t LSB = _uuid_str[i + 1];

            if (MSB > '9')
                MSB -= 7;
            if (LSB > '9')
                LSB -= 7;
            uuid[n++] = ((MSB & 0x0F) << 4) | (LSB & 0x0F);
            i += 2;
        }
    }

    const uint8_t *UUID::get_bytes() const
    {
        return uuid;
    }
} // namespace FIDO2