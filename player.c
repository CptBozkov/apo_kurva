#include "player.h"

player * createPlayer(int color, char id) {
    player * p = malloc(sizeof(player));
    p->id = id;

    if (id == 0){
        p->x = SCREEN_SIZE_X/8 + rand() % (SCREEN_SIZE_X/4);
        p->y = SCREEN_SIZE_Y/4 + rand() % (SCREEN_SIZE_Y/2);
    } else {
        p->x = 5*SCREEN_SIZE_X/8 + rand() % (SCREEN_SIZE_X/4);
        p->y = SCREEN_SIZE_Y/4 + rand() % (SCREEN_SIZE_Y/2);
    }

    p->width = BASE_PLAYER_WIDTH;
    p->rotation = ((float)rand()/RAND_MAX)*(3.14159*2);
    p->d_x = 1;
    p->d_y = 1;
    p->speed = BASE_PLAYER_SPEED;
    p->color = color;
    p->last_x = -100;
    p->last_y = -100;
    p->lives = MAX_LIVES;
    return p;
}

void updatePlayer(player * p, data_passer * dp) {
    if (p->x > SCREEN_SIZE_X || p->x < 0){
        p->d_x *= -1;
    }
    if (p->y > SCREEN_SIZE_Y || p->y < 0){
        p->d_y *= -1;
    }
    float turn_speed = PI*TURN_SPEED;
    if (p->id == 0){
        if (dp->keys[0]){
            p->rotation -= turn_speed * (1.0/FPS);
        }
        if (dp->keys[1]){
            p->speed = BASE_PLAYER_SPEED * 2;
        } else {
            p->speed = BASE_PLAYER_SPEED;
        }
        if (dp->keys[2]){
            p->rotation += turn_speed * (1.0/FPS);
        }
    } else {
        if (dp->keys[3]){
            p->rotation -= turn_speed * (1.0/FPS);
        }
        if (dp->keys[4]){
            p->speed = BASE_PLAYER_SPEED * 2;
        } else {
            p->speed = BASE_PLAYER_SPEED;
        }
        if (dp->keys[5]){
            p->rotation += turn_speed * (1.0/FPS);
        }
    }

    p->x += p->d_x * p->speed * cos(p->rotation) * (1.0/FPS);
    p->y += p->d_y * p->speed * sin(p->rotation) * (1.0/FPS);
}

int detectCollision(player * player, pixel * buffer) {
    for (int y = -player->width; y <= player->width; y ++){
        for (int x = -player->width; x <= player->width; x ++){
            if (player->x+x >= 0 && player->x+x < SCREEN_SIZE_X && player->y+y >= 0 && player->y+y < SCREEN_SIZE_Y){
                if (sqrt(x*x + y*y) <= player->width){
                    int d_x = player->last_x-(player->x+x);
                    int d_y = player->last_y-(player->y+y);
                    if (sqrt(d_x*d_x + d_y*d_y) > player->width){
                        fflush(stdout);
                        pixel * p = getFromBuffer(player->x+x, player->y+y, buffer);
                        if ((p->r != 0 || p->g != 0 || p->b != 0)){
                            player->last_x = player->x;
                            player->last_y = player->y;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    player->last_x = player->x;
    player->last_y = player->y;
    return 0;
}

void resetPlayer(player * p) {
    if (p->id == 0){
        p->x = SCREEN_SIZE_X/8 + rand() % (SCREEN_SIZE_X/4);
        p->y = SCREEN_SIZE_Y/4 + rand() % (SCREEN_SIZE_Y/2);
    } else {
        p->x = 5*SCREEN_SIZE_X/8 + rand() % (SCREEN_SIZE_X/4);
        p->y = SCREEN_SIZE_Y/4 + rand() % (SCREEN_SIZE_Y/2);
    }
    p->rotation = ((float)rand()/RAND_MAX)*(3.14159*2);
    p->last_x = -100;
    p->last_y = -100;
}

void selectColor(pixel * colors, player * p1, player * p2, pixel * buffer, knobs_values * k, knobs_values * last_k) {
    if (k->r_s){
        if (last_k->r/4 > k->r/4){
            p1->color ++;
            p1->color = p1->color % NUM_OF_COLORS;
            if (p1->color == p2->color){
                p1->color ++;
            }
            p1->color = p1->color % NUM_OF_COLORS;
        } else if (last_k->r/4 < k->r/4){
            p1->color --;
            p1->color = (p1->color + NUM_OF_COLORS) % NUM_OF_COLORS;
            if (p1->color == p2->color){
                p1->color --;
            }
            p1->color = (p1->color + NUM_OF_COLORS) % NUM_OF_COLORS;
        }
    }
    if (k->b_s){
        if (last_k->b/4 > k->b/4){
            p2->color ++;
            p2->color = p2->color % NUM_OF_COLORS;
            if (p1->color == p2->color){
                p2->color ++;
            }
            p2->color = p2->color % NUM_OF_COLORS;
        } else if (last_k->b/4 < k->b/4){
            p2->color --;
            p2->color = (p2->color + NUM_OF_COLORS) % NUM_OF_COLORS;
            if (p1->color == p2->color){
                p2->color --;
            }
            p2->color = (p2->color + NUM_OF_COLORS) % NUM_OF_COLORS;
        }
    }
    last_k->d = k->d;
}
