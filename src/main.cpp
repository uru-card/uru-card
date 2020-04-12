#include <Arduino.h>

#include <BLEDevice.h>

void setup()
{
    Serial.begin(115200);

    BLEDevice::init("URU Card");

    // BLE Server
    BLEServer *pServer = BLEDevice::createServer();

    // Device Info
    BLEService *pDeviceInfoService = pServer->createService((uint16_t)0x180A);

    // Manufacturer Name String
    pDeviceInfoService->createCharacteristic((uint16_t)0x2A29, BLECharacteristic::PROPERTY_READ)->setValue("URU Card");

    // Model Number String
    pDeviceInfoService->createCharacteristic((uint16_t)0x2A24, BLECharacteristic::PROPERTY_READ)->setValue("");

    // Firmware Revision String
    pDeviceInfoService->createCharacteristic((uint16_t)0x2A26, BLECharacteristic::PROPERTY_READ)->setValue("1.0.0");

    pDeviceInfoService->start();

    // Setup Advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(pDeviceInfoService->getUUID());
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void loop()
{
    delay(1000);
}