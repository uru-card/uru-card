#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode RequestGetInfo::getCommandCode()
        {
            return authenticatorGetInfo;
        }

        Command *parseRequestGetInfo(const uint8_t *data, const size_t length)
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

        void ResponseGetInfo::setAAGUID(FIDO2::UUID _aaguid)
        {
            aaguid = _aaguid;
        }

        FIDO2::UUID ResponseGetInfo::getAAGUID()
        {
            return aaguid;
        }

        Status encodeResponse(ResponseGetInfo *response, uint8_t *data, size_t &length)
        {
            // use external buffer?
            CBORPair cbor = CBORPair();

            // List of supported versions.
            if (response->getVersions().size() > 0)
            {
                CBORArray versions = CBORArray();
                for (auto it = response->getVersions().begin(); it != response->getVersions().end(); it++)
                {
                    versions.append((*it).c_str());
                }
                cbor.append(0x01, versions);
            }

            // // List of supported extensions
            if (response->getExtensions().size() > 0)
            {
                CBORArray extensions = CBORArray();
                for (auto it = response->getExtensions().begin(); it != response->getExtensions().end(); it++)
                {
                    extensions.append((*it).c_str());
                }
                cbor.append(0x02, extensions);
            }

            // AAGUID
            CBOR cbor_aaguid;
            cbor_aaguid.encode(response->getAAGUID().bytes(), 16);
            cbor.append(0x03, cbor_aaguid);

            // Map of options
            CBORPair options = CBORPair();
            options.append("clientPin", true);
            options.append("plat", false);
            options.append("rk", true);
            options.append("up", false);
            options.append("uv", true);
            cbor.append(0x04, options);

            // // max msg size
            cbor.append(0x05, 2048);

            // finalize the encoding
            data[0] = CTAP2_OK;
            memcpy(data + 1, cbor.to_CBOR(), cbor.length());
            length = cbor.length() + 1;

            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2