#include <Arduino.h>

#include "ble/security.h"

namespace BLE
{
    uint32_t Security::onPassKeyRequest()
    {
        Serial.println("PassKey request");

        return 1234;
    }

    void Security::onPassKeyNotify(uint32_t pass_key)
    {
        Serial.printf("PassKey notify: %d\n", pass_key);
    }

    bool Security::onSecurityRequest()
    {
        Serial.println("Security Request");

        return true;
    }

    void Security::onAuthenticationComplete(esp_ble_auth_cmpl_t)
    {
        Serial.println("Authentication Complete");
    }

    bool Security::onConfirmPIN(uint32_t pin)
    {
        Serial.printf("Confirm PIN: %d", pin);

        return true;
    }
} // namespace BLE