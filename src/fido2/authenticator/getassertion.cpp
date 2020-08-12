#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

#include "crypto/crypto.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::GetAssertion *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            std::unique_ptr<FIDO2::CTAP::Response::GetAssertion> resp = std::unique_ptr<FIDO2::CTAP::Response::GetAssertion>(new FIDO2::CTAP::Response::GetAssertion());

            Crypto::SHA256::hash((uint8_t*)request->rpId.c_str(), request->rpId.length(), resp->authenticatorData.rpIdHash);

            resp->authenticatorData.signCount = 0;

            resp->authenticatorData.flags.f.userPresent = true;
            resp->authenticatorData.flags.f.userVerified = true;

            // sign
            sign(&resp->authenticatorData, request->clientDataHash, resp->signature, &resp->signatureSize);

            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            // return response;
            return FIDO2::CTAP::CTAP2_OK;
        }
    } // namespace Authenticator
} // namespace FIDO2