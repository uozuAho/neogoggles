#include <Adafruit_NeoPixel.h>

#include "hardware_config.h"
#include "theme_runner.h"


//--------------------------------------------------------------
// data

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_OUTPUT_PIN,
                                                    NEO_GRB + NEO_KHZ800);

ThemeRunner theme_runner = ThemeRunner(pixels);


//--------------------------------------------------------------
// functions

void setup()
{
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    pixels.begin();
    PixelBuf& px = pixels.getPixelBuf();
    px.setMaxBrightness(20);
}

void loop()
{
    static unsigned long last_time = 0;
    theme_runner.vUpdate(millis());

    if (millis() - last_time > 5000)
    {
        last_time = millis();
        theme_runner.vNextTheme();
    }
}
