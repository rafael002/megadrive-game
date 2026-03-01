/*
 * physics.c — Implementação da física do jogador em fix32.
 */
#include "physics.h"

void physics_gravity(Entity *e) {
    e->vy = fix32Add(e->vy, GRAVITY);
    if (e->vy > MAX_FALL_SPEED) {
        e->vy = MAX_FALL_SPEED;
    }
}

void physics_accel_x(Entity *e, s8 dir) {
    if (dir > 0) {
        e->vx = fix32Add(e->vx, ACCEL_X);
        if (e->vx > MAX_VX) e->vx = MAX_VX;
        entity_set_flag(e, FLAG_FACING_RIGHT);
    } else if (dir < 0) {
        e->vx = fix32Sub(e->vx, ACCEL_X);
        if (e->vx < fix32Neg(MAX_VX)) e->vx = fix32Neg(MAX_VX);
        entity_clr_flag(e, FLAG_FACING_RIGHT);
    }
}

void physics_friction(Entity *e) {
    if (e->vx > FRICTION) {
        e->vx = fix32Sub(e->vx, FRICTION);
    } else if (e->vx < fix32Neg(FRICTION)) {
        e->vx = fix32Add(e->vx, FRICTION);
    } else {
        e->vx = 0;
    }
}

void physics_integrate(Entity *e) {
    e->x = fix32Add(e->x, e->vx);
    e->y = fix32Add(e->y, e->vy);
}
