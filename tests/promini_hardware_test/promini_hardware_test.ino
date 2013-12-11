#include "hardware_config.h"
#include "push_button.h"


// ---------------------------
// functions

void toggleLed()
{
    static int tog = 0;
    tog ^= 1;
    if (tog)
        digitalWrite(LED_PIN, HIGH);
    else
        digitalWrite(LED_PIN, LOW);
}

void setup()
{
    // LED
    pinMode(LED_PIN, OUTPUT);
    // button
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    regCallback_onButtonPressed(toggleLed);
}

void loop()
{
    Button_vService();
}
