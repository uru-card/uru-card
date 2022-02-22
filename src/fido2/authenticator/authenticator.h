#pragma once

#include <memory>

#include "crypto/crypto.h"

#include "fido2/ctap/ctap.h"
#include "fido2/uuid.h"

namespace FIDO2
{
    namespace Authenticator
    {
        // AAGUID
        extern const FIDO2::UUID aaguid;

        // Certificate
        extern const uint8_t certificate[];
        extern const size_t certificateSize;

        // Public Key
        extern const uint8_t publicKey[];
        extern const size_t publicKeySize;

        extern Crypto::ECDSA::PrivateKey agreementKey;
        extern uint8_t pinUvAuthToken[16];
        extern uint8_t pinRetries;
        extern uint8_t pinIsSet;

        void powerUp();

        enum Status
        {
            STATUS_IDLE = 0x00,
            STATUS_PROCESSING = 0x01,
            STATUS_WAITING_USER = 0x02,
        };

        void reset();

        uint8_t getStatus();
        void setStatus(Status status);

        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Command *request, std::unique_ptr<FIDO2::CTAP::Command> &response);

        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::GetInfo *request, std::unique_ptr<FIDO2::CTAP::Command> &response);
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::GetAssertion *request, std::unique_ptr<FIDO2::CTAP::Command> &response);
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::MakeCredential *request, std::unique_ptr<FIDO2::CTAP::Command> &response);
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response);
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::Reset *request, std::unique_ptr<FIDO2::CTAP::Command> &response);

        void sign(const FIDO2::CTAP::AuthenticatorData *authenticatorData, const uint8_t *clientDataHash, uint8_t *signature, size_t *signatureSize);

    } // namespace Authenticator
} // namespace FIDO2