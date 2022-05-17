#include "buffer.h"

void addToBuffer(unsigned x, unsigned y, pixel *p, pixel *buffer) {
    buffer[x + y * SCREEN_SIZE_X] = *p;
}

pixel * getFromBuffer(unsigned x, unsigned y, pixel *buffer) {
    return &buffer[x + y * SCREEN_SIZE_X];
}

void clearBuffer(pixel *buffer) {
    pixel p;
    p.r = 0;
    p.g = 0;
    p.b = 0;
    for (unsigned y = 0; y < SCREEN_SIZE_Y; y++){
        for (unsigned x = 0; x < SCREEN_SIZE_X; x++){
            addToBuffer(x, y, &p, buffer);
        }
    }
}
