/*
 * attack.c — Implementação do ataque básico melee.
 */
#include "attack.h"

u8 attack_start(Entity *e) {
    if (entity_has_flag(e, FLAG_ATTACKING)) return 0;
    entity_set_flag(e, FLAG_ATTACKING);
    e->timer = ATTACK_FRAMES;
    return 1;
}

void attack_tick(Entity *e) {
    if (e->timer == 0) return;
    e->timer--;
    if (e->timer == 0)
        entity_clr_flag(e, FLAG_ATTACKING);
}

u8 attack_check_hit(Entity *attacker, Entity *target) {
    int ax1, ay1, ax2, ay2;
    int bx1, by1, bx2, by2;

    if (!attack_is_active(attacker)) return 0;
    if (target->hp == 0) return 0;

    ax1 = fix32ToInt(attacker->x); ay1 = fix32ToInt(attacker->y);
    ax2 = ax1 + attacker->w;       ay2 = ay1 + attacker->h;

    bx1 = fix32ToInt(target->x);   by1 = fix32ToInt(target->y);
    bx2 = bx1 + target->w;         by2 = by1 + target->h;

    if (ax1 >= bx2 || ax2 <= bx1 || ay1 >= by2 || ay2 <= by1) return 0;

    /* Hit confirmado — aplica dano */
    if (target->hp <= attacker->damage)
        target->hp = 0;
    else
        target->hp -= attacker->damage;

    return 1;
}
