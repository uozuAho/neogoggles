#include <Adafruit_NeoPixel.h>

#define LEDS_PER_EYE                16
#define NUM_LEDS                    LEDS_PER_EYE * 2
#define NEO_OUTPUT_PIN              0

#define TOP_RIGHT                   12
#define TOP_LEFT                    20

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, NEO_OUTPUT_PIN,
                                             NEO_GRB + NEO_KHZ800);

void setup()
{
    pixels.begin();
}

void loop()
{
    PixelBuf* px = pixels.getPixelBuf();
    px->setPixelColor(TOP_RIGHT, 10, 10, 10);
    px->setPixelColor(TOP_LEFT, 10, 10, 10);
    pixels.show();

    delay(500);
}
