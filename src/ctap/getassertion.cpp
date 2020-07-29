#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        namespace Request
        {
            CommandCode GetAssertion::getCommandCode() const
            {
                return authenticatorGetAssertion;
            }

            Status parseGetAssertion(const CBOR &bor, std::unique_ptr<Command> &request)
            {
                request = std::unique_ptr<GetAssertion>(new GetAssertion());

                return CTAP2_OK;
            }
        } // namespace Request

        namespace Response
        {
            CommandCode GetAssertion::getCommandCode() const
            {
                return authenticatorGetAssertion;
            }

            Status encode(const GetAssertion *response, std::unique_ptr<CBOR> &cbor)
            {
                return CTAP2_OK;
            }

        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2