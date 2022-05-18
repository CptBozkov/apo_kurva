#ifndef APO_KURVA_DRAW_OBJECTS_H
#define APO_KURVA_DRAW_OBJECTS_H

#include "../consts_structs.h"
#include "buffer.h"
#include "font.h"
#include <math.h>

/*saves player to a buffer*/
void drawPlayer(player * p, pixel * colors, pixel * buffer);

/*saves circle with center at (centerX, centerY), radius r and color *p to a buffer*/
void drawCircle(int centerX, int centerY, int r, pixel *p, pixel *buffer);

/*saves arena to a buffer*/
void drawArena(pixel *buffer);

/*saves victory screen with given message in the middle to a buffer*/
void drawVictoryScreen(char * message, pixel * buffer);

#endif //APO_KURVA_DRAW_OBJECTS_H
