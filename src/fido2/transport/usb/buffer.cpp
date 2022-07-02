#include <Arduino.h>

#if defined(FIDO2_TRANSPORT_USB)

#include "fido2/transport/usb/buffer.h"

#include "ctaphid.h"
#include "util/util.h"
#include "util/be.h"

namespace FIDO2
{
    namespace Transport
    {
        namespace USB
        {
          
            CommandBuffer commandBuffer;

            void CommandBuffer::reset()
            {
                position = 0;
            }

            uint16_t CommandBuffer::init(const uint8_t *data, const uint16_t length)
            {
                if (length > FIDO2_MAX_MSG_SIZE){
                    return 0;
                }

                memcpy(buffer, data, length);

                const uint16_t p_len = getPayloadLength();
                position = MIN(p_len + 7, length);

                return length;
            }

            uint16_t CommandBuffer::append(const uint8_t *data, const uint16_t length)
            {
                if (position + length - 1 > FIDO2_MAX_MSG_SIZE){
                    return 0;
                }

                memcpy(buffer + position, data + CID_LENGTH + 1, length - CID_LENGTH - 1);
                const uint16_t p_len = getPayloadLength();
                position += MIN(p_len - position - 1, length - CID_LENGTH - 1);

                return length;
            }

            uint8_t* CommandBuffer::getCid()
            {
                return buffer;
            }

            uint8_t* CommandBuffer::newCid()
            {
                cid[0] = 0x01;
                cid[1] = 0x02;
                cid[2] = 0x03;
                cid[3] = 0x04;
                return cid;
            }

            uint8_t CommandBuffer::getCmd()
            {
                return buffer[CID_LENGTH] & 0x7F;
            }

            uint8_t *CommandBuffer::getBuffer()
            {
                return buffer;
            }

            uint16_t CommandBuffer::getBufferLength()
            {
                return position;
            }

            size_t CommandBuffer::getMaxBufferLength()
            {
                return FIDO2_MAX_MSG_SIZE;
            }

            bool CommandBuffer::isComplete()
            {
                return buffer[0] != 0 && position == getPayloadLength() + 7;
            }

            uint8_t *CommandBuffer::getPayload()
            {
                return buffer + 7;
            }

            uint16_t CommandBuffer::getPayloadLength()
            {
                return be_uint16_t(buffer + 5);
            }

            void CommandBuffer::setPayloadLength(uint16_t length)
            {
                // buffer[5] = buffer[4];
                // buffer[4] = buffer[3];
                // buffer[3] = buffer[2];
                // buffer[2] = buffer[1];
                // buffer[1] = buffer[0];
                // buffer[6] = length & 0xFF;
                buffer[5] = (length >> 8) & 0xFF;
                buffer[6] = length & 0xFF;

                // position = length + 6; 
                position = length + 7;
            }

        } // namespace BLE
    }     // namespace Transport
} // namespace FIDO2

#endif