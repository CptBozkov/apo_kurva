#include "font.h"

void pchar(char c, unsigned x, unsigned y, pixel * buffer) {
    font_descriptor_t font = font_rom8x16;
    pixel p;
    p.d = 0xFFFF;
    for (unsigned h = 0; h < font.height; h++) {
        for (unsigned w = 0; w < font.maxwidth; w++) {
            if (font.bits[(c - font.firstchar) * font.height + h] & (0x8000>>w)) {
                addToBuffer(x+w, y+h, &p, buffer);
            }
        }
    }
}

void pstring(int x, int y, char * s, pixel * buffer) {
    font_descriptor_t font = font_rom8x16;
    size_t n = strlen(s);
    for (int i = 0; i < n; i ++){
        pchar(s[i], x+i * font.maxwidth, y, buffer);
    }
}

void pchar_double(char c, unsigned x, unsigned y, pixel * buffer) {
    font_descriptor_t font = font_rom8x16;
    pixel p;
    p.d = 0xFFFF;
    for (unsigned h = 0; h < 2*font.height; h += 2) {
        for (unsigned w = 0; w < 2*font.maxwidth; w +=2) {
            if (font.bits[(c - font.firstchar) * font.height + h/2] & (0x8000>>(w/2))) {
                addToBuffer(x+w, y+h, &p, buffer);
                addToBuffer(x+w+1, y+h, &p, buffer);
                addToBuffer(x+w, y+h+1, &p, buffer);
                addToBuffer(x+w+1, y+h+1, &p, buffer);
            }
        }
    }
}

void pstring_double(int x, int y, char * s, pixel * buffer) {
    font_descriptor_t font = font_rom8x16;
    size_t n = strlen(s);
    for (int i = 0; i < n; i++){
        pchar_double(s[i], x + i * 2*font.maxwidth, y, buffer);
    }
}

void pchar_quadruple(char c, unsigned x, unsigned y, pixel * buffer) {
    font_descriptor_t font = font_rom8x16;
    pixel p;
    p.d = 0xFFFF;
    for (unsigned h = 0; h < 4*font.height; h += 4) {
        for (unsigned w = 0; w < 4*font.maxwidth; w +=4) {
            if (font.bits[(c - font.firstchar) * font.height + h/4] & (0x8000>>(w/4))) {
                for (int i = 0; i < 4; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        addToBuffer(x+w+j, y+h+i, &p, buffer);
                    }
                }
            }
        }
    }
}

void pstring_quadruple(int x, int y, char * s, pixel * buffer) {
    font_descriptor_t font = font_rom8x16;
    size_t n = strlen(s);
    for (int i = 0; i < n; i++){
        pchar_quadruple(s[i], x + i * 4*font.maxwidth, y, buffer);
    }
}

int get_font_width(char * s) {
    font_descriptor_t font = font_rom8x16;
    size_t n = strlen(s);
    return n * font.maxwidth;
}

int get_double_font_width(char * s) {
    font_descriptor_t font = font_rom8x16;
    size_t n = strlen(s);
    return n * 2*font.maxwidth;
}
