#ifndef APO_KURVA_FONT_H
#define APO_KURVA_FONT_H

#include <string.h>
#include "buffer.h"
#include "../consts_structs.h"
#include "../lib/font_types.h"

void pchar(char c, unsigned x, unsigned y, pixel * buffer);
void pstring(int x, int y, char * s, pixel * buffer);
void pchar_double(char c, unsigned x, unsigned y, pixel * buffer);
void pstring_double(int x, int y, char * s, pixel * buffer);
void pchar_quadruple(char c, unsigned x, unsigned y, pixel * buffer);
void pstring_quadruple(int x, int y, char * s, pixel * buffer);
int get_font_width(char * s);
int get_double_font_width(char * s);

#endif //APO_KURVA_FONT_H
