#include <Arduino.h>

#include "fido2/transport/ble/buffer.h"

#include "util.h"

namespace FIDO2
{
    namespace Transport
    {
        namespace BLE
        {
            CommandBuffer commandBuffer;

            uint16_t CommandBuffer::init(const uint8_t *data, const uint16_t length)
            {
                if (length > MAX_LENGTH)
                {
                    return 0;
                }

                memcpy(buffer, data, length);
                position = length;

                return length;
            }

            uint16_t CommandBuffer::append(const uint8_t *data, const uint16_t length)
            {
                if (position + length - 1 > MAX_LENGTH)
                {
                    return 0;
                }

                memcpy(buffer + position, data + 1, length - 1);
                position += length - 1;

                return length;
            }

            uint8_t CommandBuffer::getCmd()
            {
                return buffer[0];
            }

            uint8_t *CommandBuffer::getBuffer()
            {
                return buffer;
            }

            uint16_t CommandBuffer::getBufferLength()
            {
                return position;
            }

            bool CommandBuffer::isComplete()
            {
                return buffer[0] != 0 && position == getPayloadLength() + 3;
            }

            uint8_t *CommandBuffer::getPayload()
            {
                return buffer + 3;
            }

            uint16_t CommandBuffer::getPayloadLength()
            {
                return FROM_BIG_ENDIAN(buffer[1], buffer[2]);
            }

            void CommandBuffer::setPayloadLength(uint16_t length)
            {
                buffer[1] = (length >> 8) & 0xFF;
                buffer[2] = length & 0xFF;
            }

        } // namespace BLE
    }     // namespace Transport
} // namespace FIDO2