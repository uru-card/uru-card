#pragma once

#if defined(FIDO2_TRANSPORT_USB)

#include <Arduino.h>

namespace FIDO2
{
    namespace U2F
    {

        static const uint8_t U2F_REGISTRATION = 0x01;
        static const uint8_t U2F_AUTHENTICATION = 0x02;
        static const uint8_t U2F_GET_VERSION = 0x03;

        uint16_t processMSG(uint8_t* buffer);

    } // namespace U2F
} // namespace FIDO2

#endif