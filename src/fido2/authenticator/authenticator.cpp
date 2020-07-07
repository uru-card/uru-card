#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        int processRequest(FIDO2::CTAP::Request *request, FIDO2::CTAP::Response **response)
        {
            switch (request->getCommand())
            {
            case FIDO2::CTAP::authenticatorGetInfo:
                return processRequestGetInfo(request, response);
            default:
                // error
                break;
            }

            return 0;
        }

    } // namespace Authenticator
} // namespace FIDO2