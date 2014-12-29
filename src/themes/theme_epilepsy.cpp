#include "theme_epilepsy.h"

#define PIXEL_BRIGHTNESS        20


ThemeEpilepsy::ThemeEpilepsy(Adafruit_NeoPixel& pixels) :
    Theme(pixels)
{
    _on = false;
    _last_update_time = 0;
    _colour.u8_r = random(PIXEL_BRIGHTNESS);
    _colour.u8_g = random(PIXEL_BRIGHTNESS);
    _colour.u8_b = random(PIXEL_BRIGHTNESS);
}

ThemeEpilepsy::~ThemeEpilepsy()
{
}

void ThemeEpilepsy::vUpdate(uint32_t time_ms)
{
    if (time_ms - _last_update_time > 40)
    {
        _last_update_time = time_ms;
        _on = !_on;

        int i = 0;
        if (_on)
        {
            for (; i < 32; i++)
                _display_buf.setPixelColor(i, _colour.u8_r, _colour.u8_g, _colour.u8_b);
        }
        else
        {
            for (; i < 32; i++)
                _display_buf.setPixelColor(i, 0, 0, 0);
        }
    }

    if (time_ms % 500 == 0)
    {
        _colour.u8_r = random(PIXEL_BRIGHTNESS);
        _colour.u8_g = random(PIXEL_BRIGHTNESS);
        _colour.u8_b = random(PIXEL_BRIGHTNESS);
    }

    _pixels.show();
}

void ThemeEpilepsy::vStop()
{
    _stop_requested = true;
}

bool ThemeEpilepsy::bIsStopped()
{
    return _stop_requested;
}
