#include <Adafruit_NeoPixel.h>

//----------------------------------------------------------------------
// constants

#define NEO_DATA_PIN   0

#define TOP_LED_FIRST  3 // Change these if the first pixel is not
#define TOP_LED_SECOND 3 // at the top of the first and/or second ring.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

const int8_t PROGMEM
  yCoord[] = { // Vertical coordinate of each pixel.  First pixel is at top.
    127,117,90,49,0,-49,-90,-117,-127,-117,-90,-49,0,49,90,117 },
  sine[] = { // Brightness table for ecto effect
    0, 28, 96, 164, 192, 164, 96, 28, 0, 28, 96, 164, 192, 164, 96, 28 };

// Eyelid vertical coordinates.  Eyes shut slightly below center.
#define upperLidTop     130
#define upperLidBottom  -45
#define lowerLidTop     -40
#define lowerLidBottom -130


//----------------------------------------------------------------------
// data

uint32_t
  iColor[16][3];      // Background colors for eyes
int16_t
  hue          =   0; // Initial hue around perimeter (0-1535)
uint8_t
  iBrightness[16],    // Brightness map -- eye colors get scaled by these
  brightness   =  15, // Global brightness (0-255)
  blinkFrames  =   5, // Speed of current blink
  blinkCounter =  30, // Countdown to end of next blink
  eyePos       = 192, // Current 'resting' eye (pupil) position
  newEyePos    = 192, // Next eye position when in motion
  gazeCounter  =  75, // Countdown to next eye movement
  gazeFrames   =  50; // Duration of eye movement (smaller = faster)
int8_t
  eyeMotion    =   0; // Distance from prior to new position


//----------------------------------------------------------------------
// functions

static void updateAllPixels()
{
  uint8_t i = 0;
  uint8_t r, g, b;

  for(i=0; i<16; i++) {
    uint8_t a = iBrightness[i] + 1;
    // First eye
    r = iColor[i][0];            // Initial background RGB color
    g = iColor[i][1];
    b = iColor[i][2];
    if(a) {
      r = (r * a) >> 8;          // Scale by brightness map
      g = (g * a) >> 8;
      b = (b * a) >> 8;
    }
    pixels.setPixelColor(((i + TOP_LED_FIRST) & 15), r, g, b);

    // Second eye uses the same colors, but reflected horizontally.
    // The same brightness map is used, but not reflected (same left/right)
    r = iColor[15 - i][0];
    g = iColor[15 - i][1];
    b = iColor[15 - i][2];
    if(a) {
      r = (r * a) >> 8;
      g = (g * a) >> 8;
      b = (b * a) >> 8;
    }
    pixels.setPixelColor(16 + ((i + TOP_LED_SECOND) & 15), r, g, b);
  }
  pixels.show();
}


void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
  // Seed random number generator from an unused analog input:
  randomSeed(analogRead(2));
#else
  randomSeed(analogRead(A0));
#endif

  pixels.begin();
}

void loop() {
  uint8_t i, r, g, b, a, c, inner, outer, ep;
  int     y1, y2, y3, y4, h;
  int8_t  y;

  //-------------------------------------------------------
  // rainbow effect
  // Draw eye background colors

  // This renders a glotating rainbow...a WAY overdone LED effect but
  // does show the color gamut nicely.

  for(h=hue, i=0; i<16; i++, h += 96) {
    a = h;
    switch((h >> 8) % 6) {
     case 0: iColor[i][0] = 255; iColor[i][1] =   a; iColor[i][2] =   0; break;
     case 1: iColor[i][0] =  ~a; iColor[i][1] = 255; iColor[i][2] =   0; break;
     case 2: iColor[i][0] =   0; iColor[i][1] = 255; iColor[i][2] =   a; break;
     case 3: iColor[i][0] =   0; iColor[i][1] =  ~a; iColor[i][2] = 255; break;
     case 4: iColor[i][0] =   a; iColor[i][1] =   0; iColor[i][2] = 255; break;
     case 5: iColor[i][0] = 255; iColor[i][1] =   0; iColor[i][2] =  ~a; break;
    }
  }
  hue += 7;
  if(hue >= 1536) hue -= 1536;
  // end rainbow effect
  //-------------------------------------------------------

  // Render current blink (if any) into brightness map
  if(blinkCounter <= blinkFrames * 2) { // In mid-blink?
    if(blinkCounter > blinkFrames) {    // Eye closing
      outer = blinkFrames * 2 - blinkCounter;
      inner = outer + 1;
    } else {                            // Eye opening
      inner = blinkCounter;
      outer = inner - 1;
    }
    y1 = upperLidTop    - (upperLidTop - upperLidBottom) * outer / blinkFrames;
    y2 = upperLidTop    - (upperLidTop - upperLidBottom) * inner / blinkFrames;
    y3 = lowerLidBottom + (lowerLidTop - lowerLidBottom) * inner / blinkFrames;
    y4 = lowerLidBottom + (lowerLidTop - lowerLidBottom) * outer / blinkFrames;
    for(i=0; i<16; i++) {
      y = pgm_read_byte(&yCoord[i]);
      if(y > y1) {        // Above top lid
        iBrightness[i] = 0;
      } else if(y > y2) { // Blur edge of top lid in motion
        iBrightness[i] = brightness * (y1 - y) / (y1 - y2);
      } else if(y > y3) { // In eye
        iBrightness[i] = brightness;
      } else if(y > y4) { // Blur edge of bottom lid in motion
        iBrightness[i] = brightness * (y - y4) / (y3 - y4);
      } else {            // Below bottom lid
        iBrightness[i] = 0;
      }
    }
  } else { // Not in blink -- set all 'on'
    memset(iBrightness, brightness, sizeof(iBrightness));
  }

  if(--blinkCounter == 0) { // Init next blink?
    blinkFrames  = random(4, 8);
    blinkCounter = blinkFrames * 2 + random(5, 180);
  }

  // Calculate current eye movement, possibly init next one
  if(--gazeCounter <= gazeFrames) { // Is pupil in motion?
    ep = newEyePos - eyeMotion * gazeCounter / gazeFrames; // Current pos.
    if(gazeCounter == 0) {                   // Last frame?
      eyePos      = newEyePos;               // Current position = new pos
      newEyePos   = random(16) * 16;         // New pos. (always pixel center)
      eyeMotion   = newEyePos - eyePos;      // Distance to move
      gazeFrames  = random(10, 20);          // Duration of movement
      gazeCounter = random(gazeFrames, 130); // Count to END of next movement
    }
  } else ep = eyePos; // Not moving -- fixed gaze

  // Draw pupil -- 2 pixels wide, but sup-pixel positioning may span 3.
  a = ep >> 4;         // First candidate
  b = (a + 1)  & 0x0F; // 1 pixel CCW of a
  c = (a + 2)  & 0x0F; // 2 pixels CCW of a
  i = ep & 0x0F;       // Fraction of 'c' covered (0-15)
  iBrightness[a] = (iBrightness[a] *       i ) >> 4;
  iBrightness[b] = 0;
  iBrightness[c] = (iBrightness[c] * (16 - i)) >> 4;

  updateAllPixels();
  delay(15);
}

