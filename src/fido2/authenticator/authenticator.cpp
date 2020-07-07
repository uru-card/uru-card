#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Command processRequest(FIDO2::CTAP::Command &request)
        {
            Serial.println(__FUNCTION__);

            switch (request.getCommandCode())
            {
            case FIDO2::CTAP::authenticatorGetInfo:
                return processRequestGetInfo((FIDO2::CTAP::RequestGetInfo &)request);
            default:
                // error
                break;
            }

            return FIDO2::CTAP::CommandError(FIDO2::CTAP::CTAP1_ERR_INVALID_COMMAND);
        }

    } // namespace Authenticator
} // namespace FIDO2