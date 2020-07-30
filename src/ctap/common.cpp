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
                if (!cborId.is_string())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }
                cborId.get_string(user->id);

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

        }
    } // namespace CTAP
} // namespace FIDO2