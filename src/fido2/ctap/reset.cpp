#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util/util.h"

namespace FIDO2
{
    namespace CTAP
    {
        namespace Request
        {
            CommandCode Reset::getCommandCode() const
            {
                return authenticatorReset;
            }

            Status parseReset(const CBOR &cbor, std::unique_ptr<Command> &request)
            {
                request = std::unique_ptr<Reset>(new Reset());

                return CTAP2_OK;
            }
        } // namespace Request

        namespace Response
        {
            CommandCode Reset::getCommandCode() const
            {
                return authenticatorReset;
            }

            Status encode(const Reset *response, std::unique_ptr<CBOR> &cbor)
            {
                return CTAP2_OK;
            }

        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2