#ifndef GRADIENT_MODEL_H
#define GRADIENT_MODEL_H

#include "pixel.h"

typedef struct
{
    uint16_t u16_start_pos;
    uint16_t u16_end_pos;
    Pixel::ColourType colour_start;
    Pixel::ColourType colour_end;

} Gradient;


#endif /* GRADIENT_MODEL_H */
