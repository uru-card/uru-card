#include <memory>

#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::GetInfo *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            std::unique_ptr<FIDO2::CTAP::Response::GetInfo> resp = std::unique_ptr<FIDO2::CTAP::Response::GetInfo>(new FIDO2::CTAP::Response::GetInfo());

            resp->versions.push_back("FIDO_2_0");
            resp->versions.push_back("FIDO_2_1_PRE");

            resp->aaguid = FIDO2::Authenticator::aaguid;

            resp->options.plat = false;
            resp->options.rk = true;
            resp->options.clientPinSupported = true;
            resp->options.clientPin = true;
            resp->options.up = true;
            resp->options.uvSupported = true;
            resp->options.uv = true;

            resp->extensions.push_back("hmac-secret");

            resp->maxMsgSize = 2048;

            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }
    } // namespace Authenticator
} // namespace FIDO2