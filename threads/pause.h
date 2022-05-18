#ifndef APO_KURVA_PAUSE_H
#define APO_KURVA_PAUSE_H

#include <time.h>
#include <unistd.h>
#include "../consts_structs.h"

/*thread used for timing events*/
void* pauseThread(void * dp_void);
/*this function contains while loop which makes the thread sleep for certain amount of time when needed
 * when its done sleeping it tells other threads to execute some code*/
void pauseLoop(data_passer * dp);

#endif //APO_KURVA_PAUSE_H
