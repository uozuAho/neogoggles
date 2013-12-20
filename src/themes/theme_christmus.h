#ifndef THEME_CHRISTMUS_H
#define THEME_CHRISTMUS_H

#include "theme.h"

class ThemeChristmus: public Theme
{
public:
    ThemeChristmus(Adafruit_NeoPixel& pixels);
    ~ThemeChristmus();

    void vUpdate(uint32_t time_ms);
    void vStop();
    bool bIsStopped();

private:
    bool tog;
    uint32_t _last_update_time;
};

#endif // THEME_CHRISTMUS_H
