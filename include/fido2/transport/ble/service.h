#pragma once

#include <BLEDevice.h>
#include <BLEUUID.h>

namespace FIDO2
{
    namespace Transport
    {
        namespace BLE
        {
            class ControlPoint : public BLECharacteristicCallbacks
            {
            public:
                static BLEUUID UUID();

                virtual void onWrite(BLECharacteristic *pCharacteristic);
            };

            class ControlPointLength : public BLECharacteristicCallbacks
            {
            public:
                static BLEUUID UUID();

                virtual void onRead(BLECharacteristic *pCharacteristic);
            };

            class Status : public BLECharacteristicCallbacks
            {
            public:
                static BLEUUID UUID();
            };

            class ServiceRevision : public BLECharacteristicCallbacks
            {
            public:
                static BLEUUID UUID();

                virtual void onRead(BLECharacteristic *pCharacteristic);
                virtual void onWrite(BLECharacteristic *pCharacteristic);
            };

            class Service
            {
            public:
                static BLEUUID UUID();

                static void init();

            protected:
                static BLEService *fido2Service;
            };
        } // namespace BLE
    }     // namespace Transport
} // namespace FIDO2