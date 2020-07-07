#include <Arduino.h>

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace CTAP
    {
        Status parseRequestGetInfo(const uint8_t *data, const uint16_t length, Request **request)
        {
            // validate

            //
            *request = new RequestGetInfo();

            return CTAP2_OK;
        }

        Status encodeResponseGetInfo()
        {
            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2