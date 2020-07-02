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
                cmd = 0;
                length = 0;
                pos = 0;
            }

            void CommandBuffer::setCmd(const uint8_t _cmd)
            {
                cmd = _cmd;
            }

            uint8_t CommandBuffer::getCmd()
            {
                return cmd;
            }

            void CommandBuffer::setLength(const uint16_t len)
            {
                length = len;
            }

            uint16_t CommandBuffer::getLength()
            {
                return length;
            }

            bool CommandBuffer::isComplete()
            {
                return cmd != 0 && pos == length;
            }

            uint8_t *CommandBuffer::getPayload()
            {
                return payload;
            }

            uint16_t CommandBuffer::appendFragment(const uint8_t *data, const uint16_t len)
            {
                if (pos + len > MAX_LENGTH)
                {
                    return 0;
                }

                memcpy(payload + pos, data, len);
                pos += len;

                return len;
            }

        } // namespace BLE
    }     // namespace Transport
} // namespace FIDO2