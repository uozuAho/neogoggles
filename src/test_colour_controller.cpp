#include "test_colour_controller.h"


void TestColourController::vUpdate(unsigned long time_ms)
{
    if (time_ms - last_time_ms >= 20)
    {
        last_time_ms = time_ms;
        tog ^= 1;
        if (tog)
        {
            target.u8_r = 10;
            target.u8_g = 10;
            target.u8_b = 10;
        }
        else
        {
            target.u8_r = 0;
            target.u8_g = 0;
            target.u8_b = 0;
        }
    }
}
