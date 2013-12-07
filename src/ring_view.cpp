/*
 * ring_view.cpp
 *
 *  Created on: 23/11/2013
 *      Author: uozu
 */

#include "ring_view.h"


RingView::RingView(PixelBuf& target, uint8_t pixel_start, uint8_t pixel_end,
                   uint8_t pixel_top) :

        pixel_buf(target),
        u8_pixel_start(pixel_start),
        u8_pixel_end(pixel_end),
        u8_pixel_top(pixel_top)
{

}

void RingView::vRenderSpot(Spot& spot, RenderMode render_mode)
{
    uint8_t pos = u8_pixel_start + (spot.u16_pos >> 8) / 16;
    pixel_buf.clear();
    pixel_buf.setPixelColor(pos, spot.colour.u8_r, spot.colour.u8_g,
        spot.colour.u8_b);
}
