/*
 * arrow.c — Implementação do disparo de flecha.
 */
#include "arrow.h"

s8 arrow_fire(const Entity *shooter) {
    Entity *proj;
    s8 idx = pool_alloc();

    if (idx < 0) return -1; /* pool cheio */

    proj = &g_entities[(u8)idx];
    proj->type   = ENTITY_PROJECTILE;
    proj->x      = shooter->x;
    proj->y      = shooter->y;
    proj->vx     = fix32Mul(aim_vx(shooter), ARROW_SPEED);
    proj->vy     = fix32Mul(aim_vy(shooter), ARROW_SPEED);
    proj->damage = ARROW_DAMAGE;
    proj->hp     = 1;
    proj->w      = ARROW_W;
    proj->h      = ARROW_H;
    proj->angle  = shooter->angle;
    proj->flags  = FLAG_ACTIVE;

    return idx;
}
