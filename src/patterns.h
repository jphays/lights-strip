// Patterns
// --------

extern CRGBPalette16 gCurrentPalette;
extern uint8_t gIndex;

void paletteSweep(CRGB* pixels, unsigned long frame)
{
    // basic pattern. cycle through the palette with a slight spread of colors.
    CRGBPalette16 palette = gCurrentPalette;
    for (int i = 0; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 7));
    }
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

void paletteSweepWithGlitter(CRGB* pixels, unsigned long frame)
{
    // basic pattern with added glitter fading in.
    paletteSweep(pixels, frame);
    addGlitter(pixels, min(frame / 20, 100));
}

void leaderSpread(CRGB* pixels, unsigned long frame)
{
    // basic pattern with the center hue a few frames ahead of the rest.
    CRGBPalette16 palette = gCurrentPalette;
    pixels[0] = ColorFromPalette(palette, gIndex + 40);
    for (int i = 1; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(palette, gIndex + (i * 2));
    }
}

void distributed(CRGB* pixels, unsigned long frame)
{
    // palette hues evenly distributed among leds.
    const uint8_t spread = 255 / NUM_LEDS;
    //pixels[0] = ColorFromPalette(gCurrentPalette, sin8(gIndex));
    for (int i = 0; i < NUM_LEDS; i++)
    {
        pixels[i] = ColorFromPalette(gCurrentPalette, gIndex + spread * i);
    }
}

void confetti(CRGB* pixels, unsigned long frame)
{
    // random colored speckles that blink in and fade smoothly.
    fadeToBlackBy(pixels, NUM_LEDS, 10);
    for (uint8_t i = 0; i < beatsin8(10, 1, 3); i++)
    {
        int pos = random16(NUM_LEDS);
        pixels[pos] += ColorFromPalette(gCurrentPalette, gIndex + i * 16);
    }
}

void juggle(CRGB* pixels, unsigned long frame)
{
    // colored dots, weaving in and out of sync with each other.
    fadeToBlackBy(pixels, NUM_LEDS, 20);
    for (int i = 1; i <= 7; i++)
    {
        pixels[beatsin16(i, 0, NUM_LEDS)] |= ColorFromPalette(gCurrentPalette, gIndex + i * 3);
    }
}

void sinelon(CRGB* pixels, unsigned long frame)
{
    // a colored dot sweeping back and forth, with fading trails
    static uint8_t bpm = 6, loops = 5;
    if (frame == 0)
    {
        bpm = random8(2, 18);
        loops = random8(1, 8);
    }

    int beat = beatsin16(bpm, 0, NUM_LEDS * loops);
    int pos = beat % NUM_LEDS;
    static int prev_beat;
    if (frame == 0) prev_beat = beat;

    fadeToBlackBy(pixels, NUM_LEDS, 20);
    pixels[pos] = ColorFromPalette(gCurrentPalette, gIndex + pos);

    if (beat > prev_beat + 1)
    {
        for (int i = prev_beat + 1; i < beat; i++)
        {
            pixels[i % NUM_LEDS] = ColorFromPalette(gCurrentPalette, gIndex + pos);
        }
    }

    if (beat < prev_beat - 1)
    {
        for (int i = prev_beat - 1; i > beat; i--)
        {
            pixels[i % NUM_LEDS] = ColorFromPalette(gCurrentPalette, gIndex + pos);
        }
    }

    prev_beat = beat;
}

void candle(CRGB* pixels, unsigned long frame)
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

