#ifndef APO_KURVA_BUFFER_H
#define APO_KURVA_BUFFER_H

#include "../consts_structs.h"

/*sets given pixels value to a pixel at pos (x, y) in a buffer*/
void addToBuffer(unsigned x, unsigned y, pixel *p, pixel *buffer);

/*returns pointer to a pixel that is saved at position (x, y) in buffer*/
pixel * getFromBuffer(unsigned x, unsigned y, pixel *buffer);

/*sets all pixels in a buffer to a black color*/
void clearBuffer(pixel *buffer);

#endif //APO_KURVA_BUFFER_H
