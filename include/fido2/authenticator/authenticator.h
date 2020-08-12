#pragma once

#include <memory>

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

        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Command *request, std::unique_ptr<FIDO2::CTAP::Command> &response);

        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::GetInfo *request, std::unique_ptr<FIDO2::CTAP::Command> &response);
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::GetAssertion *request, std::unique_ptr<FIDO2::CTAP::Command> &response);
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::MakeCredential *request, std::unique_ptr<FIDO2::CTAP::Command> &response);
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::ClientPIN *request, std::unique_ptr<FIDO2::CTAP::Command> &response);
        FIDO2::CTAP::Status processRequest(const FIDO2::CTAP::Request::Reset *request, std::unique_ptr<FIDO2::CTAP::Command> &response);

        void sign(const FIDO2::CTAP::AuthenticatorData *authenticatorData, const uint8_t *clientDataHash, uint8_t *signature, size_t *signatureSize);

    } // namespace Authenticator
} // namespace FIDO2