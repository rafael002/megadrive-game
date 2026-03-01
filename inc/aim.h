/*
 * aim.h — Modo mira do arco: trava jogador, ajusta ângulo 0°–180°.
 *
 * Entity.angle (u8) mapeado ao índice da LUT (0–255 = 0–360°).
 * Aiming usa apenas [AIM_ANGLE_MIN, AIM_ANGLE_MAX] = [0, 128] = [0°, 180°]:
 *   angle=0   → direita  (cos=+1, sin= 0)
 *   angle=64  → cima     (cos= 0, sin=−1 em coords de tela)
 *   angle=128 → esquerda (cos=−1, sin= 0)
 *
 * Convenção de tela (Y cresce para baixo):
 *   aim_vy usa fix32Neg(lut_sin) para vy negativo = movimento para cima.
 *
 * Contrato com o game loop:
 *   - aim_start zera vx; enquanto FLAG_AIMING, não aplicar aceleração/atrito.
 *   - aim_end libera o jogador; física retoma no próximo frame.
 */
#pragma once

#include "entity.h"
#include "lut.h"

/* ── Constantes ──────────────────────────────────────────────────────────── */
#define AIM_ANGLE_MIN   0    /* ângulo mínimo: direita (0°)     */
#define AIM_ANGLE_MAX   128  /* ângulo máximo: esquerda (180°)  */
#define AIM_ANGLE_STEP  4    /* incremento por input (32 posições distintas) */

/* ── Macros de vetor (prontos para US16 — disparo) ──────────────────────── */
#define aim_vx(e)  lut_cos((e)->angle)
#define aim_vy(e)  fix32Neg(lut_sin((e)->angle))  /* negado: cima = vy negativo */

/* ── API ─────────────────────────────────────────────────────────────────── */

/* Inicia modo mira: seta FLAG_AIMING, zera vx (trava movimento). */
void aim_start(Entity *e);

/* Encerra modo mira: limpa FLAG_AIMING. */
void aim_end(Entity *e);

/*
 * Ajusta ângulo por AIM_ANGLE_STEP na direção dir (+1 ou −1).
 * Clampado em [AIM_ANGLE_MIN, AIM_ANGLE_MAX].
 */
void aim_adjust(Entity *e, s8 dir);
