#include "colour_controller.h"


ColourController::ColourController(Pixel::ColourType& target, EffectType effect) :
    target(target),
    effect(effect),
    u16_speed(0xffff - 10)
{

}

void ColourController::vUpdate(unsigned long time_ms)
{
    static unsigned long last_time = 0;
    uint16_t update_period_ms = 0xffff - u16_speed;
    if (!update_period_ms)
        update_period_ms = 1;

    if ((time_ms - last_time) >= update_period_ms)
    {
        last_time = time_ms;
        switch (effect)
        {
        case Effect_Test:
            vEffect1();
            break;
        default:
            break;
        }
    }
}


void ColourController::vEffect1()
{
    static uint8_t current_colour_idx = 0;
    static int8_t rate = 1;
    static uint8_t* colour_target = &target.u8_r;

    if (*colour_target == 20)
        rate = -1;
    else if (*colour_target == 0)
    {
        rate = 1;
        if (current_colour_idx == 0)
        {
            current_colour_idx = 1;
            colour_target = &target.u8_g;
        }
        else if (current_colour_idx == 1)
        {
            current_colour_idx = 2;
            colour_target = &target.u8_b;
        }
        else if (current_colour_idx == 2)
        {
            current_colour_idx = 0;
            colour_target = &target.u8_r;
        }
    }
    *colour_target += rate;
}