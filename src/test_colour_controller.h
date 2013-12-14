#ifndef TEST_COLOUR_CONTROLLER_H
#define TEST_COLOUR_CONTROLLER_H

#include "colour_controller.h"

class TestColourController : public ColourController
{
public:
    TestColourController(Pixel::ColourType& target) :
        ColourController(target),
        last_time_ms(0),
        tog(0)
    {}

    void vUpdate(unsigned long time_ms);

private:
    unsigned long last_time_ms;
    int tog;
};


#endif // TEST_COLOUR_CONTROLLER_H
