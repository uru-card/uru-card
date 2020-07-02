#include <Arduino.h>

#include "fido2/transport/ble/buffer.h"

namespace FIDO2
{
    namespace Transport
    {
        namespace BLE
        {
            CommandBuffer commandBuffer;

            void CommandBuffer::reset()
            {
                buffer[0] = 0;
                expectedLength = 0;
                position = 0;
            }

            void CommandBuffer::setCmd(const uint8_t cmd)
            {
                buffer[0] = cmd;
            }

            uint8_t CommandBuffer::getCmd()
            {
                return buffer[0];
            }

            void CommandBuffer::setExpectedLength(const uint16_t len)
            {
                expectedLength = len;
            }

            uint8_t *CommandBuffer::getBuffer()
            {
                return buffer;
            }

            uint16_t CommandBuffer::getLength()
            {
                return position;
            }

            bool CommandBuffer::isComplete()
            {
                return buffer[0] != 0 && position == expectedLength;
            }

            uint8_t *CommandBuffer::getPayload()
            {
                return buffer + 1;
            }

            uint16_t CommandBuffer::appendFragment(const uint8_t *data, const uint16_t len)
            {
                if (position + len > MAX_LENGTH)
                {
                    return 0;
                }

                memcpy(buffer + position + 1, data, len);
                position += len;

                return len;
            }

        } // namespace BLE
    }     // namespace Transport
} // namespace FIDO2