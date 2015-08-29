#ifndef __INC_TOY_PALETTES_H
#define __INC_TOY_PALETTES_H

CRGBPalette16 modifiedRainbow_p = CRGBPalette16(
    CRGB::Red, CRGB::OrangeRed, CRGB::Orange, CRGB::Gold,
    CRGB::Yellow, CRGB::GreenYellow, CRGB::Lime, CRGB::Cyan,
    CRGB::DeepSkyBlue, CRGB::Blue, CRGB::Navy, CRGB::Indigo,
    CRGB::Purple, CRGB::Violet, CRGB::Magenta, CRGB::DeepPink);

DEFINE_GRADIENT_PALETTE(coldFire_p) {
      0,   0, 172, 252,
     16,   0, 124, 252,
     32,   0,  68, 252,
     48,   0,   4, 252,
     64,  32,  16, 212,
     80,  64,  32, 172,
     96,  96,  48, 128,
    112, 128,  64,  88,
    128, 160,  80,  48,
    144, 196,  96,   4,
    160, 208, 132,   4,
    176, 224, 172,   4,
    192, 236, 212,   4,
    208, 252, 252,   0,
    224, 252, 252, 152,
    240, 252, 252, 200,
    255, 252, 252, 252
};

#endif
