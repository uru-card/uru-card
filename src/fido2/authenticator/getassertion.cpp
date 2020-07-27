#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::ResponseGetAssertion *processRequest(FIDO2::CTAP::RequestGetAssertion *request)
        {
            FIDO2::CTAP::ResponseGetAssertion *response = new FIDO2::CTAP::ResponseGetAssertion();

            return response;
        }
    } // namespace Authenticator
} // namespace FIDO2