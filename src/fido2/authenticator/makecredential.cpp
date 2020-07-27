#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::ResponseMakeCredential *processRequest(FIDO2::CTAP::RequestMakeCredential *request)
        {
            FIDO2::CTAP::ResponseMakeCredential *response = new FIDO2::CTAP::ResponseMakeCredential();

            return response;
        }
    } // namespace Authenticator
} // namespace FIDO2