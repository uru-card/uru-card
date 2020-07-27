#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::ResponseClientPIN *processRequest(FIDO2::CTAP::RequestClientPIN *request)
        {
            FIDO2::CTAP::ResponseClientPIN *response = new FIDO2::CTAP::ResponseClientPIN();

            return response;
        }
    } // namespace Authenticator
} // namespace FIDO2