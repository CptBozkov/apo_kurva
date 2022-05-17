#ifndef APO_KURVA_HARDWARE_H
#define APO_KURVA_HARDWARE_H

#include <math.h>
#include "consts_structs.h"

unsigned long getLedlineCode(int s1, int s2);
void loadKnobsInput(knobs_values * knobs_values, knobs * k, knobs * last_k, uint32_t new_data);

#endif //APO_KURVA_HARDWARE_H
