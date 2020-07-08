#pragma once

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Command *processRequest(FIDO2::CTAP::Command *request);

        FIDO2::CTAP::ResponseGetInfo *processRequest(FIDO2::CTAP::RequestGetInfo *request);
    } // namespace Authenticator
} // namespace FIDO2