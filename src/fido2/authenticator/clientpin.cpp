#include <Arduino.h>

#include "fido2/authenticator/authenticator.h"

namespace FIDO2
{
    namespace Authenticator
    {
        // getPINRetries 0x01

        // getKeyAgreement	0x02
        FIDO2::CTAP::Status cmdGetKeyAgreement(std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("getKeyAgreement");
            std::unique_ptr<FIDO2::CTAP::Response::ClientPIN> resp = std::unique_ptr<FIDO2::CTAP::Response::ClientPIN>(new FIDO2::CTAP::Response::ClientPIN());

            //
            resp->publicKey = std::unique_ptr<Crypto::ECDSA::PublicKey>(new Crypto::ECDSA::PublicKey());
            Crypto::ECDSA::getPublicKey(resp->publicKey.get());

            //
            response = std::unique_ptr<FIDO2::CTAP::Command>(resp.release());

            return FIDO2::CTAP::CTAP2_OK;
        }

        // setPIN	0x03

        // changePIN	0x04

        // getPinUvAuthTokenUsingPin	0x05

        // getPinUvAuthTokenUsingUv	0x06

        // getUVRetries	0x07

        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response)
        {
            Serial.println("ClientPIN");

            switch (request->subCommand)
            {
            case FIDO2::CTAP::Request::ClientPIN::cmdGetPINRetries:
                break;
            case FIDO2::CTAP::Request::ClientPIN::cmdGetKeyAgreement:
                return cmdGetKeyAgreement(response);
            case FIDO2::CTAP::Request::ClientPIN::cmdSetPIN:
                break;
            case FIDO2::CTAP::Request::ClientPIN::cmdChangePIN:
                break;
            case FIDO2::CTAP::Request::ClientPIN::cmdGetPinUvAuthTokenUsingPin:
                break;
            case FIDO2::CTAP::Request::ClientPIN::cmdGetPinUvAuthTokenUsingUv:
                break;
            case FIDO2::CTAP::Request::ClientPIN::cmdGetUVRetries:
                break;
            default:
                break;
            }

            return FIDO2::CTAP::CTAP1_ERR_INVALID_PARAMETER;
        }
    } // namespace Authenticator
} // namespace FIDO2