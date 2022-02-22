#pragma once

#if defined(FIDO2_TRANSPORT_BLE)

#include <BLEServer.h>

namespace BLE
{
    extern BLEServer *server;

    void init();

    void start();

    class Server : public BLEServerCallbacks
    {
        void onConnect(BLEServer *pServer);
        void onDisconnect(BLEServer *pServer);
    };
} // namespace BLE

#endif