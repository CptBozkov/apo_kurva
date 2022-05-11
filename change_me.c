#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include </usr/include/libusb-1.0/libusb.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#define SCREEN_SIZE_X 480
#define SCREEN_SIZE_Y 320

#define BASE_PLAYER_SPEED 2

#define FPS 60

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

typedef union knobs {
    struct {
        uint8_t b, g, r;
        unsigned b_p : 1;
        unsigned g_p : 1;
        unsigned r_p : 1;
    };
    uint32_t d;
} knobs;

typedef struct player {
    float x;
    float y;
    float speed;
    int d_x;
    int d_y;
    pixel color;
} player;

// tenhle struct bude slouzit k prenosu dat mezi thready
// pri volani funkce na spusteni thread mu muzes predat jeden void pointer (v tomhle pripade jsem tam dal struct data_passer)
typedef struct data_passer{
    pixel *game_buffer;
    pixel *menu_buffer;
    int drawnBuffer;

    bool draw;
    bool doneDraw;

    bool run;
} data_passer;


void addToBuffer(unsigned x, unsigned y, pixel *p, pixel *buffer){
    buffer[x + y * SCREEN_SIZE_X] = *p;
}

void drawCircle(int centerX, int centerY, int r, pixel *p, pixel *buffer){
    for (int y = -r; y <= r; y ++){
        for (int x = -r; x <= r; x ++){
            if (centerX+x >= 0 && centerX+x < SCREEN_SIZE_X && centerY+y >= 0 && centerY+y < SCREEN_SIZE_Y){
                if (sqrt(x*x + y*y) <= r){
                    addToBuffer(centerX+x, centerY+y, p, buffer);
                }
            }
        }
    }
}


void clearBuffer(pixel *buffer) {
    pixel p;
    p.r = 31;
    p.g = 63;
    p.b = 31;
    for (unsigned y = 0; y < SCREEN_SIZE_Y; y++){
        for (unsigned x = 0; x < SCREEN_SIZE_X; x++){
            addToBuffer(x, y, &p, buffer);
        }
    }
}

pixel * createPixel(int r, int b, int g){
    pixel * p = malloc(sizeof(pixel));
    p->r = r;
    p->g = g;
    p->b = b;
    return p;
}

player * createPlayer(int x, int y, pixel * color){
    player * p = malloc(sizeof(player));
    p->x = x;
    p->y = y;
    p->d_x = 1;
    p->d_y = 1;
    p->speed = BASE_PLAYER_SPEED;
    p->color = *color;
    return p;
}

data_passer * createDataPasser(){
    data_passer * dp = malloc(sizeof(data_passer));
    dp->game_buffer = malloc(SCREEN_SIZE_X*SCREEN_SIZE_Y*sizeof(pixel));
    dp->menu_buffer = malloc(SCREEN_SIZE_X*SCREEN_SIZE_Y*sizeof(pixel));
    dp->drawnBuffer = 1;
    dp->draw = false;
    dp->doneDraw = true;
    dp->run = true;
    return dp;
}


void updatePlayer(player * p){
    if (p->x > SCREEN_SIZE_X || p->x < 0){
        p->d_x *= -1;
    }
    if (p->y > SCREEN_SIZE_Y || p->y < 0){
        p->d_y *= -1;
    }
    p->x += p->d_x * p->speed;
    p->y += p->d_y * p->speed;
}

void drawPlayer(player * p, pixel * buffer){
    drawCircle((int) p->x, (int) p->y, 10, &p->color, buffer);
}

void gameLoop(data_passer * dp, struct timespec *start, struct timespec *end, struct timespec *res){
    void *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    volatile void *spiled_reg_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    parlcd_write_cmd(parlcd_reg_base, 0x2c);
    volatile uint32_t *ledline = (spiled_reg_base + SPILED_REG_LED_LINE_o);
    *ledline = 0x80000001;
    volatile uint32_t *rgb1 = (spiled_reg_base + SPILED_REG_LED_RGB1_o);
    *rgb1 = ((union led){.r = 0x10, .g = 0x10, .b = 0x10}).d;
    volatile uint32_t *knobs_input = (spiled_reg_base + SPILED_REG_KNOBS_8BIT_o);
    knobs k;



    pixel * b;

    pixel * pixel = createPixel(0xff, 0xff, 0x00);

    player * player1 = createPlayer(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2, createPixel(0xff, 0xff, 0x00));
    player * player2 = createPlayer(0, SCREEN_SIZE_Y/2, createPixel(0xff, 0x00, 0x00));


    clearBuffer(dp->game_buffer);
    clearBuffer(dp->menu_buffer);

    while (dp->run){
        if (dp->doneDraw) {
            clock_gettime(CLOCK_MONOTONIC, start);

            // sem prijde celej nas main loop
            dp->doneDraw = false;
            dp->draw = true;

            b = dp->game_buffer;
            /*dp->drawnBuffer *= -1;
            if (dp->drawnBuffer == 1) {
                b = dp->game_buffer;
            } else {
                b = dp->menu_buffer;
            }*/

            k.d = *knobs_input;
            pixel->r = k.r;
            pixel->g = k.g;
            pixel->b = k.b;

            /*if (k.r_p){
                speed += 1;
            }
            if (k.b_p){
                speed -= 1;
            }*/

            updatePlayer(player1);
            updatePlayer(player2);
            drawPlayer(player1, b);
            drawPlayer(player2, b);


            // tady konci nas main loop


            clock_gettime(CLOCK_MONOTONIC, end);
            res->tv_nsec = 1000000000/FPS - (end->tv_nsec - start->tv_nsec);
            nanosleep(res, NULL);
        }
    }
    // do dp vidi oba thready (to je ten voio pointer na struct)
    // oba while cykly se timto zastavi a program se vypne
    // dp->run = 0; pak muzem nabindovat treba na esc
    dp->run = false;
}

void draw(data_passer * dp){
    void *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

    pixel * b;

    while (dp->run){
        if (dp->draw){
            dp->draw = false;

            b = dp->game_buffer;
            /*if (dp->drawnBuffer == 1) {
                b = dp->menu_buffer;
            } else {
                b = dp->game_buffer;
            }*/

            for (unsigned i = 0; i <480*320; i++){
                parlcd_write_data(parlcd_reg_base, b[i].d);
            }
            dp->doneDraw = true;
        }
    }
}

// funkce kterou predavam gameLoopThread
void* gameLoopThread(void * dp_void){
    data_passer * dp = (data_passer *) dp_void;
    struct timespec start = {0,0};
    struct timespec end = {0,0};
    struct timespec res;
    gameLoop(dp, &start, &end, &res);
    return NULL;
}

// funkce kterou predavam drawThread
void* drawThread(void * dp_void){
    data_passer * dp = (data_passer *) dp_void;
    draw(dp);
    return NULL;
}

int main (){
    data_passer * dp = createDataPasser();

    pthread_t tid0;
    pthread_t tid1;

    pthread_create(&tid0, NULL, gameLoopThread, (void *)dp);
    pthread_create(&tid1, NULL, drawThread, (void *)dp);

    pthread_exit(NULL);
    return 0;
}
