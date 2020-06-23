#include <Arduino.h>

#include "transport/ble/security.h"

uint32_t SecurityCallbacks::onPassKeyRequest()
{
    Serial.println("PassKey request");

    return 1234;
}

void SecurityCallbacks::onPassKeyNotify(uint32_t pass_key)
{
    Serial.printf("PassKey notify: %d\n", pass_key);
}

bool SecurityCallbacks::onSecurityRequest()
{
    Serial.println("Security Request");

    return true;
}

void SecurityCallbacks::onAuthenticationComplete(esp_ble_auth_cmpl_t)
{
    Serial.println("Authentication Complete");
}

bool SecurityCallbacks::onConfirmPIN(uint32_t pin)
{
    Serial.printf("Confirm PIN: %d", pin);

    return true;
}
