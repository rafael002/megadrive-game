/*
 * combo.c — Implementação do combo de 3 hits.
 */
#include "combo.h"

u8 combo_input(Entity *e) {
    if (entity_has_flag(e, FLAG_ATTACKING)) return 0;  /* mid-animação: ignorado */

    if (e->combo > 0 && e->timer > 0) {
        /* dentro da janela: avança para o próximo hit */
        e->combo++;
    } else {
        /* idle ou janela expirada: começa do 1º hit */
        e->combo = 1;
    }

    entity_set_flag(e, FLAG_ATTACKING);
    e->timer = ATTACK_FRAMES;
    return e->combo;
}

void combo_tick(Entity *e) {
    if (e->timer == 0) return;
    e->timer--;
    if (e->timer > 0) return;

    if (entity_has_flag(e, FLAG_ATTACKING)) {
        entity_clr_flag(e, FLAG_ATTACKING);
        if (e->combo < 3) {
            /* abre janela para o próximo hit */
            e->timer = COMBO_WINDOW_FRAMES;
        } else {
            /* 3º hit concluído: reseta */
            e->combo = 0;
        }
    } else {
        /* janela expirou sem input */
        e->combo = 0;
    }
}
