#ifndef APO_KURVA_COLORS_H
#define APO_KURVA_COLORS_H

#include "../consts_structs.h"

pixel * createPixel(int r, int g, int b);
pixel * createPixelHex(uint16_t data);
pixel * generate_colors_p();
led * generate_colors_l();

#endif //APO_KURVA_COLORS_H
