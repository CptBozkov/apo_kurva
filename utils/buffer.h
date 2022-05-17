#ifndef APO_KURVA_BUFFER_H
#define APO_KURVA_BUFFER_H

#include "../consts_structs.h"

void addToBuffer(unsigned x, unsigned y, pixel *p, pixel *buffer);
pixel * getFromBuffer(unsigned x, unsigned y, pixel *buffer);
void clearBuffer(pixel *buffer);

#endif //APO_KURVA_BUFFER_H
