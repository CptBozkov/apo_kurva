#ifndef APO_KURVA_PLAYER_H
#define APO_KURVA_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "consts_structs.h"
#include "utils/buffer.h"

/*returns new player*/
player * createPlayer(int color, char id);

/*updates players pos based on inputs from keyboard*/
void updatePlayer(player * p, data_passer * dp);

/*detects collision with arena*/
int detectCollision(player * player, pixel * buffer);

/*randomizes players pos and rotation for the next round*/
void resetPlayer(player * p);

/*updates selected color by turning knobs*/
void selectColor(pixel * colors, player * p1, player * p2, pixel * buffer, knobs_values * k, knobs_values * last_k);

#endif //APO_KURVA_PLAYER_H
