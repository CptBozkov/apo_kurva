#include "gameLoop.h"

void* gameLoopThread(void * dp_void) {
    data_passer * dp = (data_passer *) dp_void;
    struct timespec start = {0,0};
    struct timespec end = {0,0};
    struct timespec res;
    gameLoop(dp, &start, &end, &res);
    return NULL;
}

void gameLoop(data_passer * dp, struct timespec *start, struct timespec *end, struct timespec *res) {
    void *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    volatile void *spiled_reg_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    parlcd_write_cmd(parlcd_reg_base, 0x2c);
    volatile uint32_t *ledline = (spiled_reg_base + SPILED_REG_LED_LINE_o);
    *ledline = getLedlineCode(MAX_LIVES, MAX_LIVES);
    volatile uint32_t *rgb1 = (spiled_reg_base + SPILED_REG_LED_RGB1_o);
    volatile uint32_t *rgb2 = (spiled_reg_base + SPILED_REG_LED_RGB2_o);

    volatile uint32_t *knobs_input = (spiled_reg_base + SPILED_REG_KNOBS_8BIT_o);

    knobs k;
    knobs last_k;
    knobs_values last_knobs_values;
    knobs_values knobs_values;

    k.d = 0;
    last_k.d = 0;

    time_t t;
    srand((unsigned) time(&t));

    pixel *b;

    pixel *colors_p = generate_colors_p();
    led *colors_l = generate_colors_l();

    player * player1 = createPlayer(0, 0);
    player * player2 = createPlayer(1, 1);

    *rgb1 = colors_l[player1->color].d;
    *rgb2 = colors_l[player2->color].d;


    clearBuffer(dp->game_buffer);
    drawArena(dp->game_buffer);
    clearBuffer(dp->menu_buffer);

    bool reset = false;

    while (dp->run){
        if (dp->doneDraw) {
            clock_gettime(CLOCK_MONOTONIC, start);

            // main loop start
            dp->doneDraw = false;
            dp->draw = true;

            // loading input from knobs
            loadKnobsInput(&knobs_values, &k, &last_k, *knobs_input);

            if (reset){
                reset = false;
                dp->pause_length = 1;
                dp->pause = 1;
            }

            if (!dp->pause){
                if (dp->scene == 0){
                    b = dp->game_buffer;

                    if (dp->clear_game_buffer){
                        dp->clear_game_buffer = false;
                        clearBuffer(dp->game_buffer);
                        drawArena(dp->game_buffer);
                    }

                    updatePlayer(player1, dp);
                    updatePlayer(player2, dp);


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

                        struct timespec short_sl = {0, 200000000L};
                        for (int i = 0; i < 4; i ++){
                            nanosleep(&short_sl, NULL);
                            *ledline = getLedlineCode(player1->lives, player2->lives);
                            nanosleep(&short_sl, NULL);
                            *ledline = last_lives;
                        }

                        *ledline = getLedlineCode(player1->lives, player2->lives);
                        if (player1->lives == 0 || player2->lives == 0){
                            dp->scene = 2;
                        }
                        resetPlayer(player1);
                        resetPlayer(player2);
                        clearBuffer(dp->game_buffer);
                        drawArena(dp->game_buffer);
                    } else {
                        drawPlayer(player1, colors_p, b);
                        drawPlayer(player2, colors_p, b);
                    }
                }
            }

            /* ---  menu --- */
            if (dp->scene == 1){
                b = dp->menu_buffer;
                clearBuffer(b);

                // set player colors by rotating knobs
                selectColor(colors_p, player1, player2, b, &knobs_values, &last_knobs_values);

                // reset rgb leds to correct color
                *rgb1 = colors_l[player1->color].d;
                *rgb2 = colors_l[player2->color].d;

                // player labels
                char * p1_label = "Player1";
                char * p2_label = "Player2";
                int p_label_len =  get_double_font_width(p1_label);
                pstring_double(SCREEN_SIZE_X/4 - p_label_len/2, SCREEN_SIZE_Y/2 - 100, p1_label, b);
                pstring_double(3*SCREEN_SIZE_X/4 - p_label_len/2, SCREEN_SIZE_Y/2 - 100, p2_label, b);

                // player circles
                drawCircle(SCREEN_SIZE_X/4, SCREEN_SIZE_Y/2, 50, &colors_p[player1->color], b);
                drawCircle(3*SCREEN_SIZE_X/4, SCREEN_SIZE_Y/2, 50, &colors_p[player2->color], b);

                // press green to start label
                char * start_label = "Press green to start!";
                int start_len =  get_double_font_width(start_label);
                pstring_double(SCREEN_SIZE_X/2 - start_len/2, SCREEN_SIZE_Y - 40, start_label, b);
            }

            if (dp->scene == 2) {
                b = dp->menu_buffer;
                clearBuffer(b);
                char * s;
                if (player1->lives == 0){
                    s = "Player1 wins!";
                    drawVictoryScreen(s, b);
                }
                if (player2->lives == 0){
                    s = "Player2 wins!";
                    drawVictoryScreen(s, b);
                }
            }


            if (knobs_values.g_p == 1){
                if (dp->scene == 0){
                    dp->scene = 1;
                } else if (dp->scene == 1){
                    dp->scene = 0;
                } else if (dp->scene == 2){
                    dp->scene = 1;
                    player1->lives = MAX_LIVES;
                    player2->lives = MAX_LIVES;
                    *ledline = getLedlineCode(player1->lives, player2->lives);
                }
            }
            // main loop end

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