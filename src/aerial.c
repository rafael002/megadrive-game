/*
 * aerial.c — Implementação do ataque aéreo.
 */
#include "aerial.h"

u8 aerial_attack_start(Entity *e) {
    if (entity_has_flag(e, FLAG_ON_GROUND)) return 0; /* apenas no ar */
    if (entity_has_flag(e, FLAG_ATTACKING))  return 0; /* já atacando  */
    entity_set_flag(e, FLAG_ATTACKING);
    e->timer = ATTACK_FRAMES;
    return 1;
}

void aerial_attack_tick(Entity *e) {
    /* Delega para attack_tick — vx não é tocado */
    attack_tick(e);
}
