#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode Request::ClientPIN::getCommandCode() const
        {
            return authenticatorClientPIN;
        }

        Status parseRequestClientPIN(const uint8_t *data, const size_t len, std::unique_ptr<Command> &request)
        {
            request = std::unique_ptr<Request::ClientPIN>(new Request::ClientPIN());

            return CTAP2_OK;
        }

        CommandCode Response::ClientPIN::getCommandCode() const
        {
            return authenticatorClientPIN;
        }

        Status encodeResponse(const Response::ClientPIN *response, uint8_t *data, size_t &len)
        {
            // use external buffer?
            CBORPair cbor = CBORPair();

            // finalize the encoding
            data[0] = CTAP2_OK;
            memcpy(data + 1, cbor.to_CBOR(), cbor.length());
            len = cbor.length() + 1;

            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2