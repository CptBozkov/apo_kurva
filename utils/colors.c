#include "colors.h"
#include <stdlib.h>

// [0]: red, [1]: orange, [2]: yellow, [3]: green, [4]: blue, [5]: violet, [6]: pink, [7]: cyan
const uint32_t colors_24bit[] = {0x440000, 0x661100, 0x221100, 0x001100, 0x000525, 0x220033, 0x330011, 0x002222};
const uint16_t colors_16bit[] = {0xF800, 0xFC40, 0xFFC0, 0x07E0, 0x05DF, 0xDC1F, 0xF818, 0x475F};

pixel * createPixel(int r, int g, int b) {
    pixel * p = malloc(sizeof(pixel));
    p->r = r;
    p->g = g;
    p->b = b;
    return p;
}

pixel * createPixelHex(uint16_t data) {
    pixel * p = malloc(sizeof(pixel));
    p->d = data;
    return p;
}

pixel * generate_colors_p() {
    pixel *pixels = malloc(NUM_OF_COLORS * sizeof(pixel));
    for (int i = 0; i < NUM_OF_COLORS; ++i) {
        pixel p;
        p.d = colors_16bit[i];
        pixels[i] = p;
    }
    return pixels;
}

led * generate_colors_l() {
    led *leds = malloc(NUM_OF_COLORS * sizeof(led));
    for (int i = 0; i < NUM_OF_COLORS; ++i) {
        led l;
        l.d = colors_24bit[i];
        leds[i] = l;
    }
    return leds;
}