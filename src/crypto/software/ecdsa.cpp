#include <Arduino.h>

#include <uECC.h>

#include "config.h"
#include "crypto/crypto.h"

#include "util/util.h"

#if !defined(HARDWARE_CRYPTO)

namespace Crypto
{
    namespace ECDSA
    {
        static PrivateKey privateKey = {
            .key = {0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD,
                    0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD},
        };

        void getPublicKey(PublicKey *publicKey)
        {
            derivePublicKey(&privateKey, publicKey);
        }

        void sign(const uint8_t *hash, uint8_t *signature)
        {
            Serial.println("Private Key:");
            serialDumpBuffer(privateKey.key, 32);

            uECC_sign(privateKey.key, hash, 32, signature, _es256_curve);
        }
    } // namespace ECDSA
} // namespace Crypto

#endif