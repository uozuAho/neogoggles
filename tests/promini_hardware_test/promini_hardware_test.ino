#include <Adafruit_NeoPixel.h>

#include "hardware_config.h"
#include "push_button.h"

// ---------------------------
// constants

#define NEOPIX_DATA_PIN     10

// ---------------------------
// data

Adafruit_NeoPixel pixels =
    Adafruit_NeoPixel(32, NEOPIX_DATA_PIN, NEO_GRB + NEO_KHZ800);
    
bool spin_this_way = false;

// ---------------------------
// functions

void toggleSpinDirection()
{
    if (spin_this_way)
      spin_this_way = false;
    else
      spin_this_way = true;
}

void setup()
{
    // LED
    pinMode(LED_PIN, OUTPUT);
    // button
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    regCallback_onButtonPressed(toggleSpinDirection);
    pixels.begin();
}

void loop()
{
    static int i = 0;
    Button_vService();
    // clear current pixel
    pixels.setPixelColor(i, 0, 0, 0);
    
    if (spin_this_way)
      i = (i + 1) % 32;
    else
      i = ((i - 1) + 32) % 32;
    // set next pixel
    pixels.setPixelColor(i, 20, 0, 0);
    pixels.show();
    
    delay(30);
}
