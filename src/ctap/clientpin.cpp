#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode RequestClientPIN::getCommandCode()
        {
            return authenticatorClientPIN;
        }

        Command *parseRequestClientPIN(const uint8_t *data, const size_t length)
        {
            return new RequestClientPIN();
        }

        CommandCode ResponseClientPIN::getCommandCode()
        {
            return authenticatorClientPIN;
        }

        Status encodeResponse(ResponseClientPIN *response, uint8_t *data, size_t &length)
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