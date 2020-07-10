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

            response->setAAGUID(FIDO2::UUID("63d9df31-662d-476a-a7a7-53b6aa038975"));

            return response;
        }
    } // namespace Authenticator
} // namespace FIDO2