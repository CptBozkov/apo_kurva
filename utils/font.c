#include "font.h"
#include "buffer.h"

void pchar(char c, unsigned x, unsigned y, font_descriptor_t font, pixel * buffer) {
    pixel p;
    p.d = 0xFFFF;
    //printf("%d %d\n", font.maxwidth, font.height);
    //fflush(stdout);
    for (unsigned h = 0; h < font.height; h++) {
        for (unsigned w = 0; w < font.maxwidth; w++) {
            if (font.bits[(c - font.firstchar) * font.height + h] & (0x8000>>w)) {
                addToBuffer(x+w, y+h, &p, buffer);
            }
        }
    }
}

void pstring(int x, int y, char * s, int n, font_descriptor_t font, pixel * buffer) {
    for (int i = 0; i < n; i ++){
        pchar(s[i], x+i * font.maxwidth, y, font, buffer);
    }
}