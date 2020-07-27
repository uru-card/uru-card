#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::ResponseReset *processRequest(FIDO2::CTAP::RequestReset *request)
        {
            FIDO2::CTAP::ResponseReset *response = new FIDO2::CTAP::ResponseReset();

            return response;
        }
    } // namespace Authenticator
} // namespace FIDO2