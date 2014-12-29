#ifndef THEME_COL_CHANGE_SPINNERS_H
#define THEME_COL_CHANGE_SPINNERS_H

#include "fader.h"
#include "pixel.h"
#include "ring_view.h"
#include "theme.h"
#include "themes/theme.h"

class ThemeColChangeSpinners: public Theme
{
public:
    ThemeColChangeSpinners(Adafruit_NeoPixel& pixels);
    ~ThemeColChangeSpinners();

    void vUpdate(uint32_t time_ms);
    void vStop();
    bool bIsStopped();

private:
    Fader* _fader;
    RingView* _left_eye;
    RingView* _right_eye;

    Spot _left_spot;
    Spot _right_spot;

    int16_t _left_spot_speed;
    int16_t _left_spot_speed_target;
    int16_t _right_spot_speed;
    int16_t _right_spot_speed_target;
    Pixel::ColourType _colour_target_left;
    Pixel::ColourType _colour_target_right;

    void updateSpeedTarget(int16_t& speed, int16_t& target);
    void updateColourTarget(Pixel::ColourType& col, Pixel::ColourType& target);
    void updateSingleColourTarget(uint8_t& val, uint8_t& target);
    void setRandomColour(Pixel::ColourType& col);
};

#endif // THEME_COL_CHANGE_SPINNERS_H

