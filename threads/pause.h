#ifndef APO_KURVA_PAUSE_H
#define APO_KURVA_PAUSE_H

#include <time.h>
#include <unistd.h>
#include "../consts_structs.h"

void* pauseThread(void * dp_void);
void pauseLoop(data_passer * dp);

#endif //APO_KURVA_PAUSE_H
