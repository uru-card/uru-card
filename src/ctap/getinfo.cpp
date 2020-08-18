#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        namespace Request
        {
            CommandCode GetInfo::getCommandCode() const
            {
                return authenticatorGetInfo;
            }

            Status parseGetInfo(const CBOR &cbor, std::unique_ptr<Command> &request)
            {
                request = std::unique_ptr<GetInfo>(new GetInfo());

                return CTAP2_OK;
            }
        } // namespace Request

        namespace Response
        {
            CommandCode GetInfo::getCommandCode() const
            {
                return authenticatorGetInfo;
            }

            Status encode(const GetInfo *response, std::unique_ptr<CBOR> &cbor)
            {
                // use external buffer?
                std::unique_ptr<CBORPair> cborPair(new CBORPair());

                // List of supported versions.
                if (response->versions.size() > 0)
                {
                    CBORArray versions = CBORArray();
                    for (auto it = response->versions.begin(); it != response->versions.end(); it++)
                    {
                        versions.append((*it).c_str());
                    }
                    cborPair->append(0x01, versions);
                }

                // // List of supported extensions
                if (response->extensions.size() > 0)
                {
                    CBORArray extensions = CBORArray();
                    for (auto it = response->extensions.begin(); it != response->extensions.end(); it++)
                    {
                        extensions.append((*it).c_str());
                    }
                    cborPair->append(0x02, extensions);
                }

                // AAGUID
                CBOR cbor_aaguid;
                cbor_aaguid.encode(response->aaguid.get_bytes(), 16);
                cborPair->append(0x03, cbor_aaguid);

                // Map of options
                CBORPair options = CBORPair();
                if (response->options.clientPinSupported)
                {
                    options.append("clientPin", response->options.clientPin);
                }
                options.append("plat", response->options.plat);
                options.append("rk", response->options.rk);
                options.append("up", response->options.up);
                if (response->options.uvSupported)
                {
                    options.append("uv", response->options.uv);
                }
                cborPair->append(0x04, options);

                // max msg size
                cborPair->append(0x05, response->maxMsgSize);

                // List of supported PIN/UV protocol versions.
                CBORArray cborVersions;
                cborVersions.append((uint8_t)0x01);
                cborPair->append(0x06, cborVersions);

                // maxCredentialIdLength
                cborPair->append(0x08, (uint8_t)16);

                // List of supported transports
                CBORArray cborTransports;
                cborTransports.append("ble");
                cborPair->append(0x09, cborTransports);

                // finalize the encoding
                cbor = std::unique_ptr<CBOR>(new CBOR(*cborPair));

                return CTAP2_OK;
            }
        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2