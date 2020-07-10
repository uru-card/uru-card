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

        std::vector<String> &ResponseGetInfo::getExtensions()
        {
            return extensions;
        }

        BLEUUID &ResponseGetInfo::getAAGUID()
        {
            return aaguid;
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

            // List of supported extensions
            CBORArray extensions = CBORArray();
            cbor.append(0x02, extensions);

            // AAGUID
            // BLEUUID aaguid("63d9df31-662d-476a-a7a7-53b6aa038975");
            // CBOR cbor_aaguid = CBOR(aaguid.getNative()->uuid.uuid128, 16);
            // cbor.append(0x03, cbor_aaguid);

            // Map of options
            CBORPair options = CBORPair();
            options.append("clientPin", true);
            options.append("plat", false);
            options.append("rk", true);
            options.append("up", false);
            options.append("uv", true);

            cbor.append(0x04, options);

            // max msg size
            cbor.append(0x05, 2048);

            // finalize the encoding
            *length = cbor.length() + 1;

            data[0] = 0;
            memcpy(data + 1, cbor.to_CBOR(), *length);

            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2