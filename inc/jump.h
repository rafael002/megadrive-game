/*
 * jump.h — Pulo e duplo pulo do jogador.
 *
 * Primeiro pulo: disponível quando FLAG_ON_GROUND está setada.
 * Duplo pulo: disponível enquanto FLAG_DOUBLE_JUMP_USED não estiver setada.
 * Ao tocar o chão (collision_move_y): chamar jump_land para resetar flags.
 */
#pragma once

#include "entity.h"

/* ── Constantes (ROM) ────────────────────────────────────────────────────── */
#define JUMP_IMPULSE        FIX32(-4.5)  /* vy negativo = sobe               */
#define DOUBLE_JUMP_IMPULSE FIX32(-3.5)  /* impulso menor no duplo pulo      */

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * jump_start — tenta executar um pulo.
 * Retorna 1 se o pulo foi realizado, 0 se não foi possível.
 *   - No chão: pulo normal (vy = JUMP_IMPULSE, limpa FLAG_ON_GROUND)
 *   - No ar sem duplo pulo usado: duplo pulo (vy = DOUBLE_JUMP_IMPULSE,
 *     seta FLAG_DOUBLE_JUMP_USED)
 *   - No ar com duplo pulo já usado: retorna 0
 */
u8 jump_start(Entity *e);

/*
 * jump_land — chamada quando a entidade toca o chão.
 * Limpa FLAG_DOUBLE_JUMP_USED para permitir novo duplo pulo.
 */
void jump_land(Entity *e);
