#include <FastLED.h>
#include <Button.h>
#include "palettes.h"

FASTLED_USING_NAMESPACE

// ===========================================
// Light patterns for DotStar APA102 strip.
// Based on FastLED 3.1 demo reel.
// Josh Hays, 8/2015
// ===========================================

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later."
#endif

#define DATA_PIN    3
#define CLOCK_PIN   4
#define LED_TYPE    APA102
#define COLOR_ORDER GBR
#define NUM_LEDS    61

CRGB leds[NUM_LEDS]; // actual output array
CRGB buffer[2][NUM_LEDS]; // intermediate buffers

#define CUR 0 // index to current buffer
#define PREV 1 // index to previous buffer

#define BUTTON1_PIN 5
#define BUTTON2_PIN 6

Button gPatternButton(BUTTON1_PIN, true, true, 20);
Button gPaletteButton(BUTTON2_PIN, true, true, 20);

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// Initialization
// --------------

void setup()
{
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    // FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    // set up serial
    Serial.begin(57600);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePattern)(CRGB* pixels);
SimplePattern gPatterns[] = {
    paletteSweep,
    paletteSweepWithGlitter,
    distributed,
    leaderSpread,
    confetti,
    juggle,
    //complement,
    //whirl,
    //wipe,
    sinelon,
    candle,
    //pulseTracer,
    //beatPhaser,
};

// List of palettes to use
CRGBPalette16 gPalettes[] = {
    CRGBPalette16(RainbowColors_p),
    CRGBPalette16(RainbowStripeColors_p),
    CRGBPalette16(PartyColors_p),
    CRGBPalette16(CloudColors_p),
    CRGBPalette16(ForestColors_p),
    CRGBPalette16(LavaColors_p),
    CRGBPalette16(OceanColors_p),
    modifiedRainbow_p,
    coldFire_p,
    royal_p,
    bp_p,
    spring_gp,
    summer_gp,
    autumn_gp,
    winter_gp,
    hot_gp,
    cool_gp,
    //kelvino_p,
    //achilles_p
};

// current and next palette, for smooth transitions
CRGBPalette16 gCurrentPalette(gPalettes[0]); // intro palette, e.g. CRGB::LightGrey
CRGBPalette16 gTargetPalette(gPalettes[0]);

uint8_t gCurrentPatternNumber = 0; // Index of current pattern
uint8_t gPreviousPatternNumber = 0; // Index of previous pattern

uint8_t gCurrentPaletteNumber = 0; // Index of current palette
uint8_t gIndex = 0; // rotating index of "current" color

bool gCycle = true; // whether to automatically cycle patterns
bool gRandomize = true; // whether to add some randomness

bool gTransitioning = false; // currently in a transition?
int gTransitionMillis = 3000; // transition length

// time and frame state
unsigned long gFrame = 0;
unsigned long gSceneFrame = 0;
unsigned long gCurrentTime = 0;
unsigned long gPreviousTime = 0;
unsigned long gTransitionTime = 0;


// Main Loop
// ---------

void loop()
{

    // update state
    gFrame++;
    gSceneFrame++;
    gPreviousTime = gCurrentTime;
    gCurrentTime = millis();

    // read buttons
    handleInput();

    // render the current frame to the led buffer, transitioning scenes if necessary
    renderFrame();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS(20) { gIndex++; } // slowly cycle the "base color" through the palette
    EVERY_N_MILLISECONDS(20) { nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette); }
    EVERY_N_SECONDS(15) { if (gCycle) nextPattern(); } // change patterns periodically
    EVERY_N_SECONDS(30) { if (gCycle) nextPalette(); } // change palettes periodically

}

void renderFrame()
{

    // render the current pattern to the current buffer
    gPatterns[gCurrentPatternNumber](buffer[CUR]);

    // transition if necessary
    if (gTransitioning)
    {
        // render the previous pattern to the previous buffer
        gPatterns[gPreviousPatternNumber](buffer[PREV]);

        // calculate the transition progress and blend the current and previous scenes to the led array
        fract8 transitionPercent = min(((gCurrentTime - gTransitionTime) * 256) / gTransitionMillis, 255);
        blend(buffer[PREV], buffer[CUR], leds, NUM_LEDS, ease8InOutQuad(transitionPercent));

        // transition ending?
        if (transitionPercent >= 255)
        {
            gTransitioning = false;
        }
    }
    else
    {
        // no transition, just copy the current buffer to led array
        memmove8(&leds, &buffer[CUR], NUM_LEDS * sizeof(CRGB));
    }

}

