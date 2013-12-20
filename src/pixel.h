#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>

namespace Pixel
{

typedef struct
{
    uint8_t u8_r;
    uint8_t u8_g;
    uint8_t u8_b;
    uint8_t u8_transparency;

} ColourType;


void setRandomColour(ColourType&);

} // end namespace Pixel

#endif // PIXEL_H
