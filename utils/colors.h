#ifndef APO_KURVA_COLORS_H
#define APO_KURVA_COLORS_H

#include "../consts_structs.h"

/*returns pointer to a new pixel with given color components*/
pixel * createPixel(int r, int g, int b);

/*returns pointer to a new pixel with given 16-bit color code*/
pixel * createPixelHex(uint16_t data);

/*generates pixel array for preselected colors
 * those are colors from which players can choose*/
pixel * generate_colors_p();

/*generates led array for preselected colors
 * the same colors but saved in different format specifically for rgb LEDs*/
led * generate_colors_l();

#endif //APO_KURVA_COLORS_H
