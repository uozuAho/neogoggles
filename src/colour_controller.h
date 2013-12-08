#ifndef COLOUR_CONTROLLER_H
#define COLOUR_CONTROLLER_H

#include "pixel.h"

class ColourController
{
public:
    ColourController(Pixel::ColourType& target);

    void vUpdate(unsigned long time_ms);

private:
    void vEffect1();

    Pixel::ColourType set_colour1;
    Pixel::ColourType set_colour2;
    Pixel::ColourType& target;
    uint16_t u16_speed;
};

#endif // COLOUR_CONTROLLER_H
