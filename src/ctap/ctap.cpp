#include <Arduino.h>

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace CTAP
    {

        Status parseRequest(const uint8_t *data, const uint16_t len, Request **request)
        {
            switch (data[0])
            {
            case authenticatorGetInfo:
                return parseRequestGetInfo(data, len, request);
            default:
                break;
            }

            return CTAP1_ERR_INVALID_COMMAND;
        }

        uint16_t encodeResponse(Response *response, uint8_t *data, uint16_t len)
        {
            return 0;
        }
    } // namespace CTAP
} // namespace FIDO2