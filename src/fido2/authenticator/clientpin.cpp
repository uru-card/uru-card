#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        const uint8_t maxRetries = 8;

        uint8_t pinIsSet = 0;
        uint8_t pinRetries = maxRetries;
        uint8_t pinUvAuthToken[16] = {};

        // getPINRetries 0x01
        FIDO2::CTAP::Status cmdGetPinRetries(std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("### Get PIN retries");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            //
            resp->pinRetries = std::unique_ptr<uint8_t>(new uint8_t(pinRetries));

            //
            if (pinRetries == 0)
            {
                resp->powerCycleState = std::unique_ptr<bool>(new bool(true));
            }

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // getKeyAgreement	0x02
        FIDO2::CTAP::Status cmdGetKeyAgreement(std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("### Get Key Agreement");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            //
            resp->publicKey = std::unique_ptr<Crypto::ECDSA::PublicKey>(new Crypto::ECDSA::PublicKey());
            Crypto::ECDSA::derivePublicKey(&agreementKey, resp->publicKey.get());

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // setPIN	0x03
        FIDO2::CTAP::Status cmdSetPin(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("### Set PIN");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            // If Authenticator does not receive mandatory parameters for this command, it returns CTAP2_ERR_MISSING_PARAMETER error.

            // If a PIN has already been set, authenticator returns CTAP2_ERR_PIN_AUTH_INVALID error.

            // Authenticator generates "sharedSecret": SHA-256((abG).x) using private key of authenticatorKeyAgreementKey,
            // "a" and public key of platformKeyAgreementKey, "bG".

            // Authenticator verifies pinUvAuthParam by generating LEFT(HMAC-SHA-256(sharedSecret, newPinEnc), 16) and
            // matching against input pinUvAuthParam parameter.

            // Authenticator decrypts newPinEnc using above "sharedSecret" producing newPin and checks newPin length
            // against minimum PIN length of 4 bytes.

            // Authenticator stores LEFT(SHA-256(newPin), 16) on the device, sets the pinRetries counter to maximum count,
            // and returns CTAP2_OK.

            pinIsSet = 1;
            pinRetries = maxRetries;

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // changePIN	0x04
        FIDO2::CTAP::Status cmdChangePin(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("### Change PIN");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            // Authenticator generates "sharedSecret"
            // SHA-256((abG).x) using private key of authenticatorKeyAgreementKey, "a" and public key of platformKeyAgreementKey, "bG".
            // SHA-256 is done over only "x" curve point of "abG"

            // Authenticator verifies pinUvAuthParam by generating LEFT(HMAC-SHA-256(sharedSecret, newPinEnc || pinHashEnc), 16)
            // and matching against input pinUvAuthParam parameter.

            // Authenticator decrements the pinRetries counter by 1.

            // Authenticator decrypts pinHashEnc and verifies against its internal stored LEFT(SHA-256(curPin), 16).

            // Authenticator decrypts newPinEnc using above "sharedSecret" producing newPin and checks newPin length against minimum PIN length of 4 bytes.

            // Authenticator stores LEFT(SHA-256(newPin), 16) on the device.

            // Authenticator generates a new pinToken.

            // resp->pinUvAuthToken = std::unique_ptr<FixedBuffer16>(new FixedBuffer16());

            pinIsSet = 1;
            pinRetries = maxRetries;

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // getPinUvAuthTokenUsingPin	0x05
        FIDO2::CTAP::Status cmdGetPinUvAuthTokenUsingPin(std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("Get PIN UV Auth token using PIN");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            resp->pinUvAuthToken = std::unique_ptr<FixedBuffer16>(new FixedBuffer16());

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // getPinUvAuthTokenUsingUv	0x06
        FIDO2::CTAP::Status cmdGetPinUvAuthTokenUsingUv(std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("Get PIN UV Auth token using UV");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            resp->pinUvAuthToken = std::unique_ptr<FixedBuffer16>(new FixedBuffer16());

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // getUVRetries	0x07
        FIDO2::CTAP::Status cmdGetUVRetries(std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("Get UV retries");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("## ClientPIN");

            switch (request->subCommand)
            {
            case FIDO2::CTAP::Request::ClientPIN::cmdGetPINRetries:
                return cmdGetPinRetries(response);
            case FIDO2::CTAP::Request::ClientPIN::cmdGetKeyAgreement:
                return cmdGetKeyAgreement(response);
            case FIDO2::CTAP::Request::ClientPIN::cmdSetPIN:
                return cmdSetPin(request, response);
            case FIDO2::CTAP::Request::ClientPIN::cmdChangePIN:
                return cmdChangePin(request, response);
            case FIDO2::CTAP::Request::ClientPIN::cmdGetPinUvAuthTokenUsingPin:
                return cmdGetPinUvAuthTokenUsingPin(response);
            case FIDO2::CTAP::Request::ClientPIN::cmdGetPinUvAuthTokenUsingUv:
                return cmdGetPinUvAuthTokenUsingUv(response);
            case FIDO2::CTAP::Request::ClientPIN::cmdGetUVRetries:
                return cmdGetUVRetries(response);
            default:
                break;
            }

            return FIDO2::CTAP::CTAP1_ERR_INVALID_PARAMETER;
        }
    } // namespace Authenticator
} // namespace FIDO2