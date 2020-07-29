#include <memory>

#include <Arduino.h>

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode Command::getCommandCode() const
        {
            return authenticatorNoCommand;
        }

        Status parseRequest(const uint8_t *data, const size_t len, std::unique_ptr<Command> &request)
        {
            switch (data[0])
            {
            case authenticatorGetInfo:
                return parseRequestGetInfo(data + 1, len - 1, request);
            case authenticatorGetAssertion:
                return parseRequestGetAssertion(data + 1, len - 1, request);
            case authenticatorMakeCredential:
                return parseRequestMakeCredential(data + 1, len - 1, request);
            case authenticatorClientPIN:
                return parseRequestClientPIN(data + 1, len - 1, request);
            case authenticatorReset:
                return parseRequestReset(data + 1, len - 1, request);
            default:
                break;
            }

            return CTAP1_ERR_INVALID_COMMAND;
        }

        Status encodeResponse(const Command *response, uint8_t *data, size_t &len)
        {
            switch (response->getCommandCode())
            {
            case authenticatorGetInfo:
                return encodeResponse((Response::GetInfo *)response, data, len);
            case authenticatorGetAssertion:
                return encodeResponse((Response::GetAssertion *)response, data, len);
            case authenticatorMakeCredential:
                return encodeResponse((Response::MakeCredential *)response, data, len);
            case authenticatorClientPIN:
                return encodeResponse((Request::ClientPIN *)response, data, len);
            case authenticatorReset:
                return encodeResponse((Response::Reset *)response, data, len);
            default:
                break;
            }

            return CTAP1_ERR_INVALID_COMMAND;
        }
    } // namespace CTAP
} // namespace FIDO2