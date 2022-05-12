/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  change_me.c      - main file

  include your name there and license for distribution.

  Remove next text: This line should not appear in submitted
  work and project name should be change to match real application.
  If this text is there I want 10 points subtracted from final
  evaluation.

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "../lib/mzapo_parlcd.h"
#include "../lib/mzapo_phys.h"
#include "../lib/mzapo_regs.h"

union led {
    struct {
        uint8_t b,g,r;
    };
    uint32_t data;
};

typedef union pixel {
    struct {
        unsigned b : 5;
        unsigned g : 6;
        unsigned r : 5;
    };
    uint16_t d;
} pixel;


// font
  font_descriptor_t font = font_rom8x16;

void pchar(char c, unsigned x, unsigned y) {
    for (unsigned w = 0; w < font.maxwidth; w++) {
        for (unsigned h = 0; h < font.height; h++) {
            if (font.bits[(c - font.firstchar) * font.height + h] & (1<<w))) {
                buffer[x + w][y + h] =  (union pixel){ .b = 0x1f };
            }
        }
    }
}


  // end of font


int main(int argc, char *argv[])
{
  printf("Hello world\n");

  volatile void *spiled_reg_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  volatile uint32_t *ledline = (spiled_reg_base + SPILED_REG_LED_LINE_o);
  *ledline = 222;

  volatile uint32_t *rgb1 = (spiled_reg_base + SPILED_REG_LED_RGB1_o);
  *rgb1 = ((union led){ .r = 80, .g = 100, .b = 222 }).data;

  unsigned char *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  parlcd_write_cmd(parlcd_reg_base, 0x2c);

/*
  for (unsigned i = 0; i < 480*320; i++) {
    parlcd_write_data(parlcd_reg_base, ((union pixel){ .r = 0x1f }).d);
  }
*/

  union pixel buffer[480][320];
  for (unsigned x = 0; x < 480; x++){
    for (unsigned y = 0; y < 320; y++) {
        buffer[x][y].d = 0xffff;
    }
  }
  for (unsigned i = 0; i < 100; i++) {
    buffer[20][i+20] = (union pixel){ .b = 0x1f };
    buffer[i+20][20] = (union pixel){ .b = 0x1f };
    buffer[120][i+20] = (union pixel){ .b = 0x1f };
    buffer[i+20][120] = (union pixel){ .b = 0x1f };
  }

  for (unsigned y = 0; y < 320; y++){
    for (unsigned x = 0; x < 480; x++) {
        parlcd_write_data(parlcd_reg_base, buffer[x][y].d);
    }
  }




  sleep(4);

  printf("Goodbye world\n");

  return 0;
}
