#include <Arduino.h>

#include <uECC.h>

#include "crypto/crypto.h"
#include "util.h"

namespace Crypto
{
    namespace ECDSA
    {
        static const struct uECC_Curve_t *_es256_curve = uECC_secp256r1();

        uint8_t privateKey[32] ={ 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE,
            0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD,
            0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD, 0xDE, 0xAD };

        void getPublicKey(PublicKey* publicKey)
        {
            uECC_compute_public_key((const uint8_t *)privateKey, (uint8_t *)publicKey, _es256_curve);
        }

        void sign(const uint8_t *hash, uint8_t *signature)
        {
            Serial.println("Private Key:");
            serialDumpBuffer(privateKey, 32);

            uECC_sign(privateKey, hash, 32, signature, _es256_curve);
        }

        void encodeSignature(const uint8_t *signature, uint8_t *encodedSignature, size_t* encodedSize)
        {
            memset(encodedSignature, 0, 72);

            // Need to caress into dumb der format ..
            uint8_t lead_s = 0; // leading zeros
            for (lead_s = 0; lead_s < 32 && signature[32 + lead_s] == 0; lead_s++)
                ;
            int8_t pad_s = ((signature[32 + lead_s] & 0x80) == 0x80);

            uint8_t lead_r = 0;
            for (lead_r = 0; lead_r < 32 && signature[lead_r] == 0; lead_r++)
                ;
            int8_t pad_r = ((signature[0 + lead_r] & 0x80) == 0x80);

            //
            encodedSignature[0] = 0x30;
            encodedSignature[1] = 0x44 + pad_s + pad_r - lead_s - lead_r;

            // R ingredient
            encodedSignature[2] = 0x02;
            encodedSignature[3 + pad_r] = 0;
            encodedSignature[3] = 0x20 + pad_r - lead_r;
            memmove(encodedSignature + 4 + pad_r, signature + lead_r, 32u - lead_r);

            // S ingredient
            encodedSignature[4 + 32 + pad_r - lead_r] = 0x02;
            encodedSignature[5 + 32 + pad_r + pad_s - lead_r] = 0;
            encodedSignature[5 + 32 + pad_r - lead_r] = 0x20 + pad_s - lead_s;
            memmove(encodedSignature + 6 + 32 + pad_r + pad_s - lead_r, signature + 32u + lead_s, 32u - lead_s);

            *encodedSize = 0x46 + pad_s + pad_r - lead_r - lead_s;
        }
    } // namespace ECDSA
} // namespace Crypto