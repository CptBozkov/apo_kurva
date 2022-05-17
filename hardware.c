#include "hardware.h"

unsigned long getLedlineCode(int s1, int s2) {
    if (s1 > LEDLINE_COUNT/2 || s2 > LEDLINE_COUNT/2) return 0xffffffff;

    // player 0
    unsigned long sum0 = 0;
    for (int i = LEDLINE_COUNT-1; i >= LEDLINE_COUNT-s1; --i) {
        sum0 += pow(2, i);
    }

    // player 1
    unsigned long sum1 = 0;
    for (int i = 0; i < s2; ++i) {
        sum1 += pow(2, i);
    }

    return sum0 + sum1;
}

void loadKnobsInput(knobs_values * knobs_values, knobs * k, knobs * last_k, uint32_t new_data) {
    last_k->d = k->d;
    k->d = new_data;

    if (k->g_p == 1 && last_k->g_p == 0){
        knobs_values->g_p = 1;
    } else {
        knobs_values->g_p = 0;
    }

    if (k->r != last_k->r){
        knobs_values->r = k->r;
        knobs_values->r_s = 1;
    } else {
        knobs_values->r_s = 0;
    }
    if (k->b != last_k->b){
        knobs_values->b = k->b;
        knobs_values->b_s = 1;
    } else {
        knobs_values->b_s = 0;
    }
}
