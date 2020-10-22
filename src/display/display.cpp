#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "display/display.h"

#ifdef DISPLAY_ENABLED

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

namespace Display
{
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

    static const unsigned char PROGMEM icon_bluetooth[] = {
        B00100000,
        B00110000,
        B00101000,
        B10100100,
        B01101000,
        B00110000,
        B01101000,
        B10100100,
        B00101000,
        B00110000,
        B00100000,
        B00000000,
    };

    bool icons[1] = {};

    void init()
    {
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display.clearDisplay();
    }

    void update()
    {
        display.clearDisplay();

        if (icons[ICON_BLUETOOTH])
        {
            display.drawBitmap(0, 0, icon_bluetooth, 8, 12, 1);
        }

        display.display();
    }

    void showLogo()
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor((SCREEN_WIDTH - strlen(DEVICE_NAME) * 6 * 2) / 2, 8);
        display.println(DEVICE_NAME);
        display.display();
    }

    void enableIcon(int icon)
    {
        icons[icon] = 1;
    }

    void disableIcon(int icon)
    {
        icons[icon] = 0;
    }

} // namespace Display

#else

namespace Display
{
    void init() {}

    void update() {}

    void showLogo() {}

    void enableIcon(int icon) {}

    void disableIcon(int icon) {}
} // namespace Display

#endif