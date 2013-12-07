#ifndef RING_VIEW_H
#define RING_VIEW_H

#include <Adafruit_NeoPixel.h>
#include "pixel_buf.h"
#include "spot_model.h"
#include "background_model.h"

class RingView
{
public:
    RingView(PixelBuf& target, uint8_t pixel_start, uint8_t pixel_end,
             uint8_t pixel_top);

    typedef enum
    {
        EXCLUSIVE,
        ADDITIVE,

    } RenderMode;

    // Model renderers
    void vRenderSpot(Spot&, RenderMode);
    void vRenderBackground(Background&, RenderMode);

private:
    PixelBuf& pixel_buf;
    uint8_t u8_pixel_start;
    uint8_t u8_pixel_end;
    uint8_t u8_pixel_top;
};

#endif // RING_VIEW_H
