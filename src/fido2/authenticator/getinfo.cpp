#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::ResponseGetInfo processRequestGetInfo(FIDO2::CTAP::RequestGetInfo &request)
        {
            FIDO2::CTAP::ResponseGetInfo response;

            return response;
        }
    } // namespace Authenticator
} // namespace FIDO2