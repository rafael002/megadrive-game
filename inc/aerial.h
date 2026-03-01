/*
 * aerial.h — Ataque aéreo com preservação de inércia horizontal.
 *
 * Diferença do ataque terrestre (attack.h):
 *   - Só ativa se o jogador estiver no ar (!FLAG_ON_GROUND).
 *   - aerial_attack_tick não modifica vx: inércia preservada.
 *   - aerial_should_preserve_vx sinaliza ao game loop para pular
 *     physics_friction enquanto o ataque aéreo estiver ativo.
 *
 * attack_is_active() e attack_check_hit() de attack.h continuam válidos.
 */
#pragma once

#include "entity.h"
#include "attack.h"

/* ── Macro inline ────────────────────────────────────────────────────────── */

/*
 * Retorna != 0 quando o ataque aéreo está ativo.
 * O game loop deve pular physics_friction nesse caso para preservar vx.
 */
#define aerial_should_preserve_vx(e) \
    (entity_has_flag((e), FLAG_ATTACKING) && \
     !entity_has_flag((e), FLAG_ON_GROUND))

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * aerial_attack_start — inicia ataque aéreo.
 * Bloqueado se FLAG_ON_GROUND ou FLAG_ATTACKING.
 * Retorna 1 se ativado, 0 caso contrário.
 */
u8 aerial_attack_start(Entity *e);

/*
 * aerial_attack_tick — atualiza timer a cada frame.
 * Delega para attack_tick; não modifica vx.
 */
void aerial_attack_tick(Entity *e);
