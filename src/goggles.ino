#include <Adafruit_NeoPixel.h>

#include "background_model.h"
#include "fader.h"
#include "hardware_config.h"
#include "push_button.h"
#include "ring_view.h"
#include "spot_model.h"
#include "test_colour_controller.h"


//--------------------------------------------------------------
// data

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_OUTPUT_PIN,
                                                    NEO_GRB + NEO_KHZ800);

static RingView left_eye =  RingView(pixels.getPixelBuf(), 16, 31, TOP_LEFT);
static RingView right_eye = RingView(pixels.getPixelBuf(), 0, 15, TOP_RIGHT);

static Fader fader = Fader(pixels.getPixelBuf(), 1, 1, 1);

static bool b_fader_on = true;

static uint8_t pixel_generation_rate = 20;


//--------------------------------------------------------------
// functions

static void vIncreasePixelGenerationRate()
{
    pixel_generation_rate += 10;
}

static void vToggleFaderOnOff()
{
    if (b_fader_on)
        b_fader_on = false;
    else
        b_fader_on = true;
}

static void vSetRandomPixelOn()
{
    PixelBuf& px = pixels.getPixelBuf();
    uint16_t pixel_num = random(0, NUM_PIXELS);
    px.setPixelColor(pixel_num, 20, 20, 20);
}

static void vRandomPixelGenerator()
{
    static unsigned long last_time_ms = 0;

    if (millis() - last_time_ms > 10)
    {
        last_time_ms = millis();
        if (random(256) < pixel_generation_rate)
        {
            vSetRandomPixelOn();
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

    Button_vRegisterCallback_buttonPressed(vIncreasePixelGenerationRate);
    randomSeed(analogRead(0));
}

void loop()
{
    Button_vService();
    vRandomPixelGenerator();
    if (b_fader_on)
        fader.vUpdate(millis());
    pixels.show();
}
