#include "change_me.h"

void addToBuffer(unsigned x, unsigned y, pixel *p, pixel *buffer){
    buffer[x + y * SCREEN_SIZE_X] = *p;
}

pixel * getFromBuffer(unsigned x, unsigned y, pixel *buffer){
    return &buffer[x + y * SCREEN_SIZE_X];
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
    p.r = 0;
    p.g = 0;
    p.b = 0;
    for (unsigned y = 0; y < SCREEN_SIZE_Y; y++){
        for (unsigned x = 0; x < SCREEN_SIZE_X; x++){
            addToBuffer(x, y, &p, buffer);
        }
    }
}
void drawArena(pixel *buffer) {
    pixel p;
    p.r = 0xff;
    p.g = 0xff;
    p.b = 0xff;
    for (unsigned y = 0; y < SCREEN_SIZE_Y; y++){
        for (unsigned x = 0; x < SCREEN_SIZE_X; x++){
            if (x < ARENA_WALL_WIDTH || x > SCREEN_SIZE_X - ARENA_WALL_WIDTH && x || y < ARENA_WALL_WIDTH || y > SCREEN_SIZE_Y - ARENA_WALL_WIDTH){
                addToBuffer(x, y, &p, buffer);
            }
        }
    }
}

pixel * createPixel(int r, int g, int b){
    pixel * p = malloc(sizeof(pixel));
    p->r = r;
    p->g = g;
    p->b = b;
    return p;
}

player * createPlayer(pixel * color, char id){
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
    p->rotation = (float)rand()/(float)(RAND_MAX/3.14*2);
    p->d_x = 1;
    p->d_y = 1;
    p->speed = BASE_PLAYER_SPEED;
    p->color = *color;
    p->last_x = -100;
    p->last_y = -100;
    p->lives = MAX_LIVES;
    return p;
}

data_passer * createDataPasser(){
    data_passer * dp = malloc(sizeof(data_passer));
    dp->game_buffer = malloc(SCREEN_SIZE_X*SCREEN_SIZE_Y*sizeof(pixel));
    dp->menu_buffer = malloc(SCREEN_SIZE_X*SCREEN_SIZE_Y*sizeof(pixel));
    dp->scene = 1;
    dp->draw = false;
    dp->doneDraw = true;
    dp->run = true;
    dp->keys = malloc(sizeof(char) * 6);
    memset(dp->keys, 0, sizeof(char) * 6);
    return dp;
}

void updatePlayer(player * p, data_passer * dp){
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
            p->rotation += turn_speed * (1.0/FPS);
        }
    } else {
        if (dp->keys[2]){
            p->rotation -= turn_speed * (1.0/FPS);
        }
        if (dp->keys[3]){
            p->rotation += turn_speed * (1.0/FPS);
        }
    }

    p->x += p->d_x * p->speed * cos(p->rotation) * (1.0/FPS);
    p->y += p->d_y * p->speed * sin(p->rotation) * (1.0/FPS);
}

