
#include "keyboard.h"

void* keyboardThread(void * dp_void) {
    data_passer * dp = (data_passer *) dp_void;
    keyboard(dp);
    return NULL;
}

void keyboard(data_passer * dp) {
    char devname[] = "/dev/input/event0";
    int device = open(devname, O_RDONLY);
    struct input_event ev;

    signal(SIGINT, exit_0);

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

void exit_0(){
    exit(0);
}
