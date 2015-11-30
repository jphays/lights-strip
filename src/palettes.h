#ifndef __INC_TOY_PALETTES_H
#define __INC_TOY_PALETTES_H

CRGBPalette16 getPulsePalette()
{
    uint8_t hue = random8();
    return CRGBPalette16(
        CHSV(hue, 255, 255), CRGB::Black, CHSV(hue, 255, 255), CRGB::Black,
        CHSV(hue, 255, 255), CRGB::Black, CHSV(hue, 255, 255), CRGB::Black,
        CHSV(hue, 255, 255), CRGB::Black, CHSV(hue, 255, 255), CRGB::Black,
        CHSV(hue, 255, 255), CRGB::Black, CHSV(hue, 255, 255), CRGB::Black);
}

CRGBPalette16 getPulse2Palette()
{
    uint8_t hue1 = random8();
    uint8_t hue2 = random8();
    return CRGBPalette16(
        CHSV(hue1, 255, 255), CRGB::Black, CHSV(hue2, 255, 255), CRGB::Black,
        CHSV(hue1, 255, 255), CRGB::Black, CHSV(hue2, 255, 255), CRGB::Black,
        CHSV(hue1, 255, 255), CRGB::Black, CHSV(hue2, 255, 255), CRGB::Black,
        CHSV(hue1, 255, 255), CRGB::Black, CHSV(hue2, 255, 255), CRGB::Black);
}

CRGBPalette16 getPulse4Palette()
{
    uint8_t hue1 = random8();
    uint8_t hue2 = random8();
    uint8_t hue3 = random8();
    uint8_t hue4 = random8();
    return CRGBPalette16(
        CHSV(hue1, 255, 255), CRGB::Black, CHSV(hue2, 255, 255), CRGB::Black,
        CHSV(hue3, 255, 255), CRGB::Black, CHSV(hue4, 255, 255), CRGB::Black,
        CHSV(hue1, 255, 255), CRGB::Black, CHSV(hue2, 255, 255), CRGB::Black,
        CHSV(hue3, 255, 255), CRGB::Black, CHSV(hue4, 255, 255), CRGB::Black);
}

CRGBPalette16 getStrobePalette()
{
    uint8_t hue = random8();
    return CRGBPalette16(
        CHSV(hue, 255, 255), CHSV(hue, 255, 255), CRGB::Black, CRGB::Black,
        CHSV(hue, 255, 255), CHSV(hue, 255, 255), CRGB::Black, CRGB::Black,
        CHSV(hue, 255, 255), CHSV(hue, 255, 255), CRGB::Black, CRGB::Black,
        CHSV(hue, 255, 255), CHSV(hue, 255, 255), CRGB::Black, CRGB::Black);
}

CRGBPalette16 getStrobe2Palette()
{
    uint8_t hue1 = random8();
    uint8_t hue2 = random8();
    return CRGBPalette16(
        CHSV(hue1, 255, 255), CHSV(hue1, 255, 255), CRGB::Black, CRGB::Black,
        CHSV(hue2, 255, 255), CHSV(hue2, 255, 255), CRGB::Black, CRGB::Black,
        CHSV(hue1, 255, 255), CHSV(hue1, 255, 255), CRGB::Black, CRGB::Black,
        CHSV(hue2, 255, 255), CHSV(hue2, 255, 255), CRGB::Black, CRGB::Black);
}

CRGBPalette16 getRampPalette()
{
    uint8_t hue1 = random8();
    uint8_t hue2 = random8();
    return CRGBPalette16(CHSV(hue1, 255, 255), CHSV(hue2, 255, 255));
}

CRGBPalette16 getEvenPalette(CRGB c0, CRGB c1, CRGB c2, CRGB c3)
{
    int p1 = 128;
    int p2 = 224;
    int p3 = 255;
    return CRGBPalette16(
        c0, blend(c0, c1, p1), blend(c0, c1, p2), blend(c0, c1, p3),
        c1, blend(c1, c2, p1), blend(c1, c2, p2), blend(c1, c2, p3),
        c2, blend(c2, c3, p1), blend(c2, c3, p2), blend(c2, c3, p3),
        c3, blend(c3, c0, p1), blend(c3, c0, p2), blend(c3, c0, p3));
}

CRGBPalette16 getCWCBPalette()
{
    return getEvenPalette(
            CHSV(random8(), 255, 255),
            CRGB::White,
            CHSV(random8(), 255, 255),
            CRGB::Black);
}

CRGBPalette16 modifiedRainbow_p = CRGBPalette16(
    CRGB::Red, CRGB::OrangeRed, CRGB::Orange, CRGB::Gold,
    CRGB::Yellow, CRGB::GreenYellow, CRGB::Lime, CRGB::Cyan,
    CRGB::DeepSkyBlue, CRGB::Blue, CRGB::Navy, CRGB::Indigo,
    CRGB::Purple, CRGB::Violet, CRGB::Magenta, CRGB::DeepPink);

DEFINE_GRADIENT_PALETTE(coldFire_p) {
      0,   0, 172, 252,
     32,   0,  68, 252,
     64,  32,  16, 212,
     96,  96,  48, 128,
    128, 160,  80,  48,
    160, 208, 132,   4,
    192, 236, 212,   4,
    224, 252, 252, 152,
    255, 252, 252, 252
};

DEFINE_GRADIENT_PALETTE(bp_p) {
      0, 168,  24, 152,
     96,   0,   0,  36,
    128,  60, 132, 228,
    192,   0, 120, 252,
    255, 220, 236, 248
};

DEFINE_GRADIENT_PALETTE(royal_p) {
      0,   0,   0,   0,
     64, 124,   4, 160,
    128, 252, 252, 248,
    192, 248, 244,   0,
    255,   0,   0,   0
};

DEFINE_GRADIENT_PALETTE(spring_gp) {
    0, 255,  0,255,
  128, 255,255,  0,
  255, 255,  0,255
};

DEFINE_GRADIENT_PALETTE(summer_gp) {
    0,   0, 55, 25,
   64,  48,141, 25,
  128, 255,255, 25,
  255,   0, 55, 25
};

DEFINE_GRADIENT_PALETTE(autumn_gp) {
    0, 255,  0,  0,
  128, 255,255,  0,
  255, 255,  0,  0
};

DEFINE_GRADIENT_PALETTE(winter_gp) {
    0,   0,  0,255,
  128,   0,255, 55,
  255,   0,  0,255
};

DEFINE_GRADIENT_PALETTE(hot_gp) {
    0,   1,  0,  0,
   32,  19,  0,  0,
   64, 103,  0,  0,
   97, 255,  1,  0,
  129, 255, 29,  0,
  161, 255,118,  0,
  194, 255,255,  1,
  226, 255,255, 60,
  255, 255,255,255
};

DEFINE_GRADIENT_PALETTE(cool_gp) {
    0,   0,255,255,
  255, 255,  0,255
};

#endif
