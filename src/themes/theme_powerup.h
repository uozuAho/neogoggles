#ifndef THEME_POWERUP
#define THEME_POWERUP

#include "fader.h"
#include "themes/theme.h"

class ThemePowerup: public Theme
{
public:
    ThemePowerup(Adafruit_NeoPixel& pixels);
    ~ThemePowerup();

    void vUpdate(uint32_t time_ms);
    void vStop();
    bool bIsStopped();

private:
    enum _stateType
    {
        RANDOM_GEN,
        PULSE,
        FINISH,
    };

    _stateType _state;

    Fader* _fader;
    uint8_t _pixel_generation_rate;
    uint8_t _pixel_generation_rate_increase_period;
    bool _theme_finished;

    uint32_t _last_update_time;
    uint32_t _last_genrate_update_time;

    void _vUpdatePixelGenRate(uint32_t time_ms);
};

#endif // THEME_POWERUP
