#pragma once

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace Authenticator
    {
        int processRequest(FIDO2::CTAP::Request *request, FIDO2::CTAP::Response **response);
    }
} // namespace FIDO2