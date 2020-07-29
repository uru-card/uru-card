#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode Request::Reset::getCommandCode() const
        {
            return authenticatorReset;
        }

        Status parseRequestReset(const uint8_t *data, const size_t len, std::unique_ptr<Command> &request)
        {
            request = std::unique_ptr<Request::Reset>(new Request::Reset());

            return CTAP2_OK;
        }

        CommandCode Response::Reset::getCommandCode() const
        {
            return authenticatorReset;
        }

        Status encodeResponse(const Response::Reset *response, uint8_t *data, size_t &len)
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