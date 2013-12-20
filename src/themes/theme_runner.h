#ifndef THEME_RUNNER_H
#define THEME_RUNNER_H

#include <Adafruit_NeoPixel.h>
#include "theme.h"


class ThemeRunner
{
public:
    ThemeRunner(Adafruit_NeoPixel& pixels):
        _pixels(pixels),
        _theme(NULL),
        _theme_idx(0)
    {
    }

    void vUpdate(unsigned long);
    void vNextTheme(void);

private:
    Adafruit_NeoPixel& _pixels;
    Theme* _theme;
    uint8_t _theme_idx;

    void vSwitchTheme(void);
};

#endif // THEME_RUNNER_H
