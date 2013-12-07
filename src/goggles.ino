#include <Adafruit_NeoPixel.h>
#include "ring_view.h"
#include "spot_model.h"

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

static RingView left_eye =  RingView(pixels.getPixelBuf(), 16, 31, TOP_LEFT);
static RingView right_eye = RingView(pixels.getPixelBuf(), 0, 15, TOP_RIGHT);


//--------------------------------------------------------------
// functions

void setup()
{
    pixels.begin();
}

void loop()
{
    delay(150);
}
