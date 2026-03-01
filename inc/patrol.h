/*
 * patrol.h — IA de patrulha ping-pong para inimigos.
 *
 * O inimigo move-se horizontalmente a PATROL_VX; ao colidir com uma parede
 * ou limite do mapa (tile_is_solid = 1 para fora dos limites), inverte a direção
 * via FLAG_FACING_RIGHT e continua no próximo frame.
 *
 * Fluxo por frame:
 *   1. patrol_tick define vx conforme FLAG_FACING_RIGHT.
 *   2. collision_move_x move e zera vx em colisão.
 *   3. Se vx zerou (bateu), inverte FLAG_FACING_RIGHT.
 *
 * patrol_start inicializa direção e define tipo ENTITY_ENEMY.
 */
#pragma once

#include "entity.h"
#include "collision.h"

/* ── Constante ───────────────────────────────────────────────────────────── */
#define PATROL_VX  FIX32(1.0)  /* velocidade horizontal de patrulha */

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * patrol_start — configura inimigo para patrulha.
 * dir: +1 = começa para a direita, -1 = esquerda.
 */
void patrol_start(Entity *e, s8 dir);

/*
 * patrol_tick — atualiza movimento e inverte direção em colisão.
 * Chamar 1× por frame para cada inimigo em patrulha.
 */
void patrol_tick(Entity *e);
