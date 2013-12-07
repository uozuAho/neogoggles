#ifndef SPOT_MODEL_H
#define SPOT_MODEL_H

#include "pixel.h"

typedef struct
{
    uint16_t u16_pos;
    uint16_t u16_width;
    Pixel::ColourType colour;

} Spot;

#endif // SPOT_MODEL_H
