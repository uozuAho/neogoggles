#include <Adafruit_NeoPixel.h>

#include "hardware_config.h"
#include "ring_view.h"
#include "spot_model.h"


//--------------------------------------------------------------
// data

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_OUTPUT_PIN,
                                                    NEO_GRB + NEO_KHZ800);

static RingView left_eye =  RingView(pixels.getPixelBuf(), 16, 16, TOP_LEFT);
static RingView right_eye = RingView(pixels.getPixelBuf(), 0, 16, TOP_RIGHT);

Spot spot1;
Spot spot2;


//--------------------------------------------------------------
// functions

static void vUpdateSpot1()
{
    static unsigned long last_time = 0;
    if (millis() - last_time > 400)
    {
        last_time = millis();
        spot1.u16_pos += 4096;
    }
}

static void vUpdateSpot2()
{
    static unsigned long last_time = 0;
    if (millis() - last_time > 50)
    {
        last_time = millis();
        spot2.u16_pos += 4096;
    }
}

void vUpdateSpots()
{
    vUpdateSpot1();
    vUpdateSpot2();
}

void setup()
{
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    pixels.begin();
    PixelBuf& px = pixels.getPixelBuf();
    px.setMaxBrightness(20);

    // init spots
    spot1.u16_pos = 0;
    spot1.u16_width = (0xffff / 4) * 3;
    spot1.colour.u8_r = 20;

    spot2.u16_pos = 1 << 15;
    spot2.u16_width = 4096 * 2;
    spot2.colour.u8_b = 20;
}

void loop()
{
    vUpdateSpots();

    left_eye.vClear();
    left_eye.vRenderSpot(spot1, RingView::RenderMode_Replace);
    left_eye.vRenderSpot(spot2, RingView::RenderMode_Replace);

    right_eye.vClear();
    right_eye.vRenderSpot(spot1, RingView::RenderMode_Add);
    right_eye.vRenderSpot(spot2, RingView::RenderMode_Add);

    pixels.show();
}
