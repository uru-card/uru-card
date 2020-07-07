#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode RequestGetInfo::getCommandCode()
        {
            return authenticatorGetInfo;
        }

        Command parseRequestGetInfo(const uint8_t *data, const uint16_t length)
        {
            Serial.println(__FUNCTION__);

            // validate

            //
            return RequestGetInfo();
        }

        CommandCode ResponseGetInfo::getCommandCode()
        {
            return authenticatorGetInfo;
        }

        Status encodeResponseGetInfo(ResponseGetInfo &response, uint8_t *data, uint16_t *length)
        {
            Serial.println(__FUNCTION__);

            CBORPair cbor = CBORPair(100);

            // List of supported versions.
            CBORArray versions = CBORArray();
            versions.append("FIDO_2_0");

            cbor.append(0x01, versions);

            //
            *length = cbor.length();

            //
            memcpy(data, cbor.to_CBOR(), *length);

            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2