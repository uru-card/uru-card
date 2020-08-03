#include <Arduino.h>

#include <SHA256.h>

#include "crypto/crypto.h"

namespace Crypto
{
    namespace SHA256
    {
        void hash(const uint8_t *data, const size_t length, uint8_t *sha)
        {
            ::SHA256 sha256;

            sha256.reset();
            sha256.update(data, length);
            sha256.finalize(sha, 32);
        }
    } // namespace SHA256
} // namespace Crypto