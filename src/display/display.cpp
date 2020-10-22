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

    void init()
    {
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display.clearDisplay();
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

} // namespace Display

#else

namespace Display
{
    void init()
    {
    }

    void showLogo()
    {
    }
} // namespace Display

#endif