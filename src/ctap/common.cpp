#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"
#include <memory>

namespace FIDO2
{
    namespace CTAP
    {
        namespace Request
        {
            Status parseRpEntity(const CBOR &cbor, PublicKeyCredentialRpEntity *rp)
            {
                if (!cbor.is_pair())
                {
                    return CTAP1_ERR_INVALID_PARAMETER;
                }

                CBORPair &cborPair = (CBORPair &)cbor;

                //
                CBOR cborId = cborPair.find_by_key("id");
                if (!cborId.is_string())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }
                cborId.get_string(rp->id);

                //
                CBOR cborName = cborPair.find_by_key("name");
                if (!cborName.is_null())
                {
                    if (!cborName.is_string())
                    {
                        return CTAP2_ERR_CBOR_UNEXPECTED_TYPE;
                    }

                    cborName.get_string(rp->name);
                }

                //
                CBOR cborIcon = cborPair.find_by_key("icon");
                if (!cborIcon.is_null())
                {
                    if (!cborIcon.is_string())
                    {
                        return CTAP2_ERR_CBOR_UNEXPECTED_TYPE;
                    }

                    cborIcon.get_string(rp->icon);
                }

                return CTAP2_OK;
            }

            Status parseUserEntity(const CBOR &cbor, PublicKeyCredentialUserEntity *user)
            {
                if (!cbor.is_pair())
                {
                    return CTAP1_ERR_INVALID_PARAMETER;
                }

                CBORPair &cborPair = (CBORPair &)cbor;

                //
                CBOR cborId = cborPair.find_by_key("id");
                if (!cborId.is_bytestring())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }
                cborId.get_bytestring(user->id);

                //
                CBOR cborName = cborPair.find_by_key("name");
                if (!cborName.is_null())
                {
                    if (!cborName.is_string())
                    {
                        return CTAP2_ERR_CBOR_UNEXPECTED_TYPE;
                    }

                    cborName.get_string(user->name);
                }

                //
                CBOR cborDisplayName = cborPair.find_by_key("displayName");
                if (!cborDisplayName.is_null())
                {
                    if (!cborDisplayName.is_string())
                    {
                        return CTAP2_ERR_CBOR_UNEXPECTED_TYPE;
                    }

                    cborDisplayName.get_string(user->displayName);
                }

                //
                CBOR cborIcon = cborPair.find_by_key("icon");
                if (!cborIcon.is_null())
                {
                    if (!cborIcon.is_string())
                    {
                        return CTAP2_ERR_CBOR_UNEXPECTED_TYPE;
                    }

                    cborIcon.get_string(user->icon);
                }

                return CTAP2_OK;
            }
        } // namespace Request

        namespace Response
        {
            void encodePublicKey(Crypto::ECDSA::PublicKey *publicKey, uint8_t *encodedKey)
            {
                // use external buffer?
                CBORPair cborPair;

                // kty: EC2 key type
                cborPair.append(1, 2);

                // alg: ES256 signature algorithm
                cborPair.append(3, -7);

                // crv: P-256 curve
                cborPair.append(-1, 1);

                // x-coordinate as byte string 32 bytes in length
                CBOR cborX;
                cborX.encode(publicKey->x, 32);
                cborPair.append(-2, cborX);

                // y-coordinate as byte string 32 bytes in length
                CBOR cborY;
                cborY.encode(publicKey->y, 32);
                cborPair.append(-3, cborY);

                memcpy(encodedKey, cborPair.to_CBOR(), cborPair.length());
            }
        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2