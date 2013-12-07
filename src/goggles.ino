#include <Adafruit_NeoPixel.h>

//--------------------------------------------------------------
// constants

#define PIXELS_PER_EYE              16
#define NUM_PIXELS                  PIXELS_PER_EYE * 2
#define NEO_OUTPUT_PIN              0

#define TOP_RIGHT                   12
#define TOP_LEFT                    20


//--------------------------------------------------------------
// data

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_OUTPUT_PIN,
                                                    NEO_GRB + NEO_KHZ800);

static PixelBuf* neobuf = pixels.getPixelBuf();


//--------------------------------------------------------------
// functions

void setup()
{
    pixels.begin();
}

void loop()
{
    static int i = 0;

    i = (i + 1) % PIXELS_PER_EYE;

    neobuf->clear();
    neobuf->setPixelColor(i, 10, 10, 10);
    neobuf->setPixelColor(i + PIXELS_PER_EYE, 10, 10, 10);
    pixels.show();

    delay(150);
}
