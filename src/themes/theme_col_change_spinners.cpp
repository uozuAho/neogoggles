#include "hardware_config.h"
#include "theme_col_change_spinners.h"

#define PIXEL_BRIGHTNESS        20
#define SPEED_MAX               400


ThemeColChangeSpinners::ThemeColChangeSpinners(Adafruit_NeoPixel& pixels) :
    Theme(pixels)
{
    _fader = new Fader(_display_buf, 1, 1, 1);
    _right_eye = new RingView(_display_buf, 0, 16, TOP_RIGHT);
    _left_eye = new RingView(_display_buf, 16, 16, TOP_LEFT);

    _left_spot.u16_pos = 0;
    _left_spot.u16_width = 4096;
    _left_spot_speed = random(-SPEED_MAX, SPEED_MAX);
    _left_spot_speed_target = random(-SPEED_MAX, SPEED_MAX);
    setRandomColour(_left_spot.colour);
    setRandomColour(_colour_target_left);

    _right_spot.u16_pos = 0;
    _right_spot.u16_width = 4096;
    _right_spot_speed = random(-SPEED_MAX, SPEED_MAX);
    _right_spot_speed_target = random(-SPEED_MAX, SPEED_MAX);
    setRandomColour(_right_spot.colour);
    setRandomColour(_colour_target_right);
}

ThemeColChangeSpinners::~ThemeColChangeSpinners()
{
    delete _fader;
    delete _right_eye;
    delete _left_eye;
}

void ThemeColChangeSpinners::vUpdate(uint32_t time_ms)
{
    _fader->vUpdate(time_ms);
    _left_spot.u16_pos += _left_spot_speed;
    _right_spot.u16_pos += _right_spot_speed;
    _left_eye->vRenderSpot(_left_spot, RingView::RenderMode_Replace);
    _right_eye->vRenderSpot(_right_spot, RingView::RenderMode_Replace);

    if (time_ms % 10 == 0)
    {
        updateSpeedTarget(_left_spot_speed, _left_spot_speed_target);
        updateSpeedTarget(_right_spot_speed, _right_spot_speed_target);
    }

    if (time_ms % 200 == 0)
    {
        updateColourTarget(_left_spot.colour, _colour_target_left);
        updateColourTarget(_right_spot.colour, _colour_target_right);
    }

    _pixels.show();
}

void ThemeColChangeSpinners::vStop()
{
    _stop_requested = true;
}

bool ThemeColChangeSpinners::bIsStopped()
{
    return _stop_requested;
}

void ThemeColChangeSpinners::updateSpeedTarget(int16_t& speed, int16_t& target)
{
    if (target > speed)
        speed++;
    else if (target < speed)
        speed--;
    else
        target = random(-SPEED_MAX, SPEED_MAX);
}

void ThemeColChangeSpinners::updateColourTarget(Pixel::ColourType& col, Pixel::ColourType& target)
{
    updateSingleColourTarget(col.u8_r, target.u8_r);
    updateSingleColourTarget(col.u8_g, target.u8_g);
    updateSingleColourTarget(col.u8_b, target.u8_b);
}

void ThemeColChangeSpinners::updateSingleColourTarget(uint8_t& val, uint8_t& target)
{
    if (target > val)
        val++;
    else if (target < val)
        val--;
    else
        target = random(PIXEL_BRIGHTNESS);
}

void ThemeColChangeSpinners::setRandomColour(Pixel::ColourType& col)
{
    col.u8_r = random(PIXEL_BRIGHTNESS);
    col.u8_g = random(PIXEL_BRIGHTNESS);
    col.u8_b = random(PIXEL_BRIGHTNESS);
}