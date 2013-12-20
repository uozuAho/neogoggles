#include "theme_test.h"

// -----------------------------------------------------------

ThemeTest::~ThemeTest()
{

}

void ThemeTest::vUpdate(uint32_t time_ms)
{
    if (time_ms - _u32_last_time_ms > 500)
    {
        _u32_last_time_ms = time_ms;
        _u8_tog ^= 1;
        if (_u8_tog)
            _display_buf.setPixelColor(1, 10, 10, 10);
        else
            _display_buf.setPixelColor(1, 0, 0, 0);
        _pixels.show();
    }
}

void ThemeTest::vStop()
{
    _stop_requested = true;
    _display_buf.clear();
    _pixels.show();
}

bool ThemeTest::bIsStopped()
{
    if (_stop_requested)
        return true;
    return false;
}


// -----------------------------------------------------------

ThemeTest2::~ThemeTest2()
{

}

void ThemeTest2::vUpdate(uint32_t time_ms)
{
    if (time_ms - _u32_last_time_ms > 500)
    {
        _u32_last_time_ms = time_ms;
        _u8_tog ^= 1;
        if (_u8_tog)
            _display_buf.setPixelColor(2, 15, 5, 8);
        else
            _display_buf.setPixelColor(2, 0, 0, 0);
        _pixels.show();
    }
}

void ThemeTest2::vStop()
{
    _stop_requested = true;
    _display_buf.clear();
    _pixels.show();
}

bool ThemeTest2::bIsStopped()
{
    if (_stop_requested)
        return true;
    return false;
}
