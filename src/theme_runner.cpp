#include "theme_runner.h"
#include "theme_test.h"


void ThemeRunner::vUpdate(unsigned long time_ms)
{
    if (_theme)
    {
        if (_theme->bIsStopped())
        {
            vSwitchTheme();
        }
        else
            _theme->vUpdate(time_ms);
    }
    else
        vSwitchTheme();
}

void ThemeRunner::vNextTheme(void)
{
    if (_theme)
        _theme->vStop();
}

void ThemeRunner::vSwitchTheme(void)
{
    if (_theme)
        delete _theme;

    switch(_theme_idx++)
    {
    case 0:
        _theme = new ThemeTest(_pixels);
        break;
    case 1:
        _theme = new ThemeTest2(_pixels);
        break;
    default:
        _theme = new ThemeTest(_pixels);
        break;
    }

    if (_theme_idx == 2)
        _theme_idx = 0;
}
