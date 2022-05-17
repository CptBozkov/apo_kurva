#include "draw_objects.h"

void drawPlayer(player * p, pixel * colors, pixel * buffer) {
    drawCircle((int) p->x, (int) p->y, p->width, &colors[p->color], buffer);
}

void drawCircle(int centerX, int centerY, int r, pixel *p, pixel *buffer) {
    for (int y = -r; y <= r; y ++){
        for (int x = -r; x <= r; x ++){
            if (centerX+x >= 0 && centerX+x < SCREEN_SIZE_X && centerY+y >= 0 && centerY+y < SCREEN_SIZE_Y){
                if (sqrt(x*x + y*y) <= r){
                    addToBuffer(centerX+x, centerY+y, p, buffer);
                }
            }
        }
    }
}

void drawArena(pixel *buffer) {
    pixel p;
    p.r = 31;
    p.g = 63;
    p.b = 31;
    for (unsigned y = 0; y < SCREEN_SIZE_Y; y++){
        for (unsigned x = 0; x < SCREEN_SIZE_X; x++){
            if (x < ARENA_WALL_WIDTH || x > SCREEN_SIZE_X - ARENA_WALL_WIDTH || y < ARENA_WALL_WIDTH || y > SCREEN_SIZE_Y - ARENA_WALL_WIDTH){
                addToBuffer(x, y, &p, buffer);
            }
        }
    }
}
