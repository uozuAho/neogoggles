#include <Adafruit_NeoPixel.h>
#include "ring_view.h"
#include "spot_model.h"
#include "background_model.h"
#include "colour_controller.h"

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

static Background bg;
static ColourController bg_colour_control = ColourController(bg.colour);


//--------------------------------------------------------------
// functions

void setup()
{
    pixels.begin();

    PixelBuf& px = pixels.getPixelBuf();
    px.setMaxBrightness(20);

    bg.brightness = 255;
}

void loop()
{
    bg_colour_control.vUpdate(millis());
    left_eye.vRenderBackground(bg, RingView::EXCLUSIVE);
    right_eye.vRenderBackground(bg, RingView::EXCLUSIVE);
    pixels.show();

    delay(33);
}
