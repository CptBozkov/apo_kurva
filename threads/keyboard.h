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

/*thread used for managing keyboard inputs*/
void* keyboardThread(void * dp_void);
/*this function contains while loop
 * it is called by keyboard thread*/
void keyboard(data_passer * dp);
/*function which exits with code: 0*/
void exit_0();

#endif //APO_KURVA_KEYBOARD_H
