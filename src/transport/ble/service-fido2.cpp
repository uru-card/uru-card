#include <Arduino.h>

#include "transport/ble/service-fido2.h"

#define FIDO2_CONTROL_POINT_LENGTH 512

void FIDO2ControlPointCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string value = pCharacteristic->getValue();
    if (value.length() == 0)
    {
        return;
    }

    for (auto i = 0; i < value.length(); i++)
    {
        Serial.printf("%02X", value[i]);
    }

    Serial.println();
}

void FIDO2ControlPointLengthCallbacks::onRead(BLECharacteristic *pCharacteristic)
{
    uint8_t value[] = {(FIDO2_CONTROL_POINT_LENGTH >> 8) & 0xFF, FIDO2_CONTROL_POINT_LENGTH & 0xFF};
    pCharacteristic->setValue(value, sizeof(value));
}

void FIDO2ServiceRevisionCallbacks::onRead(BLECharacteristic *pCharacteristic)
{
    uint8_t value[] = {0x20};
    pCharacteristic->setValue(value, 1);
}

void FIDO2ServiceRevisionCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
}
