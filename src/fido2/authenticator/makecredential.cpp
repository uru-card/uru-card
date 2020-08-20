#include <Arduino.h>

#include <YACL.h>

#include "fido2/authenticator/authenticator.h"
#include "fido2/ctap/ctap.h"

#include "crypto/crypto.h"

#include "util.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::MakeCredential *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            std::unique_ptr<FIDO2::CTAP::Response::MakeCredential> resp = std::unique_ptr<FIDO2::CTAP::Response::MakeCredential>(new FIDO2::CTAP::Response::MakeCredential());

            if (request->pinUvAuthParam != nullptr && request->pinUvAuthParam->length == 0)
            {
                return FIDO2::CTAP::CTAP2_ERR_PIN_INVALID;
            }

            //
            Crypto::ECDSA::PublicKey publicKey;
            Crypto::ECDSA::getPublicKey(&publicKey);

            // fill authenticator data structure
            memcpy(resp->authenticatorData.attestedCredentialData.aaguid, aaguid.get_bytes(), 16);

            size_t credentialIdLength = sizeof(resp->authenticatorData.attestedCredentialData.credentialId);
            resp->authenticatorData.attestedCredentialData.credentialIdLen = credentialIdLength;

            FIDO2::CTAP::Response::encodePublicKey(&publicKey, resp->authenticatorData.attestedCredentialData.publicKey);

            //
            Crypto::SHA256::hash((const uint8_t *)request->rp.id.c_str(), request->rp.id.length(), resp->authenticatorData.rpIdHash);

            // flags
            resp->authenticatorData.flags.val = 0;
            resp->authenticatorData.flags.f.userPresent = true;
            resp->authenticatorData.flags.f.userVerified = true;
            resp->authenticatorData.flags.f.attestationData = true;

            // sign
            sign(&resp->authenticatorData, request->clientDataHash, resp->signature, &resp->signatureSize);

            // finalize the response
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            // return response;
            return FIDO2::CTAP::CTAP2_OK;
        }
    } // namespace Authenticator
} // namespace FIDO2