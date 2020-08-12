#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

#include <YACL.h>

#include "crypto/crypto.h"

#include "util.h"

namespace FIDO2
{
    namespace Authenticator
    {
        void sign(const FIDO2::CTAP::AuthenticatorData *authenticatorData, const uint8_t *clientDataHash, uint8_t *signature, size_t* signatureSize)
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

        void encodePublicKey(Crypto::ECDSA::PublicKey* publicKey, uint8_t* encodedKey)
        {
            // use external buffer?
            CBORPair cborPair;

            // kty: EC2 key type
            cborPair.append(1, 2);

            // alg: ES256 signature algorithm
            cborPair.append(3, -7);

            // crv: P-256 curve
            cborPair.append(-1, 1);

            // x-coordinate as byte string 32 bytes in length
            CBOR cborX;
            cborX.encode(publicKey->x, 32);
            cborPair.append(-2, cborX);

            // y-coordinate as byte string 32 bytes in length
            CBOR cborY;
            cborY.encode(publicKey->y, 32);
            cborPair.append(-3, cborY);

            memcpy(encodedKey, cborPair.to_CBOR(), cborPair.length());
        }

        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::MakeCredential *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            std::unique_ptr<FIDO2::CTAP::Response::MakeCredential> resp = std::unique_ptr<FIDO2::CTAP::Response::MakeCredential>(new FIDO2::CTAP::Response::MakeCredential());

            //
            Crypto::ECDSA::PublicKey publicKey;
            Crypto::ECDSA::getPublicKey(&publicKey);


            // fill authenticator data structure
            memcpy(resp->authenticatorData.attestedCredentialData.aaguid, aaguid.get_bytes(), 16);

            size_t credentialIdLength = sizeof(resp->authenticatorData.attestedCredentialData.credentialId);
            resp->authenticatorData.attestedCredentialData.credentialIdLen.be.h = ((credentialIdLength >> 8) & 0xFF);
            resp->authenticatorData.attestedCredentialData.credentialIdLen.be.l = (credentialIdLength & 0xFF);

            encodePublicKey(&publicKey, resp->authenticatorData.attestedCredentialData.publicKey);

            //
            Crypto::SHA256::hash((const uint8_t *)request->rp.id.c_str(), request->rp.id.length(), resp->authenticatorData.rpIdHash);

            // flags
            resp->authenticatorData.flags.val = 0;
            resp->authenticatorData.flags.f.userPresent = true;
            resp->authenticatorData.flags.f.userVerified = true;
            resp->authenticatorData.flags.f.attestationData = true;

            // sign
            sign(&resp->authenticatorData, request->clientDataHash, resp->signature, &resp->signatureSize);

            serialDumpBuffer(resp->signature, resp->signatureSize);

            // finalize the response
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            // return response;
            return FIDO2::CTAP::CTAP2_OK;
        }
    } // namespace Authenticator
} // namespace FIDO2