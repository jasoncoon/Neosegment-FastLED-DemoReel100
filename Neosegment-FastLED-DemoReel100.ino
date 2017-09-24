#include "FastLED.h"
#include "NeoSegment.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

// Modified for example use with Neosegment

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    11
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    56
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { spell, count, spin1, spin2, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{
  random16_add_entropy(random());

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10  ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

const uint8_t digitCount = 8;

void count() {
  FastLED.clear();

  static uint8_t digits[digitCount];

  EVERY_N_MILLIS(1) {
    digits[digitCount - 1]++;

    for (uint8_t i = digitCount - 1; i > 0; i--) {
      if (digits[i] == 10) {
        digits[i] = 0;

        digits[i - 1] += 1;
      }
    }
  }

  for (uint8_t i = 0; i < digitCount; i++) {
    uint8_t hue = gHue + (256 / digitCount) * i;
    CHSV color = CHSV(hue, 255, 255);
    setDigit(i, digits[i], color, leds);
  }
}

void spell() {
  fadeToBlackBy( leds, NUM_LEDS, 20);

  static uint8_t spinnerIndex = 0;

  EVERY_N_MILLIS(60) {
    spinnerIndex++;
    if(spinnerIndex >= 7)
      spinnerIndex = 0;
  }
  
  uint8_t hue = gHue + (256 / digitCount) * 7;
  CHSV color = CHSV(hue, 255, 255);
  
  leds[spinnerIndex] = color;
  
  char letters[] = " fastled";

  for(uint8_t i = 1; i < digitCount; i++) {
    uint8_t hue = gHue + (256 / digitCount) * i;
    CHSV color = CHSV(hue, 255, 255);
    setLetter(i, letters[i], color, leds);
  }
}

const uint8_t chance = 192;

uint8_t branchClockwise(uint8_t i) {
  switch (mod8(i, 7)) {
    case 0:
      return random8() > chance ? i + 3 : i + 4;

    case 1:
      if (((int) i - 7) < 0)
        return i - 1;
      return random8() > chance ? i - 1 : i - 7;

    case 2:
      return i - 1;

    case 3:
      if (i + 7 >= NUM_LEDS)
        return i - 1;
      return random8() > chance ? i - 1 : i + 7;

    case 4:
      return i + 1;

    case 5:
      if (i + 7 >= NUM_LEDS)
        return i + 1;
      return random8() > chance ? i + 1 : i + 7;

    case 6:
      return i - 4;
  }
}

uint8_t branchCounterClockwise(uint8_t i) {
  switch (mod8(i, 7)) {
    case 0:
      return i + 1;

    case 1:
      if (i + 7 >= NUM_LEDS)
        return i + 1;
      return random8() > chance ? i + 1 : i + 7;

    case 2:
      return random8() > chance ? i + 1 : i + 4;

    case 3:
      if (((int) i - 7) < 0)
        return i - 3;
      return random8() > chance ? i - 3 : i - 7;

    case 4:
      return i - 4;

    case 5:
      if (((int) i - 7) < 0)
        return i - 1;
      return random8() > chance ? i - 1 : i - 7;

    case 6:
      return i - 1;
  }
}

void spin1() {
  // a colored dot spinning around the display, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);

  static uint8_t position0 = 0;

  EVERY_N_MILLIS(60) {
    position0 = branchCounterClockwise(position0);
  }

  leds[position0] += CHSV( gHue, 255, 192);
}

void spin2() {
  // a colored dot spinning around the display, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);

  static uint8_t position0 = 0;
  static uint8_t position1 = NUM_LEDS - 1;

  EVERY_N_MILLIS(60) {
    position0 = branchCounterClockwise(position0);
    position1 = branchClockwise(position1);
  }

  leds[position0] += CHSV( gHue, 255, 192);
  leds[position1] += CHSV( gHue + 128, 255, 192);
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 4; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 64;
  }
}
