#ifndef APO_KURVA_KEYBOARD_H
#define APO_KURVA_KEYBOARD_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include "../consts_structs.h"

void* keyboardThread(void * dp_void);
void keyboard(data_passer * dp);
void exit_0();

#endif //APO_KURVA_KEYBOARD_H
