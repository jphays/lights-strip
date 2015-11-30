// ===========================================
// Light patterns for DotStar APA102 strip.
// Based on FastLED 3.1 demo reel.
// Josh Hays, 8/2015
// ===========================================

#include <Button.h>
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include "settings.h"
#include "utils.h"
#include "palettes.h"
#include "patterns.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later."
#endif

CRGB leds[NUM_LEDS]; // actual output array
CRGB buffer[2][NUM_LEDS]; // intermediate buffers

Button gPatternButton(BUTTON1_PIN, true, true, 20);
Button gPaletteButton(BUTTON2_PIN, true, true, 20);


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
typedef void (*SimplePattern)(CRGB*, unsigned long);
SimplePattern gPatterns[] = {
    paletteSweep,
    paletteSweepWithGlitter,
    distributed,
    leaderSpread,
    confetti,
    juggle,
    sinelon,
    candle,
};

// List of palettes to use
CRGBPalette16 gPalettes[] = {
    CRGBPalette16(RainbowColors_p),
    CRGBPalette16(RainbowStripeColors_p),
    CRGBPalette16(PartyColors_p),
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
};

// Palette generation functions
typedef CRGBPalette16 (*PaletteFunction)();
PaletteFunction gPaletteFuncs[] = {
    getPulsePalette,
    getPulse2Palette,
    getPulse4Palette,
    getStrobePalette,
    getStrobe2Palette,
    getRampPalette,
    getCWCBPalette,
};

// current and next palette, for smooth transitions
CRGBPalette16 gCurrentPalette(gPalettes[0]); // could init with intro palette, e.g. CRGB::LightGrey
CRGBPalette16 gTargetPalette(gPalettes[0]);

uint8_t gCurrentPatternNumber = 0; // Index of current pattern
uint8_t gPreviousPatternNumber = 0; // Index of previous pattern
uint8_t gIndex = 0; // rotating index of "current" color

bool gCycle = true; // whether to automatically cycle patterns
bool gRandomize = true; // whether to add some randomness

bool gTransitioning = false; // currently in a transition?
int gTransitionMillis = 3000; // transition length

// time and frame state
unsigned long gFrame = 0;
unsigned long gSceneFrame[2] = { 0, 0 };
unsigned long gCurrentTime = 0;
unsigned long gPreviousTime = 0;
unsigned long gTransitionTime = 0;


// Main Loop
// ---------

void loop()
{

    // update state
    gFrame++;
    gSceneFrame[CUR]++;
    gSceneFrame[PREV]++;
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
    gPatterns[gCurrentPatternNumber](buffer[CUR], gSceneFrame[CUR]);

    // transition if necessary
    if (gTransitioning)
    {
        // render the previous pattern to the previous buffer
        gPatterns[gPreviousPatternNumber](buffer[PREV], gSceneFrame[PREV]);

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
    gSceneFrame[PREV] = gSceneFrame[CUR];
    gSceneFrame[CUR] = 0;
}

void nextPalette()
{
    if (gRandomize)
    {
        uint8_t paletteType = random8(100);
        if (paletteType < 70)
            gTargetPalette = gPalettes[random8(ARRAY_SIZE(gPalettes))];
        else
            gTargetPalette = gPaletteFuncs[random8(ARRAY_SIZE(gPaletteFuncs))]();
    }
    else
    {
        static uint8_t paletteIndex = 0;

        // pick one:
        //gTargetPalette = gPalettes[paletteIndex++ % ARRAY_SIZE(gPalettes)];
        gTargetPalette = gPaletteFuncs[paletteIndex++ % ARRAY_SIZE(gPaletteFuncs)]();
    }
}
