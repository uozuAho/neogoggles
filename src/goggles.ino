#include <Adafruit_NeoPixel.h>

#include "hardware_config.h"
#include "ring_view.h"
#include "spot_model.h"


//--------------------------------------------------------------
// data

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_OUTPUT_PIN,
                                                    NEO_GRB + NEO_KHZ800);

static RingView left_eye =  RingView(pixels.getPixelBuf(), 16, 31, TOP_LEFT);
static RingView right_eye = RingView(pixels.getPixelBuf(), 0, 15, TOP_RIGHT);

Spot spot;


//--------------------------------------------------------------
// functions

static void vUpdateSpot()
{
    static unsigned long last_time = 0;
    if (millis() - last_time > 50)
    {
        last_time = millis();
        spot.u16_pos += (256 * 16);
    }
}

void setup()
{
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    pixels.begin();
    PixelBuf& px = pixels.getPixelBuf();
    px.setMaxBrightness(20);

    // init spot
    spot.u16_pos = 0;
    spot.u16_width = 0;
    spot.colour.u8_r = 20;
}

void loop()
{
    vUpdateSpot();
    left_eye.vRenderSpot(spot, RingView::RenderMode_Add);
    pixels.show();
}
