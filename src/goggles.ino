#include <Adafruit_NeoPixel.h>

#include "background_model.h"
#include "colour_controller.h"
#include "hardware_config.h"
#include "push_button.h"
#include "ring_view.h"
#include "spot_model.h"


//--------------------------------------------------------------
// data

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_OUTPUT_PIN,
                                                    NEO_GRB + NEO_KHZ800);

static RingView left_eye =  RingView(pixels.getPixelBuf(), 16, 31, TOP_LEFT);
static RingView right_eye = RingView(pixels.getPixelBuf(), 0, 15, TOP_RIGHT);

static Background bg;
static ColourController bg_colour_control =
    ColourController(bg.colour, ColourController::Effect_Test);

static bool b_effects_paused = false;


//--------------------------------------------------------------
// functions

static void vToggleEffectPaused()
{
    if (b_effects_paused)
        b_effects_paused = false;
    else
        b_effects_paused = true;
}

void setup()
{
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    pixels.begin();
    PixelBuf& px = pixels.getPixelBuf();
    px.setMaxBrightness(10);
    bg.brightness = 100;

    Button_vRegisterCallback_buttonPressed(vToggleEffectPaused);
}

void loop()
{
    Button_vService();
    if (b_effects_paused == false)
        bg_colour_control.vUpdate(millis());
    left_eye.vRenderBackground(bg, RingView::EXCLUSIVE);
    right_eye.vRenderBackground(bg, RingView::EXCLUSIVE);
    pixels.show();
}
