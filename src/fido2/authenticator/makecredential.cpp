#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::MakeCredential *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            // FIDO2::CTAP::ResponseMakeCredential *response = new FIDO2::CTAP::ResponseMakeCredential();

            // return response;
            return FIDO2::CTAP::CTAP2_OK;
        }
    } // namespace Authenticator
} // namespace FIDO2