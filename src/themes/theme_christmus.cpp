#include "theme_christmus.h"


ThemeChristmus::ThemeChristmus(Adafruit_NeoPixel& pixels) :
    Theme(pixels)
{
    _last_update_time = 0;
    tog = false;
}

ThemeChristmus::~ThemeChristmus()
{
}

void ThemeChristmus::vUpdate(uint32_t time_ms)
{
    if (time_ms - _last_update_time > 750)
    {
        _last_update_time = time_ms;
        tog = !tog;

        int i = 0;
        if (tog)
        {
            for (; i < 16; i++)
                _display_buf.setPixelColor(i, 10, 0, 0);
            for (; i < 32; i++)
                _display_buf.setPixelColor(i, 0, 10, 0);
        }
        else
        {
            for (; i < 16; i++)
                _display_buf.setPixelColor(i, 0, 10, 0);
            for (; i < 32; i++)
                _display_buf.setPixelColor(i, 10, 0, 0);
        }
    }
    _pixels.show();
}

void ThemeChristmus::vStop()
{
    _stop_requested = true;
}

bool ThemeChristmus::bIsStopped()
{
    return _stop_requested;
}
