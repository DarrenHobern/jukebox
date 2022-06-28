#include <FastLED.h>

#define LED_PIN     5
#define TOTAL_LEDS  120
#define NUM_LEDS    120
#define BRIGHTNESS  10
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[TOTAL_LEDS];

#define FPS 200
#define MAX_VALUE 255

void UpdateLEDSFromPalette_ChaseForward();
void UpdateLEDSFromPalette_ChaseBack();
void UpdateLEDSFromPalette_Gradient();

CRGBPalette16 palette;
TBlendType    blending;

uint8_t index;
uint8_t colour_step;
void (*pattern_ptr)(void);

extern const TProgmemPalette16 chaseForward_p PROGMEM;
extern const TProgmemPalette16 chaseBackward_p PROGMEM;
extern const TProgmemPalette16 gradientUp_p PROGMEM;
extern const TProgmemPalette16 gradientDown_p PROGMEM;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, TOTAL_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  
  palette = chaseForward_p;
  blending = LINEARBLEND;
  colour_step = MAX_VALUE / NUM_LEDS;

  // Zero off all LEDS on start
  for( uint8_t i = 0; i < TOTAL_LEDS; ++i) {
    leds[i] = CRGB::Black;
  }

  pattern_ptr = UpdateLEDSFromPalette_Gradient;

}

void loop() {
  // put your main code here, to run repeatedly:
  index = wrapIncrement(index, 1, MAX_VALUE); // Move the start index along, this causes the actual chase effect

  UpdatePattern();
  pattern_ptr();

  FastLED.show();
  FastLED.delay(1000 / FPS);
}

void UpdatePattern() {
  uint8_t s = (millis() / 1000) % 60;
  static uint8_t last = 99;
  if (last != s) {
    last = s;
    if (s == 0) {
      index = 0;
        pattern_ptr = UpdateLEDSFromPalette_ChaseBack;
    }
    if (s == 20) {
      index = 0;
        pattern_ptr = UpdateLEDSFromPalette_Gradient;
    }
    if (s == 40) {
      index = 0;
        pattern_ptr = UpdateLEDSFromPalette_ChaseForward;
    }
  }
}

void UpdateLEDSFromPalette_ChaseForward() {
  fill_palette_circular(leds, NUM_LEDS, index, palette, 120, blending, false);
}

void UpdateLEDSFromPalette_ChaseBack() {
  fill_palette_circular(leds, NUM_LEDS, index, palette, 120, blending, true);
}

void UpdateLEDSFromPalette_Gradient() {
//  fill_rainbow_circular(leds, NUM_LEDS, 0, false);
  for( uint8_t i = 0; i < NUM_LEDS; ++i) {
      leds[i] = ColorFromPalette( RainbowColors_p, index, 120, blending);
  }
}

const TProgmemPalette16 chaseForward_p PROGMEM =
{
    CRGB::Green,
    CRGB::Maroon,
    CRGB::Green,
    CRGB::Maroon,
 
    CRGB::DarkRed,
    CRGB::Maroon,
    CRGB::DarkRed,
    CRGB::Maroon,
 
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::Red,
    CRGB::Orange,
 
    CRGB::White,
    CRGB::Orange,
    CRGB::Red,
    CRGB::DarkRed
};


uint8_t wrapIncrement(uint8_t idx, uint8_t inc, uint8_t maxValue)
{
    uint8_t a = idx + inc;
    uint8_t r = a % maxValue;
    return r < 0 ? r + maxValue : r;
}
