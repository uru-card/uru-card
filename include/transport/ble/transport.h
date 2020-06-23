#pragma once

#include <BLEDevice.h>

#include "transport/transport.h"

namespace Transport
{
    class BLE
    {
    public:
        void init();

    protected:
        void setupDeviceInfoService();
        void setupFido2Service();

    protected:
        BLEServer *pServer;

        BLEService *pDeviceInfoService;
        BLEService *pFido2Service;
    };
} // namespace Transport