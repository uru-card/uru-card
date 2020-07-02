#pragma once

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