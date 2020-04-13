#include <Arduino.h>

#include "esp32-hal-log.h"

#include <BLE2902.h>
#include <BLEDevice.h>

#include "ble/fido2.h"

void setupDeviceInfoService(BLEServer *pServer)
{
    BLEService *pDeviceInfoService = pServer->createService((uint16_t)0x180A);

    // Manufacturer Name String
    pDeviceInfoService
        ->createCharacteristic((uint16_t)0x2A29, BLECharacteristic::PROPERTY_READ)
        ->setValue("URU Card");

    // Model Number String
    pDeviceInfoService
        ->createCharacteristic((uint16_t)0x2A24, BLECharacteristic::PROPERTY_READ)
        ->setValue("URU Card v1");

    // Firmware Revision String
    pDeviceInfoService
        ->createCharacteristic((uint16_t)0x2A26, BLECharacteristic::PROPERTY_READ)
        ->setValue("1.0.0");

    pDeviceInfoService->start();
}

void setupFido2Service(BLEServer *pServer)
{
    BLEService *pFido2Service = pServer->createService((uint16_t)0xFFFD);

    // FIDO Control Point
    pFido2Service
        ->createCharacteristic("F1D0FFF1-DEAA-ECEE-B42F-C9BA7ED623BB", BLECharacteristic::PROPERTY_WRITE)
        ->setCallbacks(new FIDO2ControlPointCallbacks());

    // FIDO Status
    pFido2Service
        ->createCharacteristic("F1D0FFF2-DEAA-ECEE-B42F-C9BA7ED623BB", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY)
        ->addDescriptor(new BLE2902());

    // FIDO Control Point Length
    pFido2Service
        ->createCharacteristic("F1D0FFF3-DEAA-ECEE-B42F-C9BA7ED623BB", BLECharacteristic::PROPERTY_READ)
        ->setCallbacks(new FIDO2ControlPointLengthCallbacks());

    // FIDO Service Revision Bitfield
    pFido2Service
        ->createCharacteristic("F1D0FFF4-DEAA-ECEE-B42F-C9BA7ED623BB", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE)
        ->setCallbacks(new FIDO2ServiceRevisionCallbacks());

    pFido2Service->start();
}

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    esp_log_level_set("*", ESP_LOG_DEBUG);

    BLEDevice::init("URU Card");

    // BLE Server
    BLEServer *pServer = BLEDevice::createServer();

    // Device Info
    setupDeviceInfoService(pServer);

    // FIDO2 Service
    setupFido2Service(pServer);

    // Setup Advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID((uint16_t)0xFFFD);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void loop()
{
    delay(1000);
}