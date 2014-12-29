#ifndef THEME_EPILEPSY_H
#define THEME_EPILEPSY_H

#include "pixel.h"
#include "theme.h"

class ThemeEpilepsy: public Theme
{
public:
    ThemeEpilepsy(Adafruit_NeoPixel& pixels);
    ~ThemeEpilepsy();

    void vUpdate(uint32_t time_ms);
    void vStop();
    bool bIsStopped();

private:
    bool _on;
    uint32_t _last_update_time;
    Pixel::ColourType _colour;
};

#endif // THEME_EPILEPSY_H
