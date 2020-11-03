#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"
#include "fido2/ctap/ctap.h"

#include "crypto/crypto.h"

#include "util/util.h"

namespace FIDO2
{
    namespace Authenticator
    {
        void sign(const FIDO2::CTAP::AuthenticatorData *authenticatorData, const uint8_t *clientDataHash, uint8_t *signature, size_t *signatureSize)
        {
            const size_t AuthDataWithAttSize = sizeof(FIDO2::CTAP::AuthenticatorData);
            const size_t AuthDataNoAttSize = sizeof(FIDO2::CTAP::AuthenticatorData) - sizeof(FIDO2::CTAP::AttestedCredentialData);

            // construct data for signing
            uint8_t buffer[AuthDataWithAttSize + 32];
            size_t bufferSize;
            if (authenticatorData->flags.f.attestationData)
            {
                memcpy(buffer, authenticatorData, AuthDataWithAttSize);
                memcpy(buffer + AuthDataWithAttSize, clientDataHash, 32);

                bufferSize = AuthDataWithAttSize + 32;
            }
            else
            {
                memcpy(buffer, authenticatorData, AuthDataNoAttSize);
                memcpy(buffer + AuthDataNoAttSize, clientDataHash, 32);

                bufferSize = AuthDataNoAttSize + 32;
            }

            //
            uint8_t hash[32];
            Crypto::SHA256::hash(buffer, bufferSize, hash);

            Serial.println("Hash:");
            serialDumpBuffer(hash, 32);

            //
            uint8_t signatureBuf[64];
            Crypto::ECDSA::sign(hash, signatureBuf);

            Serial.println("Signature:");
            serialDumpBuffer(signatureBuf, 64);

            //
            Crypto::ECDSA::encodeSignature(signatureBuf, signature, signatureSize);

            Serial.println("Encoded signature:");
            serialDumpBuffer(signature, *signatureSize);
            Serial.printf("%d\n", *signatureSize);
        }

    }
}