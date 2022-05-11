#ifndef APO_KURVA_CHANGE_ME_H
#define APO_KURVA_CHANGE_ME_H

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
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>

#include "lib/mzapo_parlcd.h"
#include "lib/mzapo_phys.h"
#include "lib/mzapo_regs.h"

#define SCREEN_SIZE_X 480
#define SCREEN_SIZE_Y 320

#define BASE_PLAYER_SPEED 50
#define BASE_PLAYER_WIDTH 5
#define MAX_LIVES 10

#define PI 3.14159
#define TURN_SPEED 50
#define TURN_PARTS 10

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
    char id;
    char lives;
    float x;
    float y;
    float speed;
    int width;
    int d_x;
    int d_y;
    float rotation;
    pixel color;
    float last_x;
    float last_y;
} player;

typedef struct data_passer{
    pixel *game_buffer;
    pixel *menu_buffer;
    int drawnBuffer;

    bool draw;
    bool doneDraw;

    bool run;

    char * keys;

    bool color;

    bool clear_game_buffer;
} data_passer;


#endif //APO_KURVA_CHANGE_ME_H