void handleInput()
{
    gPatternButton.read();
    gPaletteButton.read();

    if (gPatternButton.wasPressed())
    {
        gCycle = false;
        gRandomize = false;
        gTransitionMillis = 500;
        nextPattern();
    }

    if (gPaletteButton.wasPressed())
    {
        gCycle = false;
        gRandomize = false;
        nextPalette();
    }

    if (gPatternButton.pressedFor(1000) || gPaletteButton.pressedFor(1000))
    {
        gCycle = true;
        gRandomize = true;
        gTransitionMillis = 3000;
    }
}


// Transitions
// -----------

void nextPattern()
{
    gPreviousPatternNumber = gCurrentPatternNumber;

    gCurrentPatternNumber = gRandomize ?
        random8Except(ARRAY_SIZE(gPatterns), gCurrentPatternNumber) :
        (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);

    memmove8(&buffer[PREV], &buffer[CUR], NUM_LEDS * sizeof(CRGB));
    memset(buffer[CUR], 0, sizeof(CRGB));

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
    //const uint8_t flashPercent = 20;
    //if (gRandomize && random8(100) < flashPercent) gCurrentPalette = CRGBPalette16(CRGB::White);
}


// Scenes
// ------

void paletteSweep(CRGB* pixels)
{
    // basic pattern. cycle through the palette with a slight spread of colors.
    CRGBPalette16 palette = gCurrentPalette;
    for (int i = 0; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 7));
    }
}

void paletteSweepWithGlitter(CRGB* pixels)
{
    // basic pattern with added glitter fading in.
    paletteSweep(pixels);
    addGlitter(pixels, min(gSceneFrame / 20, 100));
}

void addGlitter(CRGB* pixels, fract8 chanceOfGlitter)
{
    // adds a sparkle every chanceOfGlitter/255 frames.
    const uint8_t maxBright = 90;  // brightness of sparkle
    const uint8_t fadeSteps = 18;  // frames to fade
    static int glitter[fadeSteps]; // glitter led index history

    for (int i = fadeSteps - 1; i > 0; i--)
    {
        uint8_t level = scale8(ease8InOutCubic((fadeSteps - i) * (256 / fadeSteps)), maxBright);
        pixels[glitter[i]] += CRGB(level, level, level);
        glitter[i] = glitter[i - 1];
    }

    if (random8() < chanceOfGlitter)
    {
        int led = random16(NUM_LEDS);
        pixels[led] += CRGB(maxBright, maxBright, maxBright);
        glitter[0] = led;
    }
    else
    {
        glitter[0] = NUM_LEDS;
    }
}

void complement(CRGB* pixels)
{
    // basic pattern with a complementary hue in the center.
    CRGBPalette16 palette = gCurrentPalette;
    pixels[0] = ColorFromPalette(palette, gIndex + 128);
    for (int i = 1; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 7));
    }
}

void leaderSpread(CRGB* pixels)
{
    // basic pattern with the center hue a few frames ahead of the rest.
    CRGBPalette16 palette = gCurrentPalette;
    pixels[0] = ColorFromPalette(palette, gIndex + 40);
    for (int i = 1; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 2));
    }
}

void distributed(CRGB* pixels)
{
    // palette hues evenly distributed among leds.
    const uint8_t spread = 255 / NUM_LEDS;
    //pixels[0] = ColorFromPalette(gCurrentPalette, sin8(gIndex));
    for (int i = 0; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(gCurrentPalette, gIndex + spread * i);
    }
}

void confetti(CRGB* pixels)
{
    // random colored speckles that blink in and fade smoothly.
    fadeToBlackBy(pixels, NUM_LEDS, 10);
    for (uint8_t i = 0; i < beatsin8(10, 1, 3); i++)
    {
        int pos = random16(NUM_LEDS);
        pixels[pos] += ColorFromPalette(gCurrentPalette, gIndex + i * 16);
    }
}

void juggle(CRGB* pixels)
{
    // colored dots, weaving in and out of sync with each other.
    fadeToBlackBy(pixels, NUM_LEDS, 20);
    for (int i = 1; i <= 7; i++)
    {
        pixels[beatsin16(i, 0, NUM_LEDS)] |= ColorFromPalette(gCurrentPalette, gIndex + i * 3);
    }
}

