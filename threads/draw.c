#include "draw.h"

void* drawThread(void * dp_void) {
    data_passer * dp = (data_passer *) dp_void;
    draw(dp);
    return NULL;
}

void draw(data_passer * dp) {
    void *parlcd_reg_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

    pixel * b;

    while (dp->run){
        if (dp->draw){
            dp->draw = false;

            if (dp->scene == 0){
                b = dp->game_buffer;
            } else if (dp->scene == 1 || dp->scene == 2) {
                b = dp->menu_buffer;
            }

            for (unsigned i = 0; i <480*320; i++){
                parlcd_write_data(parlcd_reg_base, b[i].d);
            }
            dp->doneDraw = true;
        }
    }
}
