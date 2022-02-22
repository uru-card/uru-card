#pragma once

#include <uECC.h>

#include "config.h"

namespace Crypto
{
    bool init();
    bool isConfigured();
    void configure();

    namespace SHA256
    {
        bool hash(const uint8_t *data, const size_t length, uint8_t *sha);
    }

    namespace ECDSA
    {
        extern const struct uECC_Curve_t *_es256_curve;
        struct PrivateKey
        {
            uint8_t key[32];
        };

        struct PublicKey
        {
            uint8_t x[32];
            uint8_t y[32];
        };

        void getPublicKey(PublicKey *publicKey);

        void sign(const uint8_t *hash, uint8_t *signature);

        void encodeSignature(const uint8_t *signature, uint8_t *encodedSignature, size_t *encodedSize);

        void derivePublicKey(const PrivateKey *privateKey, PublicKey *publicKey);

    } // namespace ECDSA
} // namespace Crypto