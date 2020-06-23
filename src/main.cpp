#include <Arduino.h>

#include "esp32-hal-log.h"

#include "transport/ble/transport.h"

Transport::BLE bleTransport;

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    esp_log_level_set("*", ESP_LOG_DEBUG);

    bleTransport.init();
}

void loop()
{
    delay(1000);
}