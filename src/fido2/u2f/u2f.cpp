#if defined(FIDO2_TRANSPORT_USB)

#include <Arduino.h>

#include "fido2/u2f/u2f.h"
#include "fido2/ctap/ctap.h"
#include "fido2/authenticator/authenticator.h"
#include "crypto/crypto.h"
#include "cred-storage/storage.h"

namespace FIDO2
{
    namespace U2F
    {

        FixedBuffer64 userID;

        uint16_t processRegistration(uint8_t* buffer);
        uint16_t processAuthentication(uint8_t* buffer);
        uint16_t processGetVersion(uint8_t* buffer);

        FixedBuffer64 getUserID(){
            if(userID.length == 0){
                userID.alloc(4);
                userID.value[0] = 'u';
                userID.value[0] = 's';
                userID.value[0] = 'e';
                userID.value[0] = 'r';
            }
            return userID;
        }

        uint16_t processMSG(uint8_t* buffer){
            switch(buffer[1]){
                case U2F_REGISTRATION: return processRegistration(buffer);
                case U2F_AUTHENTICATION: return processAuthentication(buffer);
                case U2F_GET_VERSION: return processGetVersion(buffer);
                default: return 0;
            }   
        }

        uint16_t processRegistration(uint8_t* buffer){
            uint16_t length = 0;

            uint8_t clientDataHash[32];
            memcpy(clientDataHash, &buffer[7], 32);

            std::unique_ptr<FIDO2::CTAP::AuthenticatorData> authData = std::unique_ptr<FIDO2::CTAP::AuthenticatorData>(new FIDO2::CTAP::AuthenticatorData());
            authData->flags.val = 0;
            authData->flags.f.userPresent = true;
            authData->flags.f.userVerified = true;
            authData->flags.f.attestationData = true;
            memcpy(&authData->rpIdHash, &buffer[7 + 32], 32);

            // 1. first byte
            buffer[length++] = 0x05;

            // 2. public key
            Crypto::ECDSA::PublicKey publicKey;
            Crypto::ECDSA::getPublicKey(&publicKey);
            buffer[length++] = 0x04;
            memcpy(&buffer[length], publicKey.x, 32);
            length += 32;
            memcpy(&buffer[length], publicKey.y, 32);
            length += 32;

            // 3. key handle length
            buffer[length++] = CREDENTIAL_ID_LENGTH;

            // 4. key handle
            CredentialsStorage::Credential* credential = nullptr;
            // String rpID = String(authData->rpIdHash, 32);
            String rpID = "webauthn.me";
            if (!CredentialsStorage::findCredential(rpID, getUserID(), &credential)){
                CredentialsStorage::createCredential(rpID, getUserID(), &credential);
            }
            memcpy(&buffer[length], credential->id.value, CREDENTIAL_ID_LENGTH);
            length += 32;

            // 5. attestation certificate
            memcpy(&buffer[length], FIDO2::Authenticator::certificate, FIDO2::Authenticator::certificateSize);
            length += FIDO2::Authenticator::certificateSize;

            // 6. signature
            authData->attestedCredentialData.credentialIdLen = CREDENTIAL_ID_LENGTH;
            memcpy(authData->attestedCredentialData.credentialId, credential->id.value, CREDENTIAL_ID_LENGTH);
            FIDO2::CTAP::Response::encodePublicKey(&publicKey, authData->attestedCredentialData.publicKey);
            size_t signatureLength;
            FIDO2::Authenticator::sign(authData.get(), clientDataHash, &buffer[length], &signatureLength);
            length += signatureLength;

            // status success
            buffer[length++] = 0x90;
            buffer[length++] = 0x00;
          
            return length;
        }

        uint16_t processAuthentication(uint8_t* buffer){
            return 0;
        }

        uint16_t processGetVersion(uint8_t* buffer){
            return 0;
        }

    } // namespace U2F
} // namespace FIDO2

#endif