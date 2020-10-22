#pragma once

#define ICON_BLUETOOTH 0

namespace Display
{
    void init();

    void update();

    void showLogo();

    void enableIcon(int icon);

    void disableIcon(int icon);
} // namespace Display