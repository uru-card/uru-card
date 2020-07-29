#include <memory>

#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        const FIDO2::UUID aaguid("63d9df31-662d-476a-a7a7-53b6aa038975");

        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Command *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            switch (request->getCommandCode())
            {
            case FIDO2::CTAP::authenticatorGetInfo:
                return processRequest(static_cast<const FIDO2::CTAP::Request::GetInfo *>(request), response);
            case FIDO2::CTAP::authenticatorGetAssertion:
                return processRequest((const FIDO2::CTAP::Request::GetAssertion *)request, response);
            case FIDO2::CTAP::authenticatorMakeCredential:
                return processRequest((const FIDO2::CTAP::Request::MakeCredential *)request, response);
            case FIDO2::CTAP::authenticatorClientPIN:
                return processRequest((const FIDO2::CTAP::Request::ClientPIN *)request, response);
            case FIDO2::CTAP::authenticatorReset:
                return processRequest((const FIDO2::CTAP::Request::Reset *)request, response);
            default:
                break;
            }

            return FIDO2::CTAP::CTAP1_ERR_INVALID_COMMAND;
        }

    } // namespace Authenticator
} // namespace FIDO2