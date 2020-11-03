#include <Arduino.h>
#include <BLEDevice.h>

#include "esp32-hal-log.h"

#include "config.h"

#include "fido2/authenticator/authenticator.h"

#include "ble/device.h"
#include "fido2/transport/ble/service.h"

#include "display/display.h"
#include "keyboard/keyboard.h"
#include "crypto/crypto.h"

void setup()
{
    Serial.begin(115200);

    // Serial.setDebugOutput(true);
    // esp_log_level_set("*", ESP_LOG_DEBUG);

    Display::init();
    Display::showLogo();

    if (!Crypto::init())
    {
        Serial.println("Error: could not init crypto library");
        while(true);
    }

    if (!Crypto::isConfigured())
    {
        Crypto::configure();
    }

    Keyboard::init();

    BLE::init();

    FIDO2::Authenticator::powerUp();

    FIDO2::Transport::BLE::Service::init();

    BLE::start();

    delay(1000);
}

void loop()
{
    Display::update();

    Keyboard::update();

    delay(50);
}