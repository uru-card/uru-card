#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::ResponseGetInfo *processRequest(FIDO2::CTAP::RequestGetInfo *request)
        {
            FIDO2::CTAP::ResponseGetInfo *response = new FIDO2::CTAP::ResponseGetInfo();

            response->getVersions().push_back("FIDO_2_0");

            return response;
        }
    } // namespace Authenticator
} // namespace FIDO2