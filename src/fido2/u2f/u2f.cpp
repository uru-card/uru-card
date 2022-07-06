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

        // https://fidoalliance.org/specs/fido-v2.0-id-20180227/fido-client-to-authenticator-protocol-v2.0-id-20180227.html#usb-hid-cbor
        // https://fidoalliance.org/specs/fido-u2f-v1.0-nfc-bt-amendment-20150514/fido-u2f-raw-message-formats.html
        // https://fidoalliance.org/specs/fido-u2f-v1.0-rd-20140209/fido-u2f-usb-framing-of-apdus-v1.0-rd-20140209.pdf
        
        // Request 

        // 0     = CLA reseverved (must be sent to zero from host)
        // 1     = INS U2F command code (0x01 -> registration, 0x02 -> authentication, 0x03 -> get_version)
        // 2     = Request Param 1
        // 3     = Request Param 2
        // 4,5,6 = length of data big endian
        // 7,n   = data

        // Response

        // n,n-2 = data
        // n-1 = Status byte MSB
        // n   = Status byte LSB

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
            // TODO: missing checks
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
            memcpy(clientDataHash, &buffer[7], 32); // challenge

            std::unique_ptr<FIDO2::CTAP::AuthenticatorData> authData = std::unique_ptr<FIDO2::CTAP::AuthenticatorData>(new FIDO2::CTAP::AuthenticatorData());
            authData->flags.val = 0;
            authData->flags.f.userPresent = true;
            authData->flags.f.userVerified = true;
            authData->flags.f.attestationData = true;
            memcpy(&authData->rpIdHash, &buffer[7 + 32], 32); // application

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
            String rpID = String(authData->rpIdHash, 32);
            // String rpID = "webauthn.me";
            if (!CredentialsStorage::findCredential(rpID, getUserID(), &credential)){
                CredentialsStorage::createCredential(rpID, getUserID(), &credential);
            }
            memcpy(&buffer[length], credential->id.value, CREDENTIAL_ID_LENGTH);
            length += CREDENTIAL_ID_LENGTH;

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

            // status SW_NO_ERROR
            buffer[length++] = 0x90;
            buffer[length++] = 0x00;
          
            return length;
        }

        uint16_t processAuthentication(uint8_t* buffer){
            uint16_t length = 0;

            uint8_t clientDataHash[32];
            memcpy(clientDataHash, &buffer[7 + 1], 32); // challenge

            std::unique_ptr<FIDO2::CTAP::AuthenticatorData> authData = std::unique_ptr<FIDO2::CTAP::AuthenticatorData>(new FIDO2::CTAP::AuthenticatorData());
            authData->signCount = 0;
            authData->flags.f.userPresent = true;
            authData->flags.f.userVerified = true;
            authData->flags.f.attestationData = false;
            memcpy(&authData->rpIdHash, &buffer[7 + 1 + 32], 32); // application
            
            String rpID = String(buffer[8], 32);
            CredentialsStorage::Credential* credential = nullptr;

            uint8_t controlByte = buffer[7];
            if(controlByte == 0x03) { // enforce-user-presence-and-sign
                uint8_t keyHandleLength = buffer[7 + 1 + 64];
                uint8_t *keyHandle = &buffer[7 + 1 + 64 + 1];
                FixedBuffer32 keyHandleBuffer;
                keyHandleBuffer.alloc(keyHandleLength);
                memcpy(keyHandleBuffer.value, keyHandle, keyHandleLength);
                // search for credential by credential id (key handle)
                if (CredentialsStorage::getCredential(keyHandleBuffer, &credential)){
                    // status SW_CONDITIONS_NOT_SATISFIED
                    buffer[length++] = 0x69;
                    buffer[length++] = 0x85;
                    return length;
                } else {
                    // status SW_WRONG_DATA
                    buffer[length++] = 0x6a;
                    buffer[length++] = 0x80;
                    return length;
                }
            }

            // 1. user presence
            buffer[length++] = 0x01;

            // 2. counter
            buffer[length++] = ((uint8_t*)&authData->signCount)[0];
            buffer[length++] = ((uint8_t*)&authData->signCount)[1];
            buffer[length++] = ((uint8_t*)&authData->signCount)[2];
            buffer[length++] = ((uint8_t*)&authData->signCount)[3];

            // 3. signature
            size_t signatureLength;
            FIDO2::Authenticator::sign(authData.get(), clientDataHash, &buffer[length], &signatureLength);
            length += signatureLength;

            // status SW_NO_ERROR
            buffer[length++] = 0x90;
            buffer[length++] = 0x00;

            return length;
        }

        uint16_t processGetVersion(uint8_t* buffer){
            uint16_t length = 0;

            // 1. version
            uint8_t version[] = "U2F_V2";
            length += sizeof(version);

            // status SW_NO_ERROR
            buffer[length++] = 0x90;
            buffer[length++] = 0x00;

            return length;
        }

    } // namespace U2F
} // namespace FIDO2

#endif