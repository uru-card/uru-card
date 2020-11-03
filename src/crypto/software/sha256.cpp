#include <Arduino.h>

#include "config.h"
#include "crypto/crypto.h"

#if !defined(HARDWARE_CRYPTO)

#include <SHA256.h>

namespace Crypto
{
    namespace SHA256
    {
        bool hash(const uint8_t *data, const size_t length, uint8_t *sha)
        {
            ::SHA256 sha256;

            sha256.reset();
            sha256.update(data, length);
            sha256.finalize(sha, 32);

            return true;
        }
    } // namespace SHA256
} // namespace Crypto

#endif