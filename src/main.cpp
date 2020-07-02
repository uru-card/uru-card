#include <Arduino.h>
#include <BLEDevice.h>

#include "esp32-hal-log.h"

#include "ble/device.h"
#include "fido2/transport/ble/service.h"

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    esp_log_level_set("*", ESP_LOG_DEBUG);

    BLE::init();

    FIDO2::Transport::BLE::Service::init();

    BLE::start();
}

void loop()
{
    delay(1000);
}