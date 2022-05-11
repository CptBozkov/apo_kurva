#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void INThandler(){
    exit(0);
}

int main()
{
    char devname[] = "/dev/input/event0";
    int device = open(devname, O_RDONLY);
    struct input_event ev;

    signal(SIGINT, INThandler);

    while(1)
    {
        read(device,&ev, sizeof(ev));
        if(ev.type == 1 && ev.value == 1){
            printf("1Key: %i State: %i\n",ev.code,ev.value);
        }
        if(ev.type == 0 && ev.value == 1){
            printf("0Key: %i State: %i\n",ev.code,ev.value);
        }
        if(ev.type == 2 && ev.value == 1){
            printf("2Key: %i State: %i\n",ev.code,ev.value);
        }
    }

}


/*
 * A - 30, S - 31, D - 32
 * < - 105, | - 108, > - 106
 */