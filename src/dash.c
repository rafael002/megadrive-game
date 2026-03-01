/*
 * dash.c — Implementação do dash por duplo toque direcional.
 */
#include "dash.h"

u8 dash_input(Entity *e, s8 dir) {
    u8 dir_right = (dir > 0) ? 1u : 0u;
    u8 pending_right;

    if (entity_has_flag(e, FLAG_DASHING)) return 0;

    if (e->timer > 0) {
        /* 1º toque pendente — verifica se 2º toque é na mesma direção */
        pending_right = entity_has_flag(e, FLAG_DASH_DIR) ? 1u : 0u;
        if (dir_right == pending_right) {
            /* Duplo toque confirmado — ativa dash */
            entity_set_flag(e, FLAG_DASHING);
            e->vx    = dir_right ? DASH_VX : fix32Neg(DASH_VX);
            e->timer = DASH_FRAMES;
            return 1;
        }
        /* Direção diferente — reseta e trata como novo 1º toque */
    }

    /* Registra 1º toque */
    e->timer = DASH_WINDOW_FRAMES;
    if (dir_right) entity_set_flag(e, FLAG_DASH_DIR);
    else           entity_clr_flag(e, FLAG_DASH_DIR);
    return 0;
}

void dash_tick(Entity *e) {
    if (e->timer == 0) return;
    e->timer--;
    if (e->timer == 0 && entity_has_flag(e, FLAG_DASHING)) {
        entity_clr_flag(e, FLAG_DASHING);
        /* Limita vx ao máximo normal após o dash */
        if (e->vx > MAX_VX)            e->vx = MAX_VX;
        if (e->vx < fix32Neg(MAX_VX))  e->vx = fix32Neg(MAX_VX);
    }
}
