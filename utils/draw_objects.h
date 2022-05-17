#ifndef APO_KURVA_DRAW_OBJECTS_H
#define APO_KURVA_DRAW_OBJECTS_H

#include "../consts_structs.h"
#include "buffer.h"
#include "font.h"
#include <math.h>

void drawPlayer(player * p, pixel * colors, pixel * buffer);
void drawCircle(int centerX, int centerY, int r, pixel *p, pixel *buffer);
void drawArena(pixel *buffer);
void drawVictoryScreen(char * message, pixel * buffer);

#endif //APO_KURVA_DRAW_OBJECTS_H
