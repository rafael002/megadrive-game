/*
 * attack.h — Ataque básico melee com janela de hitbox em frames.
 *
 * Fluxo por frame:
 *   1. attack_start() ao pressionar o botão de ataque.
 *   2. attack_tick() uma vez por frame (decrementa timer).
 *   3. attack_is_active() para checar se a hitbox está ativa neste frame.
 *   4. attack_check_hit() para testar colisão com alvos.
 *
 * Timer (Entity.timer):
 *   - attack_start: timer = ATTACK_FRAMES, seta FLAG_ATTACKING.
 *   - active window: timer entre ATTACK_HITBOX_LO+1 e ATTACK_HITBOX_HI (inclusive).
 *   - ao zerar: FLAG_ATTACKING limpo.
 */
#pragma once

#include "entity.h"

/* ── Constantes (ROM) ────────────────────────────────────────────────────── */
#define ATTACK_FRAMES        18   /* duração total do ataque em frames        */
#define ATTACK_ACTIVE_START   4   /* frame (a partir do início) que ativa a hitbox */
#define ATTACK_ACTIVE_END    10   /* frame (a partir do início) que desativa  */

/* timer descendente: ativo quando ATTACK_HITBOX_LO < timer <= ATTACK_HITBOX_HI */
#define ATTACK_HITBOX_HI  ((u8)(ATTACK_FRAMES - ATTACK_ACTIVE_START))  /* 14 */
#define ATTACK_HITBOX_LO  ((u8)(ATTACK_FRAMES - ATTACK_ACTIVE_END))    /*  8 */

/* ── Macros inline ───────────────────────────────────────────────────────── */

/* Retorna != 0 se a hitbox está ativa neste frame. */
#define attack_is_active(e) \
    (entity_has_flag((e), FLAG_ATTACKING) && \
     (e)->timer <= ATTACK_HITBOX_HI      && \
     (e)->timer >  ATTACK_HITBOX_LO)

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * attack_start — inicia a sequência de ataque.
 * Bloqueado se FLAG_ATTACKING já estiver ativo.
 * Retorna 1 se o ataque foi iniciado, 0 caso contrário.
 */
u8 attack_start(Entity *e);

/*
 * attack_tick — atualiza o timer a cada frame.
 * Ao zerar, limpa FLAG_ATTACKING.
 */
void attack_tick(Entity *e);

/*
 * attack_check_hit — verifica colisão AABB entre atacante e alvo.
 * Só processa se attack_is_active(attacker) for verdadeiro.
 * Se houver sobreposição, aplica attacker->damage ao target->hp (mínimo 0).
 * Retorna 1 se houve hit, 0 caso contrário.
 */
u8 attack_check_hit(Entity *attacker, Entity *target);
