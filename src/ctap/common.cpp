#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"
#include <memory>

namespace FIDO2
{
    namespace CTAP
    {
        Status parseRpEntity(const CBOR &cbor, PublicKeyCredentialRpEntity *rp)
        {
            return CTAP2_OK;
        }

        Status parseUserEntity(const CBOR &cbor, PublicKeyCredentialUserEntity *user)
        {
            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2