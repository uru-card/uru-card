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

        Command *parseRequest(const uint8_t *data, const size_t len)
        {
            switch (data[0])
            {
            case authenticatorGetInfo:
                return parseRequestGetInfo(data, len);
            default:
                break;
            }

            return new CommandError(CTAP1_ERR_INVALID_COMMAND);
        }

        Status encodeResponse(ResponseGetInfo *response, uint8_t *data, size_t &len);

        Status encodeResponse(Command *response, uint8_t *data, size_t &len)
        {
            switch (response->getCommandCode())
            {
            case authenticatorError:
                return CTAP1_ERR_INVALID_COMMAND;
            case authenticatorGetInfo:
                return encodeResponse((ResponseGetInfo *)response, data, len);
            default:
                break;
            }

            return CTAP1_ERR_INVALID_COMMAND;
        }
    } // namespace CTAP
} // namespace FIDO2