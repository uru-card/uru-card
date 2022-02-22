#include <Arduino.h>

#if defined(HARDWARE_KEYBOARD)

#include <Adafruit_MPR121.h>

#include "keyboard/keyboard.h"

namespace Keyboard
{
    static Adafruit_MPR121 touch = Adafruit_MPR121();

    static const char keystrokes[13] = {'\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'C', '0', '\n'};

    void init()
    {
        if (!touch.begin(0x5A))
        {
            Serial.println("! MPR121 not found");
        }
    }

    static char getTouched()
    {
        static uint16_t lasttouched = 0;
        uint16_t currtouched = touch.touched();
        int index = 0;

        for (auto i = 0; i < 12; i++)
        {
            // it if *is* touched and *wasnt* touched before
            if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)))
            {
                index = i + 1;
                break;
            }
        }

        // reset our state
        lasttouched = currtouched;

        return keystrokes[index];
    }

    void update()
    {
    }

    bool waitForTouch(const char key, const unsigned long timeout)
    {
        unsigned long start = millis();

        // do
        // {
        //     // char touched = getTouched();
        //     // if (touched == key)
        //     // {
        //     //     return true;
        //     // }
        //     yield();
        //     delay(100);
        // } while (millis() < start + timeout);

        // return false;
        return true;
    }

} // namespace Keyboard

#endif