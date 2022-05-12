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

#define ARENA_WALL_WIDTH 5

#define PI 3.14159
#define TURN_SPEED 1

#define FPS 20

#define NUM_OF_COLORS 7
// [0]: red, [1]: orange, [2]: yellow, [3]: green, [4]: blue, [5]: violet, [6]: pink
const uint32_t colors_24bit[] = {0x440000, 0x661100, 0x221100, 0x001100, 0x000525, 0x220033, 0x330011};
const uint16_t colors_16bit[] = {0xF800, 0xFC40, 0xFFC0, 0x07E0, 0x05DF, 0xDC1F, 0xF818};


typedef union led {
    struct {
        uint8_t b, g, r;
    };
    uint32_t d;
} led;

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
    int lives;
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
    int scene;

    bool draw;
    bool doneDraw;

    bool run;

    char * keys;

    bool color;

    bool clear_game_buffer;
} data_passer;


#endif //APO_KURVA_CHANGE_ME_H
