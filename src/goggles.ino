#include <Adafruit_NeoPixel.h>

#include "hardware_config.h"
#include "push_button.h"
#include "themes/theme_runner.h"


//--------------------------------------------------------------
// data

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_OUTPUT_PIN,
                                                    NEO_GRB + NEO_KHZ800);

ThemeRunner theme_runner = ThemeRunner(pixels);


//--------------------------------------------------------------
// functions

void _nextTheme()
{
    theme_runner.vNextTheme();
}

void setup()
{
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    pixels.begin();
    PixelBuf& px = pixels.getPixelBuf();
    px.setMaxBrightness(20);

    Button_vRegisterCallback_buttonPressed(_nextTheme);

    randomSeed(analogRead(0));
}

void loop()
{
    Button_vService();
    theme_runner.vUpdate(millis());
}
