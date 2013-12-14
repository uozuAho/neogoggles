#ifndef FADER_H
#define FADER_H

#include "pixel_buf.h"

class Fader
{
public:
    Fader(PixelBuf& target, uint8_t rate_r, uint8_t rate_g,
          uint8_t rate_b) :
        target(target),
        last_time_ms(0),
        rate_r(rate_r),
        rate_g(rate_g),
        rate_b(rate_b)
    {}

    void vUpdate(unsigned long time_ms);

private:
    PixelBuf& target;
    unsigned long last_time_ms;
    uint8_t rate_r;
    uint8_t rate_g;
    uint8_t rate_b;
};


#endif // FADER_H
