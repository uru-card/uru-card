#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Command *processRequest(FIDO2::CTAP::Command *request)
        {
            switch (request->getCommandCode())
            {
            case FIDO2::CTAP::authenticatorGetInfo:
                return processRequest((FIDO2::CTAP::RequestGetInfo *)request);
            case FIDO2::CTAP::authenticatorGetAssertion:
                return processRequest((FIDO2::CTAP::RequestGetAssertion *)request);
            case FIDO2::CTAP::authenticatorMakeCredential:
                return processRequest((FIDO2::CTAP::RequestMakeCredential *)request);
            case FIDO2::CTAP::authenticatorClientPIN:
                return processRequest((FIDO2::CTAP::RequestClientPIN *)request);
            case FIDO2::CTAP::authenticatorReset:
                return processRequest((FIDO2::CTAP::RequestReset *)request);

            default:
                return new FIDO2::CTAP::CommandError(FIDO2::CTAP::CTAP1_ERR_INVALID_COMMAND);
            }
        }

    } // namespace Authenticator
} // namespace FIDO2