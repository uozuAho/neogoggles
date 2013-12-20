#include "theme_runner.h"
#include "theme_test.h"
#include "theme_powerup.h"

#define NUM_THEMES      2


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
        _theme = new ThemePowerup(_pixels);
        break;
    case 1:
        _theme = new ThemeTest(_pixels);
        break;
    default:
        _theme = new ThemeTest(_pixels);
        break;
    }

    if (_theme_idx == NUM_THEMES)
        _theme_idx = 0;
}
