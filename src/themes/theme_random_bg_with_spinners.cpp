#include "hardware_config.h"
#include "theme_random_bg_with_spinners.h"

#define PIXEL_BRIGHTNESS        20


ThemeRandomBgWithSpinners::ThemeRandomBgWithSpinners(Adafruit_NeoPixel& pixels) :
    Theme(pixels)
{
    _fader = new Fader(_display_buf, 1, 1, 1);
    _right_eye = new RingView(_display_buf, 0, 16, TOP_RIGHT);
    _left_eye = new RingView(_display_buf, 16, 16, TOP_LEFT);

    _left_spot.u16_pos = 0;
    _left_spot.u16_width = 4096;
    _left_spot_speed = -30;
    _vSetRandomColour(_left_spot.colour);

    _right_spot.u16_pos = 0;
    _right_spot.u16_width = 4096;
    _right_spot_speed = 40;
    _vSetRandomColour(_right_spot.colour);

    _last_update_time = 0;
    _state = PULSE_ON;
    _vSetRandomColour(_bg_colour);
}

ThemeRandomBgWithSpinners::~ThemeRandomBgWithSpinners()
{
    delete _fader;
    delete _right_eye;
    delete _left_eye;
}

void ThemeRandomBgWithSpinners::vUpdate(uint32_t time_ms)
{
    _fader->vUpdate(time_ms);

    if (_state == PULSE_ON)
    {
        uint16_t i = 0;
        for (; i < _display_buf.num_pixels; i++)
            _display_buf.setPixelColor(i, _bg_colour.u8_r, _bg_colour.u8_g,
                                       _bg_colour.u8_b);
        _state = PULSE_OFF;
        _last_update_time = time_ms;
    }
    else if (_state == PULSE_OFF)
    {
        if (time_ms - _last_update_time > 1500)
        {
            _state = PULSE_ON;
            _vSetRandomColour(_bg_colour);

            if (random(256) < 200)
                _vSetRandomColour(_left_spot.colour);

            if (random(256) < 200)
                _vSetRandomColour(_right_spot.colour);
        }
    }

    _left_spot.u16_pos += _left_spot_speed;
    _right_spot.u16_pos += _right_spot_speed;
    _left_eye->vRenderSpot(_left_spot);
    _right_eye->vRenderSpot(_right_spot);

    _pixels.show();
}

void ThemeRandomBgWithSpinners::vStop()
{
    _stop_requested = true;
}

bool ThemeRandomBgWithSpinners::bIsStopped()
{
    return _stop_requested;
}

void ThemeRandomBgWithSpinners::_vSetRandomColour(Pixel::ColourType& col)
{
    col.u8_r = random(PIXEL_BRIGHTNESS);
    col.u8_g = random(PIXEL_BRIGHTNESS);
    col.u8_b = random(PIXEL_BRIGHTNESS);
}
