#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

// ---------------------------
// types

enum ButtonStateType
{
    BUTTON_DOWN,
    BUTTON_UP
};

typedef void (*buttonCallbackFunc)(void);


// ---------------------------
// functions

ButtonStateType getButtonState(void);

void regCallback_onButtonPressed(buttonCallbackFunc func);

void Button_vService(void);

#endif // PUSH_BUTTON_H

