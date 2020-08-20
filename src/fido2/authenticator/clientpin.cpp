#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        // getPINRetries 0x01
        FIDO2::CTAP::Status cmdGetPinRetries(std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("Get PIN retries");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            //
            resp->pinRetries = std::unique_ptr<uint8_t>(new uint8_t(8));

            //
            resp->powerCycleState = std::unique_ptr<bool>(new bool(false));

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // getKeyAgreement	0x02
        FIDO2::CTAP::Status cmdGetKeyAgreement(std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("Get Key Agreement");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            //
            resp->publicKey = std::unique_ptr<Crypto::ECDSA::PublicKey>(new Crypto::ECDSA::PublicKey());
            Crypto::ECDSA::getPublicKey(resp->publicKey.get());

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // setPIN	0x03
        FIDO2::CTAP::Status cmdSetPin(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("Set PIN");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            resp->pinUvAuthToken = std::unique_ptr<FixedBuffer16>(new FixedBuffer16());

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // changePIN	0x04
        FIDO2::CTAP::Status cmdChangePin(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("Change PIN");

            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            resp->pinUvAuthToken = std::unique_ptr<FixedBuffer16>(new FixedBuffer16());

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
            Serial.println("ClientPIN");

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