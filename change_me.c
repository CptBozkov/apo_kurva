#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
//#include <libusb-1.0/libusb.h>  klavesnice

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#define SCREEN_SIZE_X 480
#define SCREEN_SIZE_Y 320

#define FPS 30

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

// tenhle struct bude slouzit k prenosu dat mezi thready
// pri volani funkce na spusteni thread mu muzes predat jeden void pointer (v tomhle pripade jsem tam dal struct data_passer)
typedef struct data_passer{
    pixel *buffer1;
    pixel *buffer2;

    int drawnBuffer;
    int draw;
    int doneDraw;

    int run;
} data_passer_t;


void addToBuffer(unsigned x, unsigned y, pixel p, pixel *buffer){
    buffer[x + y * SCREEN_SIZE_X] = p;
}

void drawCircle(int centerX, int centerY, int r, pixel p, pixel *buffer){
    for (int y = -r; y <= r; y ++){
        for (int x = -r; x <= r; x ++){
            if (sqrt(x*x + y*y) <= r){
                addToBuffer(centerX + x, centerY + y, p, buffer);
            }
        }
    }
}


void clearBuffer(pixel *buffer) {
    pixel p;
    p.r = 0xff;
    p.g = 0xff;
    p.b = 0xff;
    for (unsigned y = 0; y < SCREEN_SIZE_Y; y++){
        for (unsigned x = 0; x < SCREEN_SIZE_X; x++){
            addToBuffer(x, y, p, buffer);
        }
    }
}


void gameLoop(data_passer_t * dp, struct timespec *start, struct timespec *end, struct timespec *res){
    // az tady nadefinujem vsechny promenny

    volatile void *spiled_reg_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    volatile uint32_t *ledline = (spiled_reg_base + SPILED_REG_LED_LINE_o);
    volatile uint32_t *rgb1 = (spiled_reg_base + SPILED_REG_LED_RGB1_o);


/* ledky, rgb ledka
    *ledline = 0x80000001;
    *rgb1 = ((union led){.r = 0x10, .g = 0x10, .b = 0x10}).d;
*/

    volatile void *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    parlcd_write_cmd(parlcd_reg_base, 0x2c);

    pixel p;
    p.r = 0xff;
    p.g = 0xff;
    p.b = 0x00;


    float x = SCREEN_SIZE_X;
    float y = SCREEN_SIZE_Y;
    float d_x = 1;
    float d_y = 1;

    pixel * b;

    clearBuffer(dp->buffer1);
    clearBuffer(dp->buffer2);

    while (dp->run){
        if (dp->doneDraw) {
            clock_gettime(CLOCK_MONOTONIC, start);

            // sem prijde celej nas main loop
            //ty funkce nad a po printem drzej konstantni FPS
            dp->doneDraw = 0;
            dp->drawnBuffer *= -1;
            dp->draw = 1;

            if (dp->drawnBuffer == 1) {
                b = dp->buffer1;
            } else {
                b = dp->buffer2;
            }

            clearBuffer(b);
            drawCircle((int)x, (int)y, 20, p, b);

            if (x > SCREEN_SIZE_X || x < 0){
                d_x *= -1;
            }
            if (y > SCREEN_SIZE_Y || y < 0){
                d_y *= -1;
            }

            x += d_x;
            y += d_y;
            // tady konci nas main loop

            clock_gettime(CLOCK_MONOTONIC, end);
            res->tv_nsec = 1000000000/FPS - (end->tv_nsec - start->tv_nsec);
            nanosleep(res, NULL);
        }
    }
    // do dp vidi oba thready (to je ten voio pointer na struct)
    // oba while cykly se timto zastavi a program se vypne
    // dp->run = 0; pak muzem nabindovat treba na esc
    dp->run = 0;
}

void draw(data_passer_t * dp){
    volatile void *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

    pixel * b;

    while (dp->run){
        if (dp->draw){
            dp->draw = 0;
            if (dp->drawnBuffer == 1) {
                b = dp->buffer2;
            } else {
                b = dp->buffer1;
            }
            for (unsigned i = 0; i <480*320; i++){
                parlcd_write_data(parlcd_reg_base, b[i].d);
            }
            dp->doneDraw = 1;
        }
    }
}

// funkce kterou predavam gameLoopThread
void* gameLoopThread(void * dp_void){
    data_passer_t * dp = (data_passer_t *) dp_void;
    struct timespec start = {0,0};
    struct timespec end = {0,0};
    struct timespec res;
    gameLoop(dp, &start, &end, &res);
    return NULL;
}

// funkce kterou predavam drawThread
void* drawThread(void * dp_void){
    data_passer_t * dp = (data_passer_t *) dp_void;
    draw(dp);
    return NULL;
}

int main (){
    data_passer_t * dp = malloc(sizeof(data_passer_t));
    dp->buffer1 = malloc(SCREEN_SIZE_X*SCREEN_SIZE_Y*sizeof(pixel));
    dp->buffer2 = malloc(SCREEN_SIZE_X*SCREEN_SIZE_Y*sizeof(pixel));

    dp->drawnBuffer = 1;
    dp->draw = 0;
    dp->doneDraw = 0;

    dp->run = 1;

    pthread_t tid0;
    pthread_t tid1;

    // musime pointery pretypovat na void a pak zas zpatky na data_passer_t
    pthread_create(&tid0, NULL, gameLoopThread, (void *)dp);
    pthread_create(&tid1, NULL, drawThread, (void *)dp);

    pthread_exit(NULL);
    return 0;
}
