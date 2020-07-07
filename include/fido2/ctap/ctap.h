#pragma once

namespace FIDO2
{
    namespace CTAP
    {

        class Request
        {
        };

        class RequestGetInfo : public Request
        {
        };

        int parseRequest(uint8_t *data, uint16_t length, Request **request);

        class Response
        {
        };

        class ResponseGetInfo : public Response
        {
        };

        uint16_t encodeResponse(Response *response, uint8_t *data, uint16_t length);
    } // namespace CTAP
} // namespace FIDO2