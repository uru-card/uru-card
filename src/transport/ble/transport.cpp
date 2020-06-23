#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLESecurity.h>

#include "transport/ble/transport.h"

#include "transport/ble/security.h"
#include "transport/ble/service-fido2.h"

#define GATT_DEVICE_NAME "URU Card"

#define GATT_UUID_FIDO2_SVC 0xFFFD

#define GATT_UUID_FIDO2_CP "F1D0FFF1-DEAA-ECEE-B42F-C9BA7ED623BB"
#define GATT_UUID_FIDO2_STATUS "F1D0FFF2-DEAA-ECEE-B42F-C9BA7ED623BB"
#define GATT_UUID_FIDO2_CP_LEN "F1D0FFF3-DEAA-ECEE-B42F-C9BA7ED623BB"
#define GATT_UUID_FIDO2_REV "F1D0FFF4-DEAA-ECEE-B42F-C9BA7ED623BB"

namespace Transport
{

    void BLE::init()
    {
        //
        BLEDevice::init(GATT_DEVICE_NAME);
        BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
        BLEDevice::setSecurityCallbacks(new SecurityCallbacks());

        // BLE Server
        pServer = BLEDevice::createServer();

        // Device Info
        setupDeviceInfoService();

        // FIDO2 Service
        setupFido2Service();

        // Setup Advertising
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

        pAdvertising->setScanResponse(false);
        pAdvertising->setAppearance(0x00);
        pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x10);
        pAdvertising->addServiceUUID((uint16_t)GATT_UUID_FIDO2_SVC);

        BLEDevice::startAdvertising();
    }

    void BLE::setupDeviceInfoService()
    {
        pDeviceInfoService = pServer->createService((uint16_t)ESP_GATT_UUID_DEVICE_INFO_SVC);

        // Manufacturer Name String
        pDeviceInfoService
            ->createCharacteristic((uint16_t)ESP_GATT_UUID_MANU_NAME, BLECharacteristic::PROPERTY_READ)
            ->setValue("URU Card");

        // Model Number String
        pDeviceInfoService
            ->createCharacteristic((uint16_t)ESP_GATT_UUID_MODEL_NUMBER_STR, BLECharacteristic::PROPERTY_READ)
            ->setValue("URU Card v1");

        // Firmware Revision String
        pDeviceInfoService
            ->createCharacteristic((uint16_t)ESP_GATT_UUID_FW_VERSION_STR, BLECharacteristic::PROPERTY_READ)
            ->setValue("1.0.0");

        pDeviceInfoService->start();
    }

    void BLE::setupFido2Service()
    {
        pFido2Service = pServer->createService((uint16_t)GATT_UUID_FIDO2_SVC);

        // FIDO Control Point
        pFido2Service
            ->createCharacteristic(GATT_UUID_FIDO2_CP, BLECharacteristic::PROPERTY_WRITE)
            ->setCallbacks(new FIDO2ControlPointCallbacks());

        // FIDO Status
        pFido2Service
            ->createCharacteristic(GATT_UUID_FIDO2_STATUS, BLECharacteristic::PROPERTY_NOTIFY)
            ->addDescriptor(new BLE2902());

        // FIDO Control Point Length
        pFido2Service
            ->createCharacteristic(GATT_UUID_FIDO2_CP_LEN, BLECharacteristic::PROPERTY_READ)
            ->setCallbacks(new FIDO2ControlPointLengthCallbacks());

        // FIDO Service Revision Bitfield
        pFido2Service
            ->createCharacteristic(GATT_UUID_FIDO2_REV, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE)
            ->setCallbacks(new FIDO2ServiceRevisionCallbacks());

        pFido2Service->start();
    }

} // namespace Transport