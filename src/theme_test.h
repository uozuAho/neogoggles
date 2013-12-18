#ifndef THEME_TEST_H
#define THEME_TEST_H

#include "theme.h"

class ThemeTest: public Theme
{
public:
    ThemeTest(Adafruit_NeoPixel& pixels) :
        Theme(pixels),
        _u8_tog(0)
    {}

    ~ThemeTest();

    void vUpdate(uint32_t time_ms);
    void vStop();
    bool bIsStopped();

private:
    uint8_t _u8_tog;
};


// similar to above, just want to test that theme switching works
class ThemeTest2: public Theme
{
public:
    ThemeTest2(Adafruit_NeoPixel& pixels) :
        Theme(pixels),
        _u8_tog(0)
    {}

    ~ThemeTest2();

    void vUpdate(uint32_t time_ms);
    void vStop();
    bool bIsStopped();

private:
    uint8_t _u8_tog;
};

#endif // THEME_TEST_H
