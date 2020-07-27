#pragma once

#include "fido2/ctap/ctap.h"

namespace FIDO2
{
    namespace Authenticator
    {
        FIDO2::CTAP::Command *processRequest(FIDO2::CTAP::Command *request);

        FIDO2::CTAP::ResponseGetInfo *processRequest(FIDO2::CTAP::RequestGetInfo *request);
        FIDO2::CTAP::ResponseGetAssertion *processRequest(FIDO2::CTAP::RequestGetAssertion *request);
        FIDO2::CTAP::ResponseMakeCredential *processRequest(FIDO2::CTAP::RequestMakeCredential *request);
        FIDO2::CTAP::ResponseClientPIN *processRequest(FIDO2::CTAP::RequestClientPIN *request);
        FIDO2::CTAP::ResponseReset *processRequest(FIDO2::CTAP::RequestReset *request);
    } // namespace Authenticator
} // namespace FIDO2