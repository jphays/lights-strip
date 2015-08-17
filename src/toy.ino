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
CRGB leds[NUM_LEDS]; // actual output array
CRGB buffer[2][NUM_LEDS]; // intermediate buffers

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
typedef void (*SimplePattern)(CRGB* pixels);
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
uint8_t gPreviousPatternNumber = 0; // Index of previous pattern

uint8_t gCurrentPaletteNumber = 0; // Index of current palette
uint8_t gIndex = 0; // rotating index of "current" color

bool gRandomize = false; // whether to add some randomness
bool gTransitioning = false; // currently in a transition?
int gTransitionMillis = 2000; // transition length

// time and frame state
unsigned long gFrame = 0;
unsigned long gSceneFrame = 0;
unsigned long gCurrentTime = 0;
unsigned long gPreviousTime = 0;
unsigned long gTransitionTime = 0;

void loop()
{

    // update state
    gFrame++;
    gSceneFrame++;
    gPreviousTime = gCurrentTime;
    gCurrentTime = millis();

    // call the current pattern function once, updating the 'buffer' array
    gPatterns[gCurrentPatternNumber](buffer[0]);


    if (gTransitioning)
    {
        // transition if necessary
        gPatterns[gPreviousPatternNumber](buffer[1]);
        fract8 transitionPercent = ((gCurrentTime - gTransitionTime) * 256) / gTransitionMillis;
        blend(buffer[1], buffer[0], leds, NUM_LEDS, transitionPercent);
        if (transitionPercent >= 255)
        {
            gTransitioning = false;
        }
    }
    else
    {
        // copy buffer to leds
        memmove8(&leds, &buffer[0], NUM_LEDS * sizeof(CRGB));
    }

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
    gPreviousPatternNumber = gCurrentPatternNumber;

    gCurrentPatternNumber = gRandomize ?
        random8Except(ARRAY_SIZE(gPatterns), gCurrentPatternNumber) :
        (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);

    memmove8(&buffer[1], &buffer[0], NUM_LEDS * sizeof(CRGB));
    memset(buffer[0], 0, sizeof(CRGB));

    gTransitioning = true;
    gTransitionTime = gCurrentTime;
    gSceneFrame = 0;
}

void nextPalette()
{
    gCurrentPaletteNumber = gRandomize ?
        random8Except(ARRAY_SIZE(gPalettes), gCurrentPaletteNumber) :
        (gCurrentPaletteNumber + 1) % ARRAY_SIZE(gPalettes);

    gTargetPalette = gPalettes[gCurrentPaletteNumber];
    // flash occasionally
    if (gRandomize && random8(100) < 20) gCurrentPalette = CRGBPalette16(CRGB::LightGrey);
}

void randomPalette()
{
    gCurrentPaletteNumber = random8Except(ARRAY_SIZE(gPalettes), gCurrentPaletteNumber);
    gTargetPalette = gPalettes[gCurrentPaletteNumber];
}

void paletteSweep(CRGB* pixels)
{
    CRGBPalette16 palette = gCurrentPalette;
    for (int i = 0; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 7));
    }
}

void paletteSweepWithGlitter(CRGB* pixels)
{
    paletteSweep(pixels);
    // fade in the glitter
    addGlitter(pixels, min(gSceneFrame / 7, 80));
}

void addGlitter(CRGB* pixels, fract8 chanceOfGlitter)
{
    if (random8() < chanceOfGlitter)
    {
        int led = random16(NUM_LEDS);
        pixels[led] += pixels[led];
    }
}

void contrast(CRGB* pixels)
{
    CRGBPalette16 palette = gCurrentPalette;
    pixels[0] = ColorFromPalette(palette, (gIndex + 128) % 256);
    for (int i = 1; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 7));
    }
}

void leaderSpread(CRGB* pixels)
{
    CRGBPalette16 palette = gCurrentPalette;
    pixels[0] = ColorFromPalette(palette, (gIndex + 40) % 256);
    for (int i = 1; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 2));
    }
}

void confetti(CRGB* pixels)
{
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(pixels, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    pixels[pos] += ColorFromPalette(gCurrentPalette, gIndex + random8(64));
}

void juggle(CRGB* pixels)
{
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(pixels, NUM_LEDS, 20);
    for (int i = 0; i < 8; i++)
    {
        pixels[beatsin16(i+7,0,NUM_LEDS)] |= ColorFromPalette(gCurrentPalette, gIndex);
    }
}

void sinelon(CRGB* pixels)
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(pixels, NUM_LEDS, 20);
    int beat = beatsin16(6, 0, (NUM_LEDS - 1) * 3) + beatsin16(4, 0, 12);
    int pos = beat % (NUM_LEDS - 1) + 1;
    pixels[pos] += ColorFromPalette(gCurrentPalette, gIndex + pos * 2);

    if (random8(100) < 3)
    {
        pixels[0] += ColorFromPalette(gCurrentPalette, (gIndex + 10) % 256) +
            ColorFromPalette(gCurrentPalette, gIndex);
    }
}

void bpm(CRGB* pixels)
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 32;
    CRGBPalette16 palette = gCurrentPalette;
    uint8_t beat = beatsin8(BeatsPerMinute, 100, 200) + beatsin8(BeatsPerMinute * 8 / 3, 0, 55);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex+(i*2)); // , beat-gIndex+(i*3));
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
