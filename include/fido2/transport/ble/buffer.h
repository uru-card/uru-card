#pragma once

#include <Arduino.h>

namespace FIDO2
{
    namespace Transport
    {
        namespace BLE
        {
            class CommandBuffer
            {
            public:
                static const uint16_t MAX_LENGTH = 2048;

            public:
                void reset();

                void setCmd(const uint8_t cmd);
                uint8_t getCmd();

                void setLength(const uint16_t len);
                uint16_t getLength();

                bool isComplete();
                uint8_t *getPayload();

                uint16_t appendFragment(const uint8_t *data, const uint16_t len);

            protected:
                uint8_t cmd;
                uint8_t payload[MAX_LENGTH - 1];
                uint16_t length;
                uint16_t pos;
            };

            extern CommandBuffer commandBuffer;

        } // namespace BLE
    }     // namespace Transport
} // namespace FIDO2