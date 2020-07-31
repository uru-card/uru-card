#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::MakeCredential *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            std::unique_ptr<FIDO2::CTAP::Response::MakeCredential> resp = std::unique_ptr<FIDO2::CTAP::Response::MakeCredential>(new FIDO2::CTAP::Response::MakeCredential());

            Serial.println("--");
            Serial.printf("%d\n", sizeof(FIDO2::CTAP::AuthenticatorData));
            Serial.printf("%d\n", sizeof(FIDO2::CTAP::AuthenticatorDataFlags));
            Serial.printf("%d\n", sizeof(FIDO2::CTAP::AttestedCredentialData));


            // fill authenticator data structure
            memcpy(resp->authenticatorData.attestedCredentialData.aaguid, aaguid.get_bytes(), 16);

            resp->authenticatorData.attestedCredentialData.credentialId[0] = 0x01;
            resp->authenticatorData.attestedCredentialData.credentialId[1] = 0x23;
            resp->authenticatorData.attestedCredentialData.credentialId[2] = 0x45;
            resp->authenticatorData.attestedCredentialData.credentialId[3] = 0x67;

            size_t credentialIdLength = sizeof(resp->authenticatorData.attestedCredentialData.credentialId);
            resp->authenticatorData.attestedCredentialData.credentialIdLen.be.h = ((credentialIdLength >> 8) & 0xFF);
            resp->authenticatorData.attestedCredentialData.credentialIdLen.be.l = (credentialIdLength & 0xFF);

            //

            // flags
            resp->authenticatorData.flags.val = 0;
            resp->authenticatorData.flags.f.userPresent = true;
            resp->authenticatorData.flags.f.userVerified = true;
            resp->authenticatorData.flags.f.attestationData = true;

            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            // return response;
            return FIDO2::CTAP::CTAP2_OK;
        }
    } // namespace Authenticator
} // namespace FIDO2