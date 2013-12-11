#ifndef COLOUR_CONTROLLER_H
#define COLOUR_CONTROLLER_H

#include "pixel.h"

class ColourController
{
public:
    typedef enum
    {
        Effect_Test,
        Effect_Something

    } EffectType;

    ColourController(Pixel::ColourType& target, EffectType);

    void vUpdate(unsigned long time_ms);

private:
    Pixel::ColourType& target;
    EffectType effect;
    uint16_t u16_speed;

    void vEffect1();
};

#endif // COLOUR_CONTROLLER_H
