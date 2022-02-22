#pragma once

#define ICON_BLUETOOTH 0
#define ICON_PROCESSING 1

namespace Display
{
    void init();

    void update();

    void showLogo();
    void showText(const char* text);

    void enableIcon(int icon);
    void disableIcon(int icon);
} // namespace Display