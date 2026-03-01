/*
 * patrol.c — Implementação da IA de patrulha ping-pong.
 */
#include "patrol.h"

void patrol_start(Entity *e, s8 dir) {
    e->type = ENTITY_ENEMY;
    if (dir > 0)
        entity_set_flag(e, FLAG_FACING_RIGHT);
    else
        entity_clr_flag(e, FLAG_FACING_RIGHT);
    e->vx = 0;
    e->vy = 0;
}

void patrol_tick(Entity *e) {
    /* Define vx conforme direção atual */
    e->vx = entity_has_flag(e, FLAG_FACING_RIGHT)
            ? PATROL_VX
            : fix32Neg(PATROL_VX);

    /* Move e resolve colisão; vx zerado em caso de colisão */
    collision_move_x(e);

    /* Se bateu em parede: inverte direção */
    if (e->vx == 0)
        entity_tog_flag(e, FLAG_FACING_RIGHT);
}
