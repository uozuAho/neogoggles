#include <Arduino.h>
#include "pixel.h"

namespace Pixel
{

void setRandomColour(ColourType& colour)
{
    colour.u8_r = random(256);
    colour.u8_g = random(256);
    colour.u8_b = random(256);
}

} // namespace Pixel
