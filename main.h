#ifndef APO_KURVA_MAIN_H
#define APO_KURVA_MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <memory.h>

#include "consts_structs.h"
#include "threads/draw.h"
#include "threads/gameLoop.h"
#include "threads/keyboard.h"
#include "threads/pause.h"

/*returns new data passer*/
data_passer * createDataPasser();

#endif //APO_KURVA_MAIN_H
