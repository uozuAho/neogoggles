#ifndef COLOUR_CONTROLLER_H
#define COLOUR_CONTROLLER_H

#include "pixel.h"

class ColourController
{
public:
    ColourController(Pixel::ColourType& target) :
        target(target)
    {}

    virtual void vUpdate(unsigned long time_ms);

protected:
    Pixel::ColourType& target;

};

#endif // COLOUR_CONTROLLER_H
