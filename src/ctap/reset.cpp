#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode RequestReset::getCommandCode()
        {
            return authenticatorReset;
        }

        Command *parseRequestReset(const uint8_t *data, const size_t length)
        {
            return new RequestReset();
        }

        CommandCode ResponseReset::getCommandCode()
        {
            return authenticatorReset;
        }

        Status encodeResponse(ResponseReset *response, uint8_t *data, size_t &length)
        {
            // use external buffer?
            CBORPair cbor = CBORPair();

            // finalize the encoding
            data[0] = CTAP2_OK;
            memcpy(data + 1, cbor.to_CBOR(), cbor.length());
            length = cbor.length() + 1;

            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2