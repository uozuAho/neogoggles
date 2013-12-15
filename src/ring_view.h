#ifndef RING_VIEW_H
#define RING_VIEW_H

#include <Adafruit_NeoPixel.h>
#include "pixel_buf.h"
#include "spot_model.h"
#include "background_model.h"

class RingView
{
public:
    RingView(PixelBuf& target, uint16_t pixel_start, uint16_t num_pixels,
             uint16_t pixel_top);

    typedef enum
    {
        RenderMode_Replace,
        RenderMode_Add,

    } RenderMode;

    void vClear(void);

    // Model renderers
    void vRenderSpot(Spot&, RenderMode);
    void vRenderBackground(Background&, RenderMode);

private:
    PixelBuf& pixel_buf;
    uint16_t u16_pixel_start;
    uint16_t u16_num_pixels;
    uint16_t u16_pixel_top;
};

#endif // RING_VIEW_H
