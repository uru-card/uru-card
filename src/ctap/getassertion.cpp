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

            Status parseGetAssertion(const CBOR &cbor, std::unique_ptr<Command> &request)
            {
                Serial.println("Parse GetAssertion");

                if (!cbor.is_pair())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }

                CBORPair &cborPair = (CBORPair &)cbor;

                std::unique_ptr<GetAssertion> rq(new GetAssertion());

                request = std::unique_ptr<Command>(rq.release());

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