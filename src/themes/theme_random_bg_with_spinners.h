#ifndef THEME_RANDOM_BG_WITH_SPINNERS_H
#define THEME_RANDOM_BG_WITH_SPINNERS_H

#include "fader.h"
#include "pixel.h"
#include "ring_view.h"
#include "theme.h"
#include "themes/theme.h"

class ThemeRandomBgWithSpinners: public Theme
{
public:
    ThemeRandomBgWithSpinners(Adafruit_NeoPixel& pixels);
    ~ThemeRandomBgWithSpinners();

    void vUpdate(uint32_t time_ms);
    void vStop();
    bool bIsStopped();

private:
    enum _stateType
    {
        PULSE_ON,
        PULSE_OFF
    };

    _stateType _state;

    Fader* _fader;
    RingView* _left_eye;
    RingView* _right_eye;

    Spot _left_spot;
    Spot _right_spot;

    int16_t _left_spot_speed;
    int16_t _right_spot_speed;

    uint32_t _last_update_time;
    Pixel::ColourType _bg_colour;

    void _vSetRandomColour(Pixel::ColourType&);
};

#endif // THEME_RANDOM_BG_WITH_SPINNERS_H
