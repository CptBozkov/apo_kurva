#ifndef APO_KURVA_CONSTS_STRUCTS_H
#define APO_KURVA_CONSTS_STRUCTS_H

#include <stdint.h>
#include <stdbool.h>

/* --- constants --- */
#define _POSIX_C_SOURCE 200112L

#define SCREEN_SIZE_X 480
#define SCREEN_SIZE_Y 320
#define FPS 20

#define BASE_PLAYER_SPEED 50
#define BASE_PLAYER_WIDTH 5
#define MAX_LIVES 4
#define TURN_SPEED 1
#define PI 3.14159

#define ARENA_WALL_WIDTH 5

#define LEDLINE_COUNT 32
#define NUM_OF_COLORS 8

/* --- unions && structs --- */

/*struct that stores colors in 24-bit format*/
typedef union led {
    struct {
        uint8_t b, g, r;
    };
    uint32_t d;
} led;

/*union that stores colors in 16-bit format*/
typedef union pixel {
    struct {
        unsigned b : 5;
        unsigned g : 6;
        unsigned r : 5;
    };
    uint16_t d;
} pixel;

/*union that stores knobs input*/
typedef union knobs {
    struct {
        uint8_t b, g, r;
        unsigned b_p : 1;
        unsigned g_p : 1;
        unsigned r_p : 1;
    };
    uint32_t d;
} knobs;

/*union used to pass around knobs values*/
typedef union knobs_values {
    struct{
        unsigned b_p : 1;
        unsigned g_p : 1;
        unsigned r_p : 1;
        uint8_t b, g, r;
        unsigned b_s : 1;
        unsigned g_s : 1;
        unsigned r_s : 1;
    };
    uint64_t d;
} knobs_values;

/*struct that stores players data*/
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
    int color;
    float last_x;
    float last_y;
} player;

/*struct that is used to pass data between threads throughout the game*/
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

    bool pause;
    int pause_length;
} data_passer;

#endif //APO_KURVA_CONSTS_STRUCTS_H
