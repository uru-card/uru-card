#pragma once

namespace Crypto
{
    namespace SHA256
    {
        void hash(const uint8_t *data, const size_t length, uint8_t *sha);
    }

    namespace ECDSA
    {
        struct PublicKey
        {
            uint8_t x[32];
            uint8_t y[32];
        };

        void getPublicKey(PublicKey* publicKey);

        void sign(const uint8_t *hash, uint8_t *signature);

        void encodeSignature(const uint8_t *signature, uint8_t *encodedSignature, size_t* encodedSize);
    } // namespace ECDSA
} // namespace Crypto