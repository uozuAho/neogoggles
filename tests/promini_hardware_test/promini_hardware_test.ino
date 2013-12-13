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
int brightness = 0;

// ---------------------------
// functions

void toggleSpinDirection()
{
    if (spin_this_way)
      spin_this_way = false;
    else
      spin_this_way = true;
}

void increaseBrightness()
{
  brightness += 10;
}

// call this in loop() for a spinning LED
// effect. button presses change the 
// spin direction
void spinTestCycle()
{
    static int i = 0;
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

// call this in loop to test brightness. 
// WARNING: gets very bright, no upper limit!
// I reckon 20's the brightest you'd ever want
// to go if you're anywhere near these goggles :)
void brightnessTestCycle()
{
  int i = 0;
  for (; i < 32; i++)
    pixels.setPixelColor(i, brightness, brightness, brightness);
  pixels.show();
}

void setup()
{
    // LED
    pinMode(LED_PIN, OUTPUT);
    // button
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    regCallback_onButtonPressed(increaseBrightness);
    pixels.begin();
}

void loop()
{
    Button_vService();
    brightnessTestCycle();
}
