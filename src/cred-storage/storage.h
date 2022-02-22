#pragma once

#include "util/fixedbuffer.h"

namespace CredentialsStorage
{
    struct Credential
    {
        FixedBuffer32 id;
        String rpId;
        FixedBuffer64 userId;
    };

    void reset();

    bool getCredential(const FixedBuffer32 &credentialId, Credential **credential);

    bool findCredential(const String &rpId, const FixedBuffer64 &userId, Credential **credential);

    bool createCredential(const String &rpId, const FixedBuffer64 &userId, Credential **credential);

    // bool storeCredential(const String &rpId, const FixedBuffer64 &userId);
} // namespace CredentialsStorage