#include <Arduino.h>

#include "config.h"
#include "crypto/crypto.h"

#if !defined(HARDWARE_CRYPTO)

namespace Crypto
{
    bool init()
    {
        return true;
    }

    bool isConfigured()
    {
        return true;
    }

    void configure()
    {
    }

} // namespace Crypto

#endif