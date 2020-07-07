#pragma once

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace Authenticator
    {
        int processRequest(FIDO2::CTAP::Request *request, FIDO2::CTAP::Response **response);

        int processRequestGetInfo(FIDO2::CTAP::RequestGetInfo *request, FIDO2::CTAP::ResponseGetInfo **response);
    } // namespace Authenticator
} // namespace FIDO2