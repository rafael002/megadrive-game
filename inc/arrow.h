/*
 * arrow.h — Disparo de flecha via Entity Pool.
 *
 * arrow_fire aloca uma entidade do pool global (g_entities) e configura:
 *   - posição inicial = posição do atirador
 *   - vx/vy = aim_vx/aim_vy(shooter) × ARROW_SPEED (vetor via LUT)
 *   - type = ENTITY_PROJECTILE, hp = 1, damage = ARROW_DAMAGE
 *
 * Movimentação: o game loop chama physics_integrate na entidade alocada.
 * Destruição: pool_free(idx) quando hp chega a 0 ou sai do mapa.
 */
#pragma once

#include "entity.h"
#include "pool.h"
#include "aim.h"

/* ── Constantes ──────────────────────────────────────────────────────────── */
#define ARROW_SPEED   FIX32(5.0)  /* pixels/frame                  */
#define ARROW_DAMAGE  1           /* dano por hit                  */
#define ARROW_W       4           /* largura da hitbox em pixels   */
#define ARROW_H       4           /* altura  da hitbox em pixels   */

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * arrow_fire — dispara uma flecha a partir do atirador.
 * Aloca entidade do pool global; seta posição, velocidade e atributos.
 * Retorna índice no pool (>= 0) ou -1 se pool cheio.
 */
s8 arrow_fire(const Entity *shooter);
