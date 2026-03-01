/*
 * physics.h — Física do jogador: gravidade, aceleração e atrito em fix32.
 *
 * Todas as constantes são #define — ficam na ROM, zero RAM.
 * Zero float em runtime: toda aritmética usa fix32Add/Sub e comparações s32.
 *
 * Separação de responsabilidades:
 *   - physics_gravity / physics_accel_x / physics_friction — alteram vx, vy
 *   - physics_integrate — aplica vx/vy à posição (x, y)
 *   - Resposta à colisão (corrige posição e reseta vy) fica em US07
 */
#pragma once

#include "entity.h"

/* ── Constantes físicas (ROM) ────────────────────────────────────────────── */
#define GRAVITY         FIX32(0.25)   /* gravidade por frame (vy += GRAVITY)  */
#define MAX_FALL_SPEED  FIX32(6.0)    /* velocidade máxima de queda           */
#define ACCEL_X         FIX32(0.5)    /* aceleração horizontal com input       */
#define FRICTION        FIX32(0.3)    /* desaceleração horizontal sem input    */
#define MAX_VX          FIX32(3.0)    /* velocidade horizontal máxima          */

/* ── API ─────────────────────────────────────────────────────────────────── */

/* Aplica gravidade: vy += GRAVITY, limitado a MAX_FALL_SPEED. */
void physics_gravity(Entity *e);

/*
 * Aplica aceleração horizontal conforme input.
 * dir: +1 = direita, -1 = esquerda, 0 = sem input (não chama friction).
 * Atualiza FLAG_FACING_RIGHT.
 */
void physics_accel_x(Entity *e, s8 dir);

/*
 * Aplica atrito: reduz |vx| em FRICTION por frame.
 * Chamar quando dir == 0 (sem input horizontal).
 */
void physics_friction(Entity *e);

/* Integra velocidade na posição: x += vx, y += vy. */
void physics_integrate(Entity *e);
