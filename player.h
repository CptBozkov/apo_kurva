#ifndef APO_KURVA_PLAYER_H
#define APO_KURVA_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "consts_structs.h"
#include "utils/buffer.h"

player * createPlayer(int color, char id);
void updatePlayer(player * p, data_passer * dp);
int detectCollision(player * player, pixel * buffer);
void resetPlayer(player * p);
void selectColor(pixel * colors, player * p1, player * p2, pixel * buffer, knobs_values * k, knobs_values * last_k);

#endif //APO_KURVA_PLAYER_H
