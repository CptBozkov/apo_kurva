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
#include <math.h>
#include <pthread.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#define SCREEN_SIZE_X 480
#define SCREEN_SIZE_Y 320

union led {
    struct {
        uint8_t b, g, r;
    };
    uint32_t d;
};

typedef union pixel {
    struct {
        unsigned b : 5;
        unsigned g : 6;
        unsigned r : 5;
    };
    uint16_t d;
} pixel;



void addToBuffer(int x, int y, pixel *p, pixel *buffer){
    buffer[x + y * SCREEN_SIZE_X] = *p;
}

void drawCircle(int centerX, int centerY, int r, unsigned red, unsigned green, unsigned blue, pixel *buffer){
    pixel *p = malloc(sizeof(pixel));
    p->r = red;
    p->g = green;
    p->b = blue;
    for (int y = -r; y <= r; y ++){
        for (int x = -r; x <= r; x ++){
            if (sqrt(x*x + y*y) <= r){
                addToBuffer(centerX + x, centerY + y, p, buffer);
            }
        }
    }
    free(p);
}

void updateDisplay(pixel *buffer){
    volatile void *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    parlcd_write_cmd(parlcd_reg_base, 0x2c);
    for (unsigned i = 0; i <480*320; i++){
        parlcd_write_data(parlcd_reg_base, buffer[i].d);
    }
}

void *myThreadFun(void *vargp)
{
    sleep(5);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}


int main(int argc, char *argv[])
{
    volatile void *spiled_reg_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    volatile uint32_t *ledline = (spiled_reg_base + SPILED_REG_LED_LINE_o);
    volatile uint32_t *rgb1 = (spiled_reg_base + SPILED_REG_LED_RGB1_o);

    pixel *buffer = malloc(sizeof(pixel) * SCREEN_SIZE_X * SCREEN_SIZE_Y);
    if (!buffer){
        return 1;
    }


    *ledline = 0x80000001;
    *rgb1 = ((union led){.r = 0x10, .g = 0x10, .b = 0x10}).d;






    for (unsigned y = 0; y < SCREEN_SIZE_Y; y++){
        for (unsigned x = 0; x < SCREEN_SIZE_X; x++){
            pixel *p = malloc(sizeof(pixel));
            p->r = 0x00;
            p->g = 0x00;
            p->b = 0x00;
            addToBuffer(x, y, p, buffer);
        }
    }

    updateDisplay(buffer);
    drawCircle(20, 20, 10, 0x00, 0xff, 0x00, buffer);

    updateDisplay(buffer);

    pthread_t thread_id;
    printf("Before Thread\n");
    pthread_create(&thread_id, NULL, myThreadFun, NULL);
    pthread_join(thread_id, NULL);
    printf("After Thread\n");

    drawCircle(50, 50, 10, 0xff, 0xff, 0xff, buffer);
    drawCircle(50, 80, 10, 0xff, 0xff, 0x00, buffer);

    updateDisplay(buffer);
    free(buffer);
    return 0;
}