int detectCollision(player * player, pixel * buffer){
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
                            printf("collision: player%d\n", player->id);
                            fflush(stdout);
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

void drawPlayer(player * p, pixel * buffer){
    drawCircle((int) p->x, (int) p->y, p->width, &p->color, buffer);
}

#define LEDLINE_COUNT 32

unsigned long getLedlineCode(int s1, int s2) {
    if (s1 > LEDLINE_COUNT/2 || s2 > LEDLINE_COUNT/2) return 0xffffffff;

    // player 0
    unsigned long sum0 = 0;
    for (int i = LEDLINE_COUNT-1; i >= LEDLINE_COUNT-s1; --i) {
        sum0 += pow(2, i);
    }

    // player 1
    unsigned long sum1 = 0;
    for (int i = 0; i < s2; ++i) {
        sum1 += pow(2, i);
    }

    return sum0 + sum1;
}

void resetPlayer(player * p){
    if (p->id == 0){
        p->x = SCREEN_SIZE_X/8 + rand() % (SCREEN_SIZE_X/4);
        p->y = SCREEN_SIZE_Y/4 + rand() % (SCREEN_SIZE_Y/2);
    } else {
        p->x = 5*SCREEN_SIZE_X/8 + rand() % (SCREEN_SIZE_X/4);
        p->y = SCREEN_SIZE_Y/4 + rand() % (SCREEN_SIZE_Y/2);
    }
    p->rotation = (float)rand()/(float)(RAND_MAX/3.14*2);
    p->last_x = -100;
    p->last_y = -100;
}

void gameLoop(data_passer * dp, struct timespec *start, struct timespec *end, struct timespec *res){
    void *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    volatile void *spiled_reg_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    parlcd_write_cmd(parlcd_reg_base, 0x2c);
    volatile uint32_t *ledline = (spiled_reg_base + SPILED_REG_LED_LINE_o);
    *ledline = getLedlineCode(MAX_LIVES, MAX_LIVES);
    volatile uint32_t *rgb1 = (spiled_reg_base + SPILED_REG_LED_RGB1_o);
    *rgb1 = ((union led){.r = 0x10, .g = 0x10, .b = 0x10}).d;
    volatile uint32_t *knobs_input = (spiled_reg_base + SPILED_REG_KNOBS_8BIT_o);
    knobs k;

    time_t t;
    srand((unsigned) time(&t));

    pixel * b;

    pixel * pixel = createPixel(0xff, 0xff, 0x00);

    player * player1 = createPlayer(createPixel(0xff, 0xff, 0x00), 0);
    player * player2 = createPlayer(createPixel(0xff, 0x00, 0x00), 1);


    clearBuffer(dp->game_buffer);
    drawArena(dp->game_buffer);
    clearBuffer(dp->menu_buffer);

    bool reset = false;

    while (dp->run){
        if (dp->doneDraw) {
            clock_gettime(CLOCK_MONOTONIC, start);

            // sem prijde celej nas main loop
            dp->doneDraw = false;
            dp->draw = true;
            if (dp->scene == 0){
                b = dp->game_buffer;

                if (dp->clear_game_buffer){
                    dp->clear_game_buffer = false;
                    clearBuffer(dp->game_buffer);
                    drawArena(dp->game_buffer);
                }

                updatePlayer(player1, dp);
                updatePlayer(player2, dp);

                if (reset){
                    reset = false;
                    sleep(1);

                }

                if (detectCollision(player1, b)){
                    player1->lives -= 1;
                    reset = true;
                }
                if (detectCollision(player2, b)){
                    player2->lives -= 1;
                    reset = true;
                }

                if (reset){
                    uint32_t last_lives = *ledline;
                    printf("%d:%d\n", player1->lives, player2->lives);
                    fflush(stdout);

                    struct timespec short_sl = {0, 200000000};
                    for (int i = 0; i < 4; i ++){
                        nanosleep(&short_sl, NULL);
                        *ledline = getLedlineCode(player1->lives, player2->lives);
                        nanosleep(&short_sl, NULL);
                        *ledline = last_lives;
                    }

                    *ledline = getLedlineCode(player1->lives, player2->lives);

                    resetPlayer(player1);
                    resetPlayer(player2);
                    clearBuffer(dp->game_buffer);
                    drawArena(dp->game_buffer);
                } else {
                    drawPlayer(player1, b);
                    drawPlayer(player2, b);
                }
            }

            if (dp->scene == 1){
                b = dp->menu_buffer;
                drawCircle(SCREEN_SIZE_X/4, SCREEN_SIZE_Y/2, 30, createPixel(0x02, 0x02,0x02), b);
                drawCircle(3*SCREEN_SIZE_X/4, SCREEN_SIZE_Y/2, 30, createPixel(0x02, 0x02,0x02), b);

                k.d = *knobs_input;

                if (k.g_p){
                    dp->scene = 0;
                }
            }

            // tady konci nas main loop


            clock_gettime(CLOCK_MONOTONIC, end);
            res->tv_nsec = 1000000000/FPS - (end->tv_nsec - start->tv_nsec);
            struct timespec ts = {0, res->tv_nsec};
            nanosleep(&ts, NULL);
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

            if (dp->scene == 1) {
                b = dp->menu_buffer;
            } else {
                b = dp->game_buffer;
            }

            for (unsigned i = 0; i <480*320; i++){
                parlcd_write_data(parlcd_reg_base, b[i].d);
            }
            dp->doneDraw = true;
        }
    }
}

void INThandler(){
    exit(0);
}

void keyboard(data_passer * dp){
    char devname[] = "/dev/input/event0";
    int device = open(devname, O_RDONLY);
    struct input_event ev;

    signal(SIGINT, INThandler);

    while(dp->run)
    {
        read(device,&ev, sizeof(ev));
        if(ev.type == 1 && ev.value == 1){
            switch (ev.code) {
                case 30: dp->keys[0] = 1; break;
                case 32: dp->keys[1] = 1; break;
                case 105: dp->keys[2] = 1; break;
                case 106: dp->keys[3] = 1; break;
                case 1: dp->run = 0; break;
                case 64: dp->clear_game_buffer = true; break;
            }
            //printf("Key: %i State: %i\n",ev.code,ev.value);
            //fflush(stdout);
        }
        if(ev.type == 1 && ev.value == 0) {
            switch (ev.code) {
                case 30: dp->keys[0] = 0; break;
                case 32: dp->keys[1] = 0; break;
                case 105: dp->keys[2] = 0; break;
                case 106: dp->keys[3] = 0; break;
            }
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

void* keyboardThread(void * dp_void){
    data_passer * dp = (data_passer *) dp_void;
    keyboard(dp);
    return NULL;
}

int main (){
    data_passer * dp = createDataPasser();

    pthread_t tid0;
    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid0, NULL, gameLoopThread, (void *)dp);
    pthread_create(&tid1, NULL, drawThread, (void *)dp);
    pthread_create(&tid2, NULL, keyboardThread, (void *)dp);

    pthread_exit(NULL);
    return 0;
}
