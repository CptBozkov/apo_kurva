#ifndef APO_KURVA_FONT_H
#define APO_KURVA_FONT_H

#include "../consts_structs.h"
#include "../lib/font_types.h"

void pchar(char c, unsigned x, unsigned y, font_descriptor_t font, pixel * buffer);
void pstring(int x, int y, char * s, int n, font_descriptor_t font, pixel * buffer);

#endif //APO_KURVA_FONT_H
