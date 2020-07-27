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
            protected:
                static const uint8_t CMD_PING = 0x81;
                static const uint8_t CMD_KEEPALIVE = 0x82;
                static const uint8_t CMD_MSG = 0x83;
                static const uint8_t CMD_CANCEL = 0xbe;
                static const uint8_t CMD_ERROR = 0xbf;

            public:
                static BLEUUID UUID();

                virtual void onWrite(BLECharacteristic *pCharacteristic);

                void processRequest();

                void processMessage();

            protected:
                void sendResponse();

                void sendError(uint8_t errorCode);
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