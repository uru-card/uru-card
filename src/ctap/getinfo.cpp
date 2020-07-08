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

        Command *parseRequestGetInfo(const uint8_t *data, const uint16_t length)
        {
            // validate

            //
            return new RequestGetInfo();
        }

        CommandCode ResponseGetInfo::getCommandCode()
        {
            return authenticatorGetInfo;
        }

        std::vector<String> &ResponseGetInfo::getVersions()
        {
            return versions;
        }

        Status encodeResponse(ResponseGetInfo *response, uint8_t *data, uint16_t *length)
        {
            CBORPair cbor = CBORPair(100);

            // List of supported versions.
            CBORArray versions = CBORArray();
            for (auto it = response->getVersions().begin(); it != response->getVersions().end(); it++)
            {
                versions.append((*it).c_str());
            }
            cbor.append(0x01, versions);

            // finalize the encoding
            *length = cbor.length();
            memcpy(data, cbor.to_CBOR(), *length);

            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2