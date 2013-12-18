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

Background bg;

//--------------------------------------------------------------
// functions

void vSetRandomBgColour()
{
    bg.colour.u8_r = random(256);
    bg.colour.u8_g = random(256);
    bg.colour.u8_b = random(256);
}

enum PulseState
{
    BRIGHTEN,
    DARKEN,
    PAUSE
};

void vPulseRandomBgColour()
{
    static unsigned long last_time = 0;

    if (millis() - last_time > 100)
    {
        static PulseState state = BRIGHTEN;
        last_time = millis();

        switch(state)
        {
        case BRIGHTEN:
            bg.brightness += 3;
            if (bg.brightness >= 10)
                state = DARKEN;
            break;
        case DARKEN:
            if (bg.brightness > 2)
                bg.brightness--;
            else
            {
                state = PAUSE;
            }
            break;
        case PAUSE:
        {
            static int pause_counter = 0;
            if (pause_counter++ > 5)
            {
                pause_counter = 0;
                state = BRIGHTEN;
                vSetRandomBgColour();
            }
            break;
        }
        default:
            break;
        }

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

    randomSeed(analogRead(0));
    vSetRandomBgColour();
}

void loop()
{
    vPulseRandomBgColour();
    left_eye.vRenderBackground(bg);
    right_eye.vRenderBackground(bg);
    pixels.show();
}
