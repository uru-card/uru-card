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
    static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

    enum Mode
    {
        MODE_LOGO = 1,
        MODE_TEXT = 2,
    };

    static Mode mode = MODE_LOGO;

    static bool needsUpdate = false;

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

    static const unsigned char PROGMEM icon_processing[] = {
        B00000000,
        B00000000,
        B10000001,
        B01000010,
        B00111100,
        B11111111,
        B00111100,
        B01000010,
        B10000001,
        B00000000,
        B00000000,
        B00000000,
    };

    bool icons[2] = {};

    String text;

    void init()
    {
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display.clearDisplay();
    }

    void update()
    {
        if (!needsUpdate)
            return;

        display.clearDisplay();

        if (icons[ICON_BLUETOOTH])
        {
            display.drawBitmap(SCREEN_WIDTH - 8, 0, icon_bluetooth, 8, 12, 1);
        }
        if (icons[ICON_PROCESSING])
        {
            display.drawBitmap(SCREEN_WIDTH - 8, 16, icon_processing, 8, 12, 1);
        }

        switch (mode)
        {
        case MODE_LOGO:
            display.setTextSize(2);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor((SCREEN_WIDTH - strlen(DEVICE_NAME) * 6 * 2) / 2, 8);
            display.println(DEVICE_NAME);
            break;
        case MODE_TEXT:
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.println(text);
            break;
        }

        display.display();

        needsUpdate = false;
    }

    void showLogo()
    {
        mode = MODE_LOGO;
        needsUpdate = true;
    }

    void showText(const char *_text)
    {
        mode = MODE_TEXT;
        text = _text;
        needsUpdate = true;
    }

    void enableIcon(int icon)
    {
        icons[icon] = 1;
        needsUpdate = true;
    }

    void disableIcon(int icon)
    {
        icons[icon] = 0;
        needsUpdate = true;
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