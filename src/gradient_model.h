#ifndef GRADIENT_MODEL_H
#define GRADIENT_MODEL_H

#include "pixel.h"

class Gradient
{
private:
    uint16_t u16_start_pos;
    uint16_t u16_end_pos;
    Pixel::ColourType colour_start;
    Pixel::ColourType colour_end;
};


#endif /* GRADIENT_MODEL_H */
