#include "main.h"

int main (){
    data_passer * dp = createDataPasser();

    pthread_t tid0;
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    pthread_create(&tid0, NULL, gameLoopThread, (void *)dp);
    pthread_create(&tid1, NULL, drawThread, (void *)dp);
    pthread_create(&tid2, NULL, keyboardThread, (void *)dp);
    pthread_create(&tid3, NULL, pauseThread, (void *)dp);

    pthread_exit(NULL);
    return 0;
}

data_passer * createDataPasser() {
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