void wipe(CRGB* pixels)
{
    // a sweep across the jewel hexagon.
    static long lastStepTime = 0;
    static uint8_t step = 0;
    static uint8_t direction = 0;
    int intensity = beatsin8(9, 0, 160);
    fadeToBlackBy(pixels, NUM_LEDS, beatsin8(6, 2, 20));
    CRGB currentColor = ColorFromPalette(gCurrentPalette, gIndex);

    if (gCurrentTime - lastStepTime >= 100)
    {
        if (step == 0)
        {
            pixels[0] = currentColor + CRGB(intensity, intensity, intensity);
        }

        if (step < 3)
        {
            wipeStep(pixels, currentColor, step, direction, false);
            step++;
        }
        else if (step == 3)
        {
            step = 0;
            direction = random8Except(6, direction);
        }

        lastStepTime = gCurrentTime;
    }
}

void whirl(CRGB* pixels)
{
    const uint8_t bpm = 6;
    const uint8_t cycles = 8;
    uint8_t phase = beatsin8(bpm, 0, 6 * cycles);

    wipeStep(pixels, ColorFromPalette(gCurrentPalette, gIndex + 60), 0, phase, false);
    wipeStep(pixels, ColorFromPalette(gCurrentPalette, gIndex), 1, phase, false);
    wipeStep(pixels, ColorFromPalette(gCurrentPalette, gIndex + 65), 2, phase, false);
    pixels[0] = ColorFromPalette(gCurrentPalette, gIndex) + CRGB(phase * 4, phase * 4, phase * 4);
}

void wipeStep(CRGB* pixels, CRGB color, uint8_t step, uint8_t direction, boolean includeCenter)
{
    switch (step % 3)
    {
        case 0:
            pixels[addmod8(0, direction, 6) + 1] = color;
            pixels[addmod8(1, direction, 6) + 1] = color;
            break;
        case 1:
            if (includeCenter) pixels[0] = color;
            pixels[addmod8(2, direction, 6) + 1] = color;
            pixels[addmod8(5, direction, 6) + 1] = color;
            break;
        case 2:
            pixels[addmod8(3, direction, 6) + 1] = color;
            pixels[addmod8(4, direction, 6) + 1] = color;
            break;
    }
}

void sinelon(CRGB* pixels)
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(pixels, NUM_LEDS, 20);
    int beat = beatsin16(12, 0, (NUM_LEDS - 1) * 2) + beatsin16(8, 0, 12);
    int pos = beat % (NUM_LEDS - 1) + 1;
    pixels[pos] += ColorFromPalette(gCurrentPalette, gIndex + pos * 2);

    if (beat == 0)
    {
        pixels[0] += ColorFromPalette(gCurrentPalette, gIndex + 10) +
            ColorFromPalette(gCurrentPalette, gIndex);
    }
}

void candle(CRGB* pixels)
{
    // slow flicker with some slight noise
    uint8_t flickerBpm = 16;
    CRGBPalette16 palette = gCurrentPalette;
    uint8_t beat = beatsin8(flickerBpm, 150, 220) + beatsin8(flickerBpm * 8 / 3, 0, 30);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 3), beat - random8(10)); // , beat-gIndex+(i*3));
    }
}

void pulseTracer(CRGB* pixels)
{
    uint8_t bpm = 10;
    int offset = beatsin8(bpm, 0, 32) - 16;
    int pos = beatsin16(bpm * 5 / 2, 0, NUM_LEDS - 1);

    for (int i = 1; i < NUM_LEDS; i++)
    {
        pixels[(i + pos) % (NUM_LEDS - 1) + 1] =
            ColorFromPalette(gCurrentPalette, gIndex + (i * offset), 255 - beatsin8(bpm, 100, 255));
    }

    pixels[0] = ColorFromPalette(gCurrentPalette, gIndex, beatsin8(bpm, 60, 255));
}

void beatPhaser(CRGB* pixels)
{
    fadeToBlackBy(pixels, NUM_LEDS, beatsin8(8, 2, 8));
    int pos = scale8(beat8(beatsin8(10, 0, 60)), NUM_LEDS - 1) + 1;
    pixels[pos] = ColorFromPalette(gCurrentPalette, gIndex, beatsin8(10, 100, 255));
}

// Utilities
// ---------

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
