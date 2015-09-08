#ifndef __INC_TOY_PALETTES_H
#define __INC_TOY_PALETTES_H

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

DEFINE_GRADIENT_PALETTE(achilles_p) {
      0,  76, 128,  72,
     32, 232, 132,  76,
     64, 236,  60,  80,
     96, 136,  92,  68,
    128, 108,  60, 188,
    160, 192, 184, 192,
    192, 212, 204,  64,
    224, 140, 188, 100,
    255, 160, 200, 216
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

DEFINE_GRADIENT_PALETTE(kelvino_p) {
      0, 247, 252, 240,
     64, 168, 221, 181,
    128,  78, 179, 211,
    192,   8, 104, 172,
    255,   8,  64, 129
};

// Gradient palette "spring_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/spring.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE(spring_gp) {
    0, 255,  0,255,
   68, 255, 13,115,
  135, 255, 62, 37,
  204, 255,156,  4,
  255, 255,255,  0
};

// Gradient palette "summer_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/summer.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE(summer_gp) {
    0,   0, 55, 25,
   68,   8, 92, 25,
  135,  48,141, 25,
  204, 142,201, 25,
  255, 255,255, 25
};

// Gradient palette "autumn_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/autumn.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE(autumn_gp) {
    0, 255,  0,  0,
  255, 255,255,  0
};

// Gradient palette "winter_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/winter.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE(winter_gp) {
    0,   0,  0,255,
  255,   0,255, 44
};

// Gradient palette "hot_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/hot.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

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

// Gradient palette "cool_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/cool.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE(cool_gp) {
    0,   0,255,255,
  255, 255,  0,255
};

#endif
