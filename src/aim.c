/*
 * aim.c — Implementação do modo mira.
 */
#include "aim.h"

void aim_start(Entity *e) {
    entity_set_flag(e, FLAG_AIMING);
    e->vx = 0;
}

void aim_end(Entity *e) {
    entity_clr_flag(e, FLAG_AIMING);
}

void aim_adjust(Entity *e, s8 dir) {
    s16 a = (s16)e->angle + (s16)(dir > 0 ? AIM_ANGLE_STEP : -AIM_ANGLE_STEP);
    if (a < AIM_ANGLE_MIN) a = AIM_ANGLE_MIN;
    if (a > AIM_ANGLE_MAX) a = AIM_ANGLE_MAX;
    e->angle = (u8)a;
}
