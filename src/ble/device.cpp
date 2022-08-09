#include <Arduino.h>

#if defined(FIDO2_TRANSPORT_BLE)

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLESecurity.h>

#include "config.h"
#include "ble/security.h"
#include "ble/device.h"
#include "fido2/transport/ble/service.h"
#include "display/display.h"

#define GATT_DEVICE_NAME DEVICE_NAME
#define GATT_MODEL_NUMBER "URU Card v1"
#define GATT_FIRMWARE_REVISION "1.0.0"

namespace BLE
{
    BLEServer *server = nullptr;

    BLEService *pDeviceInfoService = nullptr;

    void init()
    {
        //
        BLEDevice::init(GATT_DEVICE_NAME);

        BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
        BLEDevice::setSecurityCallbacks(new Security());

        // BLE Server
        server = BLEDevice::createServer();
        server->setCallbacks(new Server());

        // Device Info Service
        pDeviceInfoService = server->createService((uint16_t)ESP_GATT_UUID_DEVICE_INFO_SVC);

        // Manufacturer Name String
        pDeviceInfoService
            ->createCharacteristic((uint16_t)ESP_GATT_UUID_MANU_NAME, BLECharacteristic::PROPERTY_READ)
            ->setValue(GATT_DEVICE_NAME);

        // Model Number String
        pDeviceInfoService
            ->createCharacteristic((uint16_t)ESP_GATT_UUID_MODEL_NUMBER_STR, BLECharacteristic::PROPERTY_READ)
            ->setValue(GATT_MODEL_NUMBER);

        // Firmware Revision String
        pDeviceInfoService
            ->createCharacteristic((uint16_t)ESP_GATT_UUID_FW_VERSION_STR, BLECharacteristic::PROPERTY_READ)
            ->setValue(GATT_FIRMWARE_REVISION);

        pDeviceInfoService->start();
    }

    void start()
    {
        // Setup Advertising
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

        pAdvertising->setScanResponse(false);
        pAdvertising->setAppearance(0x00);
        pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x10);
        pAdvertising->addServiceUUID(FIDO2::Transport::BLE::Service::UUID());

        BLEDevice::startAdvertising();
    }

    void Server::onConnect(BLEServer *pServer) {
#if defined(HARDWARE_DISPLAY)
        Display::enableIcon(ICON_BLUETOOTH);
        Display::showText("");
#endif

        BLEDevice::stopAdvertising();
    }

    void Server::onDisconnect(BLEServer *pServer) {
#if defined(HARDWARE_DISPLAY)      
        Display::disableIcon(ICON_BLUETOOTH);
        Display::showLogo();
#endif

        BLEDevice::startAdvertising();
    }

} // namespace BLE

#endif