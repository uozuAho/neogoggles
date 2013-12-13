#include <Arduino.h>

#include "hardware_config.h"
#include "push_button.h"

//--------------------------------------------------------------
// constants

#define DEBOUNCE_DELAY_MS       25


//--------------------------------------------------------------
// data

/// Current 'button pressed' callback function
buttonCallbackFunc callback_pressed = NULL;


//--------------------------------------------------------------
// private functions

static ButtonStateType getButtonState()
{
    if (digitalRead(BUTTON_INPUT_PIN))
        return BUTTON_UP;
    return BUTTON_DOWN;
}

static void vRunCallback()
{
    if (callback_pressed)
        callback_pressed();
}


//--------------------------------------------------------------
// public functions

void Button_vRegisterCallback_buttonPressed(buttonCallbackFunc func)
{
    callback_pressed = func;
}

void Button_vService()
{
    static ButtonStateType prev_bstate = BUTTON_UP;
    static ButtonStateType prev_debounced_bstate = BUTTON_UP;
    static long start_debounce_timestamp = 0;

    ButtonStateType bstate = getButtonState();

    if (bstate != prev_bstate)
        start_debounce_timestamp = millis();

    if (millis() - start_debounce_timestamp > DEBOUNCE_DELAY_MS)
    {
        if (bstate != prev_debounced_bstate)
        {
            prev_debounced_bstate = bstate;
            if (bstate == BUTTON_DOWN)
                vRunCallback();
        }
    }

    prev_bstate = bstate;
}
