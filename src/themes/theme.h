#ifndef THEME_H
#define THEME_H

#include <Adafruit_NeoPixel.h>

/** Theme interface */
class Theme
{
public:
    Theme(Adafruit_NeoPixel& pixels):
        _pixels(pixels),
        _display_buf(pixels.getPixelBuf()),
        _stop_requested(false)
    {}

    virtual ~Theme() {}

    /// Update the theme display
    virtual void vUpdate(uint32_t time_ms) = 0;

    /// Stop the theme - gives time for fade-outs etc.
    virtual void vStop() = 0;

    /// Returns true once the theme has finished
    /// its stop routine (if one exists)
    virtual bool bIsStopped() = 0;

protected:
    Adafruit_NeoPixel& _pixels;
    PixelBuf& _display_buf;
    bool _stop_requested;
};


/// Get a theme.
Theme* Theme_create();

#endif // THEME_H
