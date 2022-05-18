#ifndef APO_KURVA_DRAW_H
#define APO_KURVA_DRAW_H

#include <time.h>
#include "../consts_structs.h"

#include "../lib/mzapo_parlcd.h"
#include "../lib/mzapo_phys.h"
#include "../lib/mzapo_regs.h"
#include "../lib/font_types.h"

/*thread that draws active scene on screen*/
void* drawThread(void * dp_void);
/*called by drawThread, it contains drawing while loop*/
void draw(data_passer * dp);

#endif //APO_KURVA_DRAW_OBJECTS_H
