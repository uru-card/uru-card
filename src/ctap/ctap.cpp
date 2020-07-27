#include <Arduino.h>

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode Command::getCommandCode()
        {
            return authenticatorNoCommand;
        }

        CommandCode CommandError::getCommandCode()
        {
            return authenticatorError;
        }

        CommandError::CommandError(Status errorCode)
        {
            this->errorCode = errorCode;
        }

        Command *parseRequestGetInfo(const uint8_t *data, const size_t len);
        Command *parseRequestGetAssertion(const uint8_t *data, const size_t len);
        Command *parseRequestMakeCredential(const uint8_t *data, const size_t len);
        Command *parseRequestClientPIN(const uint8_t *data, const size_t len);
        Command *parseRequestReset(const uint8_t *data, const size_t len);

        Command *parseRequest(const uint8_t *data, const size_t len)
        {
            switch (data[0])
            {
            case authenticatorGetInfo:
                return parseRequestGetInfo(data, len);
            case authenticatorGetAssertion:
                return parseRequestGetAssertion(data, len);
            case authenticatorMakeCredential:
                return parseRequestMakeCredential(data, len);
            case authenticatorClientPIN:
                return parseRequestClientPIN(data, len);
            case authenticatorReset:
                return parseRequestReset(data, len);
            default:
                break;
            }

            return new CommandError(CTAP1_ERR_INVALID_COMMAND);
        }

        Status encodeResponse(ResponseGetInfo *response, uint8_t *data, size_t &len);
        Status encodeResponse(ResponseGetAssertion *response, uint8_t *data, size_t &len);
        Status encodeResponse(ResponseMakeCredential *response, uint8_t *data, size_t &len);
        Status encodeResponse(ResponseClientPIN *response, uint8_t *data, size_t &len);
        Status encodeResponse(ResponseReset *response, uint8_t *data, size_t &len);

        Status encodeResponse(Command *response, uint8_t *data, size_t &len)
        {
            switch (response->getCommandCode())
            {
            case authenticatorError:
                return CTAP1_ERR_INVALID_COMMAND;
            case authenticatorGetInfo:
                return encodeResponse((ResponseGetInfo *)response, data, len);
            case authenticatorGetAssertion:
                return encodeResponse((ResponseGetAssertion *)response, data, len);
            case authenticatorMakeCredential:
                return encodeResponse((ResponseMakeCredential *)response, data, len);
            case authenticatorClientPIN:
                return encodeResponse((RequestClientPIN *)response, data, len);
            case authenticatorReset:
                return encodeResponse((ResponseReset *)response, data, len);
            default:
                break;
            }

            return CTAP1_ERR_INVALID_COMMAND;
        }
    } // namespace CTAP
} // namespace FIDO2