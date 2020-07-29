#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode Request::GetAssertion::getCommandCode() const
        {
            return authenticatorGetAssertion;
        }

        Status parseRequestGetAssertion(const uint8_t *data, const size_t len, std::unique_ptr<Command> &request)
        {
            request = std::unique_ptr<Request::GetAssertion>(new Request::GetAssertion());

            return CTAP2_OK;
        }

        CommandCode Response::GetAssertion::getCommandCode() const
        {
            return authenticatorGetAssertion;
        }

        Status encodeResponse(const Response::GetAssertion *response, uint8_t *data, size_t &len)
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