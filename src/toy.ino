#include "FastLED.h"
#include "palettes.h"

FASTLED_USING_NAMESPACE

// Toy cube light for Selina
// Based on FastLED 3.1 demo reel.
// Josh Hays, 8/2015

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later."
#endif

#define DATA_PIN    9
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    7
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND   60

void setup()
{
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
    // FastLED.setCorrection(TypicalLEDStrip);
    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePattern)();
SimplePattern gPatterns[] = {
    paletteSweep,
    paletteSweepWithGlitter,
    confetti,
    juggle,
    leaderSpread,
    contrast,
    sinelon,
    bpm,
};

// List of palettes to use
CRGBPalette16 gPalettes[] = {
    CRGBPalette16(RainbowColors_p),
    CRGBPalette16(CloudColors_p),
    CRGBPalette16(ForestColors_p),
    modifiedRainbow_p,
    CRGBPalette16(LavaColors_p),
    CRGBPalette16(OceanColors_p),
};

// current and next palette, for smooth transitions
CRGBPalette16 gCurrentPalette(gPalettes[0]); // intro palette, e.g. CRGB::LightGrey
CRGBPalette16 gTargetPalette(gPalettes[0]);

uint8_t gCurrentPatternNumber = 0; // Index of current pattern
uint8_t gCurrentPaletteNumber = 0; // Index of current palette
uint8_t gIndex = 0; // rotating index of "current" color

// time and frame state
unsigned long gFrame = 0;
unsigned long gSceneFrame = 0;
unsigned long gCurrentTime = 0;
unsigned long gPreviousTime = 0;

void loop()
{

    // update state
    gFrame++;
    gSceneFrame++;
    gPreviousTime = gCurrentTime;
    gCurrentTime = millis();

    // call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS(20) { gIndex++; } // slowly cycle the "base color" through the palette
    EVERY_N_MILLISECONDS(20) { nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette); }
    EVERY_N_SECONDS(15) { nextPattern(); } // change patterns periodically
    EVERY_N_SECONDS(30) { nextPalette(); } // change palettes periodically

}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
    // add one to the current pattern number, and wrap around at the end
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
    gSceneFrame = 0;
}

void randomPattern()
{
    // choose a random pattern, but don't pick the same one twice in a row
    gCurrentPatternNumber = random8Except(ARRAY_SIZE(gPatterns), gCurrentPatternNumber);
    gSceneFrame = 0;
}

void nextPalette()
{
    gCurrentPaletteNumber = (gCurrentPaletteNumber + 1) % ARRAY_SIZE(gPalettes);
    gTargetPalette = gPalettes[gCurrentPaletteNumber];
}

void randomPalette()
{
    gCurrentPaletteNumber = random8Except(ARRAY_SIZE(gPalettes), gCurrentPaletteNumber);
    gTargetPalette = gPalettes[gCurrentPaletteNumber];
    // flash occasionally
    if (random8(100) < 20) gCurrentPalette = CRGBPalette16(CRGB::LightGrey);
}

void paletteSweep()
{
    CRGBPalette16 palette = gCurrentPalette;
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = ColorFromPalette(palette, gIndex + (i * 7));
    }
}

void paletteSweepWithGlitter()
{
    paletteSweep();
    // fade in the glitter
    addGlitter(min(gSceneFrame / 7, 80));
}

void addGlitter(fract8 chanceOfGlitter)
{
    if (random8() < chanceOfGlitter)
    {
        int led = random16(NUM_LEDS);
        leds[led] += leds[led]; //CRGB::White;
    }
}

void contrast()
{
    CRGBPalette16 palette = gCurrentPalette;
    leds[0] = ColorFromPalette(palette, (gIndex + 128) % 256);
    for (int i = 1; i < NUM_LEDS; i++)
    {
        leds[i] = ColorFromPalette(palette, gIndex + (i * 7));
    }
}

void leaderSpread()
{
    CRGBPalette16 palette = gCurrentPalette;
    leds[0] = ColorFromPalette(palette, (gIndex + 40) % 256);
    for (int i = 1; i < NUM_LEDS; i++)
    {
        leds[i] = ColorFromPalette(palette, gIndex + (i * 2));
    }
}

void confetti()
{
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += ColorFromPalette(gCurrentPalette, gIndex + random8(64));
}

void juggle()
{
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(leds, NUM_LEDS, 20);
    for (int i = 0; i < 8; i++)
    {
        leds[beatsin16(i+7,0,NUM_LEDS)] |= ColorFromPalette(gCurrentPalette, gIndex);
    }
}

void sinelon()
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(leds, NUM_LEDS, 20);
    int beat = beatsin16(6, 0, (NUM_LEDS - 1) * 3) + beatsin16(4, 0, 12);
    int pos = beat % (NUM_LEDS - 1) + 1;
    leds[pos] += ColorFromPalette(gCurrentPalette, gIndex + pos * 2);

    if (random8(100) < 3)
    {
        leds[0] += ColorFromPalette(gCurrentPalette, (gIndex + 10) % 256) +
            ColorFromPalette(gCurrentPalette, gIndex);
    }
}

void bpm()
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 32;
    CRGBPalette16 palette = gCurrentPalette;
    uint8_t beat = beatsin8(BeatsPerMinute, 100, 200) + beatsin8(BeatsPerMinute * 8 / 3, 0, 55);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = ColorFromPalette(palette, gIndex+(i*2)); // , beat-gIndex+(i*3));
    }
}

uint8_t random8Except(uint8_t max, uint8_t except)
{
    // returns a random number 0 <= r < max, but won't return r == except.
    // useful for picking a random array value different from the previous one.
    uint8_t r = random8(max);
    while (r == except && max > 1)
    {
        r = random8(max);
    }
    return r;
}
