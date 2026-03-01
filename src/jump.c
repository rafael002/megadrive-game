/*
 * jump.c — Implementação do pulo e duplo pulo.
 */
#include "jump.h"

u8 jump_start(Entity *e) {
    if (entity_has_flag(e, FLAG_ON_GROUND)) {
        /* Pulo do chão */
        e->vy = JUMP_IMPULSE;
        entity_clr_flag(e, FLAG_ON_GROUND);
        return 1;
    }
    if (!entity_has_flag(e, FLAG_DOUBLE_JUMP_USED)) {
        /* Duplo pulo no ar */
        e->vy = DOUBLE_JUMP_IMPULSE;
        entity_set_flag(e, FLAG_DOUBLE_JUMP_USED);
        return 1;
    }
    return 0;  /* sem pulos disponíveis */
}

void jump_land(Entity *e) {
    entity_clr_flag(e, FLAG_DOUBLE_JUMP_USED);
}
