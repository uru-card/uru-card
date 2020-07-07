#include <Arduino.h>

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace CTAP
    {
        int parseRequest(uint8_t *data, uint16_t len, Request **request)
        {
            return 0;
        }

        uint16_t encodeResponse(Response *response, uint8_t *data, uint16_t len)
        {
            return 0;
        }
    } // namespace CTAP
} // namespace FIDO2