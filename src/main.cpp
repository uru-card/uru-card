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

#if defined(HARDWARE_DISPLAY)
    Display::init();
    Display::showLogo();
#endif

    if (!Crypto::init())
    {
        Serial.println("Error: could not init crypto library");
        while(true);
    }

    if (!Crypto::isConfigured())
    {
        Crypto::configure();
    }

#if defined(HARDWARE_KEYBOARD)
    Keyboard::init();
#endif

    FIDO2::Authenticator::powerUp();

#if defined(FIDO2_TRANSPORT_BLE)
    BLE::init();

    FIDO2::Transport::BLE::Service::init();

    BLE::start();
#endif

    delay(1000);
}

void loop()
{
#if defined(HARDWARE_DISPLAY)
    Display::update();
#endif

#if defined(HARDWARE_KEYBOARD)
    Keyboard::update();
#endif 

    delay(50);
}