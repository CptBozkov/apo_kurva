#include "pause.h"

void* pauseThread(void * dp_void) {
    data_passer * dp = (data_passer *) dp_void;
    pauseLoop(dp);
    return NULL;
}

void pauseLoop(data_passer * dp) {
    while (dp->run){
        if (dp->pause){
            sleep(dp->pause_length);
            dp->pause = 0;
        }
    }
}