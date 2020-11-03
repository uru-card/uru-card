#pragma once

namespace Keyboard
{
    void init();

    void update();

    bool waitForTouch(const char key, const unsigned long timeout);
};