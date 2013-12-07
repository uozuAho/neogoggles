#ifndef SPOT_MODEL_H
#define SPOT_MODEL_H

#include "pixel.h"

class Spot
{
public:
    Spot(uint16_t pos, uint16_t width, Pixel::ColourType colour);

private:
    uint16_t u16_pos;
    uint16_t u16_width;
    Pixel::ColourType colour;
};

#endif // SPOT_MODEL_H
