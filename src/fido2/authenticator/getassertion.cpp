#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

#include "crypto/crypto.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::GetAssertion *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("## GetAssertion");

            std::unique_ptr<FIDO2::CTAP::Response::GetAssertion> resp = std::unique_ptr<FIDO2::CTAP::Response::GetAssertion>(new FIDO2::CTAP::Response::GetAssertion());

            // 1. If authenticator supports clientPin and platform sends a zero length pinUvAuthParam,
            // wait for user touch and then return either CTAP2_ERR_PIN_NOT_SET if pin is not set or
            // CTAP2_ERR_PIN_INVALID if pin has been set.

            // 2. If authenticator supports clientPin and pinUvAuthParam parameter is present and the
            // pinUvAuthProtocol is not supported, return CTAP2_ERR_PIN_AUTH_INVALID error.

            // 3. If the options parameter is present, process all the options. If the option is known
            // but not supported, terminate this procedure and return CTAP2_ERR_UNSUPPORTED_OPTION.
            // If the option is known but not valid for this command, terminate this procedure and
            // return CTAP2_ERR_INVALID_OPTION. Ignore any options that are not understood.
            // Note that because this specification defines normative behaviors for them,
            // all authenticators MUST understand the "rk", "up", and "uv" options.

            // 4. Optionally, if the extensions parameter is present, process any extensions that
            // this authenticator supports. Authenticator extension outputs generated by the
            // authenticator extension processing are returned in the authenticator data.

            // 5. If authenticator is not protected by some form of user verification and platform
            // has set "uv" or pinUvAuthParam to get the user verification, return CTAP2_ERR_UNSUPPORTED_OPTION.

            // 6. If authenticator is protected by some form of user verification: ...

            // 7. Locate all credentials that are eligible for retrieval under the specified criteria:

            // 8. Collect user presence if required: ...

            // 9. If allowList is present: ...

            // 10. If allowlist is not present: ...

            Crypto::SHA256::hash((uint8_t *)request->rpId.c_str(), request->rpId.length(), resp->authenticatorData.rpIdHash);

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