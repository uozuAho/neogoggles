#include "ring_view.h"


RingView::RingView(PixelBuf& target, uint16_t pixel_start, uint16_t num_pixels,
                   uint16_t pixel_top) :

        pixel_buf(target),
        u16_pixel_start(pixel_start),
        u16_num_pixels(num_pixels),
        u16_pixel_top(pixel_top)
{
}

void RingView::vRenderSpot(Spot& spot, RenderMode render_mode)
{
    uint8_t pos = u16_pixel_start + (spot.u16_pos >> 8) / 16;
    pixel_buf.clearRange(u16_pixel_start, u16_num_pixels);
    pixel_buf.setPixelColor(pos, spot.colour.u8_r, spot.colour.u8_g,
        spot.colour.u8_b);
}

void RingView::vRenderBackground(Background& bg, RenderMode render_mode)
{
    int i = u16_pixel_start;
    Pixel::ColourType colour = bg.colour;

    pixel_buf.clearRange(u16_pixel_start, u16_num_pixels);

    if (bg.brightness < 0xff)
    {
        colour.u8_r = (((uint16_t)bg.brightness * colour.u8_r) >> 8);
        colour.u8_g = (((uint16_t)bg.brightness * colour.u8_g) >> 8);
        colour.u8_b = (((uint16_t)bg.brightness * colour.u8_b) >> 8);
    }
    for (; i <= u16_num_pixels; i++)
    {
        pixel_buf.setPixelColor(i, colour.u8_r, colour.u8_g, colour.u8_b);
    }
}
