#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode Request::GetInfo::getCommandCode() const
        {
            return authenticatorGetInfo;
        }

        Status parseRequestGetInfo(const uint8_t *data, const size_t len, std::unique_ptr<Command> &request)
        {
            request = std::unique_ptr<Request::GetInfo>(new Request::GetInfo());

            return CTAP2_OK;
        }

        CommandCode Response::GetInfo::getCommandCode() const
        {
            return authenticatorGetInfo;
        }

        Status encodeResponse(const Response::GetInfo *response, uint8_t *data, size_t &len)
        {
            // use external buffer?
            CBORPair cbor = CBORPair();

            // List of supported versions.
            if (response->versions.size() > 0)
            {
                CBORArray versions = CBORArray();
                for (auto it = response->versions.begin(); it != response->versions.end(); it++)
                {
                    versions.append((*it).c_str());
                }
                cbor.append(0x01, versions);
            }

            // // List of supported extensions
            if (response->extensions.size() > 0)
            {
                CBORArray extensions = CBORArray();
                for (auto it = response->extensions.begin(); it != response->extensions.end(); it++)
                {
                    extensions.append((*it).c_str());
                }
                cbor.append(0x02, extensions);
            }

            // AAGUID
            CBOR cbor_aaguid;
            cbor_aaguid.encode(response->aaguid.get_bytes(), 16);
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
            len = cbor.length() + 1;

            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2