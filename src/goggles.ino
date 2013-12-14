/** Example of using multiple pixel buffers */

#include <Adafruit_NeoPixel.h>

#include "pixel.h"
#include "hardware_config.h"


//--------------------------------------------------------------
// constants

#define BAR_WIDTH  5

//--------------------------------------------------------------
// data

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_OUTPUT_PIN,
                                                    NEO_GRB + NEO_KHZ800);

static PixelBuf& display_buf = pixels.getPixelBuf();

static PixelBuf draw_buf_1 = PixelBuf(NUM_PIXELS);
static PixelBuf draw_buf_2 = PixelBuf(NUM_PIXELS);

//--------------------------------------------------------------
// functions

static void vDrawBar(PixelBuf& buf, uint8_t start, uint8_t len,
                     Pixel::ColourType& col)
{
    if (start < buf.num_pixels)
    {
        uint8_t i = 0;
        for (; i < len; i++)
        {
            buf.setPixelColor(start++, col.u8_r, col.u8_g, col.u8_b);
            if (start == buf.num_pixels)
                start = 0;
        }
    }
}

static void vUpdateBar1()
{
    static unsigned long last_time_ms = 0;
    static uint8_t bar_pos = 0;
    static Pixel::ColourType bar_colour = {20,0,0,0};

    if (millis() - last_time_ms > 200)
    {
        last_time_ms = millis();
        draw_buf_1.clear();
        vDrawBar(draw_buf_1, bar_pos++, BAR_WIDTH, bar_colour);
        if (bar_pos == draw_buf_1.num_pixels)
            bar_pos = 0;
    }
}

static void vUpdateBar2()
{
    static unsigned long last_time_ms = 0;
    static uint8_t bar_pos = 16;
    static Pixel::ColourType bar_colour = {0,0,20,0};

    if (millis() - last_time_ms > 80)
    {
        last_time_ms = millis();
        draw_buf_2.clear();
        vDrawBar(draw_buf_2, bar_pos++, BAR_WIDTH, bar_colour);
        if (bar_pos == draw_buf_2.num_pixels)
            bar_pos = 0;
    }
}


void setup()
{
    pinMode(BUTTON_LOW_DRIVE_PIN, OUTPUT);
    digitalWrite(BUTTON_LOW_DRIVE_PIN, LOW);
    pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);

    pixels.begin();
    display_buf.setMaxBrightness(20);
}

void loop()
{
    vUpdateBar1();
    vUpdateBar2();
    display_buf.clear();
    display_buf.addBuf(draw_buf_1);
    display_buf.addBuf(draw_buf_2);
    pixels.show();
}
