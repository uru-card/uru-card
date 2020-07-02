#pragma once

#include <BLEServer.h>

namespace BLE
{
    extern BLEServer *server;

    void init();

    void start();
} // namespace BLE