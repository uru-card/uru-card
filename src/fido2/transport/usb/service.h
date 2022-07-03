#pragma once

#if defined(FIDO2_TRANSPORT_USB)

#include "ctaphid.h"

namespace FIDO2
{
    namespace Transport
    {
        namespace USB
        {

            // mandatory commands
            static const uint8_t CMD_MSG = 0x03;
            static const uint8_t CMD_CBOR = 0x10;
            static const uint8_t CMD_INIT = 0x06;
            static const uint8_t CMD_PING = 0x01;
            static const uint8_t CMD_CANCEL = 0x11;
            static const uint8_t CMD_ERROR = 0x3f;
            static const uint8_t CMD_KEEPALIVE = 0x3b;
            // optional commands
            static const uint8_t CMD_WINK = 0x08;
            static const uint8_t CMD_LOCK = 0x04;

            void processRequest();
            void processINIT();
            void processCBOR();
            void processMSG();
            void processWINK();
            void sendResponse();

            class Service
            {
            public:
                static bool init();
                static void setCallback(void (*fun)(uint8_t const* buffer, uint16_t bufsize));
                static void flush();
            };

            class HIDCallbacksImpl : public HIDCallbacks {
                void onData(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);
            };

        } // namespace USB
    }     // namespace Transport
} // namespace FIDO2

#endif