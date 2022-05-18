#ifndef APO_KURVA_FONT_H
#define APO_KURVA_FONT_H

#include <string.h>
#include "buffer.h"
#include "../consts_structs.h"
#include "../lib/font_types.h"

/*saves char c to a buffer at pos (x, y)*/
void pchar(char c, unsigned x, unsigned y, pixel * buffer);

/*saves string s to a buffer at pos (x, y)*/
void pstring(int x, int y, char * s, pixel * buffer);

/*saves char c with double the size to a buffer at pos (x, y)*/
void pchar_double(char c, unsigned x, unsigned y, pixel * buffer);

/*saves string s with double the size to a buffer at pos (x, y)*/
void pstring_double(int x, int y, char * s, pixel * buffer);

/*saves char c with quadruple the size to a buffer at pos (x, y)*/
void pchar_quadruple(char c, unsigned x, unsigned y, pixel * buffer);

/*saves string s with quadruple the size to a buffer at pos (x, y)*/
void pstring_quadruple(int x, int y, char * s, pixel * buffer);

/*returns string width in pixels*/
int get_font_width(char * s);

/*returns string width with double the size in pixels*/
int get_double_font_width(char * s);
#endif //APO_KURVA_FONT_H
