#include "fader.h"


void Fader::vUpdate(unsigned long time_ms)
{
    if (time_ms - last_time_ms >= 33)
    {
        uint16_t i = 0;
        last_time_ms = time_ms;
        for (; i < target.num_pixels; i++)
            target.remPixelColor(i, rate_r, rate_g, rate_b);
    }
}
