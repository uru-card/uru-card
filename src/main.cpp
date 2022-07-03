#include <Arduino.h>

#include "esp32-hal-log.h"
#include "config.h"
#include "fido2/authenticator/authenticator.h"
#include "crypto/crypto.h"

#if defined(FIDO2_TRANSPORT_BLE)
#include "ble/device.h"
#include "fido2/transport/ble/service.h"
#elif defined(FIDO2_TRANSPORT_USB)
#include "fido2/transport/usb/service.h"
#endif 

#if defined(HARDWARE_DISPLAY)
#include "display/display.h"
#endif

#if defined(HARDWARE_KEYBOARD)
#include "keyboard/keyboard.h"
#endif

#if defined(WIFI_LOGGER) && defined(WIFI_SSID) && defined(WIFI_PASS)
#define ENABLE_WIFI_LOGGER 1
#endif

#ifdef ENABLE_WIFI_LOGGER
#include <WiFi.h>

const char END[] = "\r\n";
WiFiServer wifiServer(8080);
WiFiClient client;

void connectToWiFi(){
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  for(;;) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.print("Connected with IP address: ");
      Serial.println(WiFi.localIP());
      break;
    }
    delay(500);
    Serial.print(".");
  }
}

void startServerTCP(){
  wifiServer.begin();
}

void sendLogToClientTCP(uint8_t const* buffer, uint16_t bufsize){
  if (!client.connected()) {
      client = wifiServer.available();
      if (client.connected()) {
          client.setNoDelay(true);
      }
  }
  if(client){
      if(client.connected()){
          client.write(buffer, bufsize);
          client.write(END, 2);
      } else {
          client.stop();
      }
  }
}
#endif

void setup()
{
    Serial.begin(115200);

    // Serial.setDebugOutput(true);
    // esp_log_level_set("*", ESP_LOG_DEBUG);

#ifdef LED_BUILTIN
    pinMode(LED_BUILTIN, OUTPUT);
#endif

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

#ifdef ENABLE_WIFI_LOGGER
    connectToWiFi();
    startServerTCP();
#endif

#if defined(FIDO2_TRANSPORT_BLE)
    BLE::init();

    FIDO2::Transport::BLE::Service::init();

    BLE::start();
#elif defined(FIDO2_TRANSPORT_USB)
    FIDO2::Transport::USB::Service::init();
#ifdef ENABLE_WIFI_LOGGER
    FIDO2::Transport::USB::Service::setCallback(&sendLogToClientTCP);
#endif
#endif

    delay(1000);
}

void loop()
{
#if defined(FIDO2_TRANSPORT_USB)
    FIDO2::Transport::USB::Service::flush();
#endif

#if defined(HARDWARE_DISPLAY)
    Display::update();
#endif

#if defined(HARDWARE_KEYBOARD)
    Keyboard::update();
#endif 

    delay(50);
}