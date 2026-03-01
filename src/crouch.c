/*
 * crouch.c — Implementação do agachamento.
 */
#include "crouch.h"

void crouch_start(Entity *e) {
    if (entity_has_flag(e, FLAG_CROUCHING)) return;
    entity_set_flag(e, FLAG_CROUCHING);
    e->h  = CROUCH_H;
    e->y  = fix32Add(e->y, intToFix32(CROUCH_DELTA));
}

void crouch_end(Entity *e) {
    s16 px, right, new_top;

    if (!entity_has_flag(e, FLAG_CROUCHING)) return;

    px      = (s16)fix32ToInt(e->x);
    right   = (s16)(px + e->w - 1);
    new_top = (s16)(fix32ToInt(e->y) - CROUCH_DELTA);

    /* Verifica teto nos dois cantos superiores da hitbox em pé */
    if (tile_is_solid((s16)(px >> 4),    (s16)(new_top >> 4)) ||
        tile_is_solid((s16)(right >> 4), (s16)(new_top >> 4))) {
        return;  /* teto baixo — permanece agachado */
    }

    /* Espaço livre — levanta */
    e->y = fix32Sub(e->y, intToFix32(CROUCH_DELTA));
    e->h = STAND_H;
    entity_clr_flag(e, FLAG_CROUCHING);
}
