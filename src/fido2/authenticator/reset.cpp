#include <Arduino.h>

#include "cred-storage/storage.h"
#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::Reset *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("## Reset");

            Authenticator::reset();
            CredentialsStorage::reset();

            response = std::unique_ptr<FIDO2::CTAP::Response::Reset>(new FIDO2::CTAP::Response::Reset());

            // return response;
            return FIDO2::CTAP::CTAP2_OK;
        }
    } // namespace Authenticator
} // namespace FIDO2