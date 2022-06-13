#include <Arduino.h>

#include <memory>
#include <vector>

#include "cred-storage/storage.h"

namespace CredentialsStorage
{
    static std::vector<std::unique_ptr<Credential>> credentials;

    void reset()
    {
        credentials.clear();
    }

    bool getCredential(const FixedBuffer32 &credentialId, Credential **credential)
    {
        for (auto it = credentials.begin(); it != credentials.end(); it++)
        {
            if ((*it)->id == credentialId)
            {
                *credential = (*it).get();
                return true;
            }
        }
        return false;
    }

    bool findCredential(const String &rpId, const FixedBuffer64 &userId, Credential **credential)
    {
        for (auto it = credentials.begin(); it != credentials.end(); it++)
        {
            if ((*it)->rpId == rpId && (*it)->userId == userId)
            {
                *credential = (*it).get();
                return true;
            }
        }
        return false;
    }

    bool createCredential(const String &rpId, const FixedBuffer64 &userId, Credential **credential)
    {
        std::unique_ptr<Credential> newCredential = std::unique_ptr<Credential>(new Credential());

        esp_fill_random(newCredential->id.value, newCredential->id.maxLength);
        newCredential->rpId = rpId;
        newCredential->userId = userId;

        *credential = newCredential.get();
        credentials.push_back(std::unique_ptr<Credential>(newCredential.release()));

        return true;
    }
} // namespace CredentialsStorage