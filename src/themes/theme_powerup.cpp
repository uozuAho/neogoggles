#include "theme_powerup.h"

#define PIXEL_BRIGHTNESS                    20


ThemePowerup::ThemePowerup(Adafruit_NeoPixel& pixels) :
    Theme(pixels)
{
    _state = RANDOM_GEN;
    _fader = new Fader(_display_buf, 1, 1, 1);
    _pixel_generation_rate = 1;
    _pixel_generation_rate_increase_period = 128;
    _theme_finished = false;
    _last_update_time = 0;
    _last_genrate_update_time = 0;
}

ThemePowerup::~ThemePowerup()
{
    delete _fader;
}

void ThemePowerup::vUpdate(uint32_t time_ms)
{
    _fader->vUpdate(time_ms);

    switch (_state)
    {
    case RANDOM_GEN:
        _vUpdatePixelGenRate(time_ms);
        if (_pixel_generation_rate == 255)
            _state = PULSE;

        if (time_ms - _last_update_time > 10)
        {
            _last_update_time = time_ms;
            if (random(256) < _pixel_generation_rate)
            {
                uint16_t pixel_num = random(0, _display_buf.num_pixels);
                _display_buf.setPixelColor(pixel_num, PIXEL_BRIGHTNESS,
                                           PIXEL_BRIGHTNESS, PIXEL_BRIGHTNESS);
            }
        }
        break;

    case PULSE:
    {
        int i = 0;
        for (; i < _display_buf.num_pixels; i++)
            _display_buf.setPixelColor(i, PIXEL_BRIGHTNESS,
                                       PIXEL_BRIGHTNESS, PIXEL_BRIGHTNESS);
        _state = FINISH;
        break;
    }

    case FINISH:
        if (time_ms - _last_update_time > 2000)
            _theme_finished = true;
        break;

    default:
        break;
    }

    _pixels.show();
}

void ThemePowerup::vStop()
{
    // ignore - this theme decides when it's done
}

bool ThemePowerup::bIsStopped()
{
    return _theme_finished;
}

void ThemePowerup::_vUpdatePixelGenRate(uint32_t time_ms)
{
    if (time_ms - _last_genrate_update_time > _pixel_generation_rate_increase_period)
    {
        _last_genrate_update_time = time_ms;

        if (_pixel_generation_rate_increase_period)
            _pixel_generation_rate_increase_period--;

        if (_pixel_generation_rate < 255)
            _pixel_generation_rate++;
    }

}