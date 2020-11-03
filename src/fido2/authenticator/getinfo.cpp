#include <memory>

#include <Arduino.h>

#include "config.h"

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::GetInfo *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("## GetInfo");

            std::unique_ptr<FIDO2::CTAP::Response::GetInfo> resp = std::unique_ptr<FIDO2::CTAP::Response::GetInfo>(new FIDO2::CTAP::Response::GetInfo());

            // List of supported versions. Supported versions are: "FIDO_2_0" for CTAP2 / FIDO2 / Web Authentication authenticators
            // and "U2F_V2" for CTAP1/U2F authenticators.
            resp->versions.push_back("FIDO_2_0");
            // resp->versions.push_back("FIDO_2_1_PRE");
            // resp->versions.push_back("FIDO_2_1");

            // List of supported extensions
            resp->extensions.push_back("hmac-secret");
            // resp->extensions.push_back("credProtect");

            // The claimed AAGUID. 16 bytes in length and encoded the same as MakeCredential AuthenticatorData, as specified in [WebAuthn]
            resp->aaguid = FIDO2::Authenticator::aaguid;

            //List of supported options.
            resp->options.plat = false;
            resp->options.rk = true;
            resp->options.clientPinSupported = true;
            resp->options.clientPin = pinIsSet ? true : false;
            resp->options.up = false;
            resp->options.uvSupported = true;
            resp->options.uv = true;

            // Maximum message size supported by the authenticator.
            resp->maxMsgSize = std::unique_ptr<uint16_t>(new uint16_t(2048));

            // List of supported PIN/UV protocol versions.
            resp->pinUvAuthProtocols = std::unique_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>());
            resp->pinUvAuthProtocols->push_back(1);

            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }
    } // namespace Authenticator
} // namespace FIDO2