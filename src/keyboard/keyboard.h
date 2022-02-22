#pragma once

#if defined(HARDWARE_KEYBOARD)

namespace Keyboard
{
    void init();

    void update();

    bool waitForTouch(const char key, const unsigned long timeout);
};

#endif