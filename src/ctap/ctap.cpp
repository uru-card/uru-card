#include <memory>

#include <Arduino.h>

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace CTAP
    {
        namespace Request
        {
            Status parse(const uint8_t *data, const size_t len, std::unique_ptr<Command> &request)
            {
                CommandCode commandCode = (CommandCode)data[0];

                CBOR cbor = CBOR((uint8_t *)data + 1, len - 1, true);

                switch (commandCode)
                {
                case authenticatorGetInfo:
                    return parseGetInfo(cbor, request);
                case authenticatorGetAssertion:
                    return parseGetAssertion(cbor, request);
                case authenticatorMakeCredential:
                    return parseMakeCredential(cbor, request);
                case authenticatorClientPIN:
                    return parseClientPIN(cbor, request);
                case authenticatorReset:
                    return parseReset(cbor, request);
                default:
                    break;
                }

                return CTAP1_ERR_INVALID_COMMAND;
            }
        } // namespace Request

        namespace Response
        {
            Status encode(const Command *response, std::unique_ptr<CBOR> &cbor)
            {
                switch (response->getCommandCode())
                {
                case authenticatorGetInfo:
                    return encode((Response::GetInfo *)response, cbor);
                case authenticatorGetAssertion:
                    return encode((Response::GetAssertion *)response, cbor);
                case authenticatorMakeCredential:
                    return encode((Response::MakeCredential *)response, cbor);
                case authenticatorClientPIN:
                    return encode((Request::ClientPIN *)response, cbor);
                case authenticatorReset:
                    return encode((Response::Reset *)response, cbor);
                default:
                    break;
                }

                return CTAP1_ERR_INVALID_COMMAND;
            }
        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2