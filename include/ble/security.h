#pragma once

#include <BLESecurity.h>

namespace BLE
{
    class Security : public BLESecurityCallbacks
    {
    public:
        virtual uint32_t onPassKeyRequest();

        virtual void onPassKeyNotify(uint32_t pass_key);

        virtual bool onSecurityRequest();

        virtual void onAuthenticationComplete(esp_ble_auth_cmpl_t);

        virtual bool onConfirmPIN(uint32_t pin);
    };
} // namespace BLE
