#pragma once

namespace Crypto
{
    namespace SHA256
    {
        void hash(const uint8_t *data, const size_t length, uint8_t *sha);
    }

    namespace ECDSA
    {
        void sign(const uint8_t *data, const size_t length, uint8_t *signature);

        void encodeSignature(const uint8_t *signature, uint8_t *encodedSignature);
    } // namespace ECDSA
} // namespace Crypto