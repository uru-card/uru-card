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
            CommandCode ClientPIN::getCommandCode() const
            {
                return authenticatorClientPIN;
            }

            Status parseClientPIN(const CBOR &cbor, std::unique_ptr<Command> &request)
            {
                request = std::unique_ptr<Request::ClientPIN>(new Request::ClientPIN());

                return CTAP2_OK;
            }

        } // namespace Request

        namespace Response
        {
            CommandCode ClientPIN::getCommandCode() const
            {
                return authenticatorClientPIN;
            }

            Status encode(const ClientPIN *response, std::unique_ptr<CBOR> &cbor)
            {

                return CTAP2_OK;
            }
        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2