#ifndef BACKGROUND_MODEL_H
#define BACKGROUND_MODEL_H

#include "pixel.h"

typedef struct
{
    Pixel::ColourType colour;

    /// Scales the colour brightness
    /// 0 (colour = 0) to 255 (original colour)
    uint8_t brightness;

} Background;

#endif // BACKGROUND_MODEL_H
