#ifndef APO_KURVA_GAMELOOP_H
#define APO_KURVA_GAMELOOP_H

#include <time.h>
#include "../consts_structs.h"
#include "../utils/colors.h"
#include "../utils/buffer.h"
#include "../utils/draw_objects.h"
#include "../hardware.h"
#include "../player.h"
#include "../utils/font.h"

#include "../lib/mzapo_parlcd.h"
#include "../lib/mzapo_phys.h"
#include "../lib/mzapo_regs.h"
#include "../lib/font_types.h"

void* gameLoopThread(void * dp_void);
void gameLoop(data_passer * dp, struct timespec *start, struct timespec *end, struct timespec *res);

#endif //APO_KURVA_GAMELOOP_H
