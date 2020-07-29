#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            // FIDO2::CTAP::ResponseClientPIN *response = new FIDO2::CTAP::ResponseClientPIN();

            return FIDO2::CTAP::CTAP1_ERR_INVALID_COMMAND;
        }
    } // namespace Authenticator
} // namespace FIDO2