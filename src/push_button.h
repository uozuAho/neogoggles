#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

//--------------------------------------------------------------
// types

enum ButtonStateType
{
    BUTTON_DOWN,
    BUTTON_UP
};

/// button event callback function type
typedef void (*buttonCallbackFunc)(void);


//--------------------------------------------------------------
// functions

/// Set the callback function for 'button pressed' event
void Button_vRegisterCallback_buttonPressed(buttonCallbackFunc func);

/// Check button state, run callbacks
void Button_vService(void);


#endif // PUSH_BUTTON_H

