/*
 * dash.h — Dash por duplo toque direcional em <300ms (18 frames @ 60Hz).
 *
 * Detecção via timer (u8 em Entity):
 *   - 1º toque: timer = DASH_WINDOW_FRAMES; FLAG_DASH_DIR armazena direção.
 *   - 2º toque (mesma dir, timer > 0): dash! FLAG_DASHING, timer = DASH_FRAMES.
 *   - timer expira sem 2º toque: reseta (sem dash).
 *   - timer expira com FLAG_DASHING: fim do dash, vx volta ao máximo normal.
 *
 * dash_tick deve ser chamado 1x por frame.
 * dash_input deve ser chamado ao detectar press direcional (não hold).
 */
#pragma once

#include "entity.h"
#include "physics.h"

/* ── Constantes (ROM) ────────────────────────────────────────────────────── */
#define DASH_WINDOW_FRAMES  18          /* janela duplo toque: ~300ms @ 60Hz */
#define DASH_FRAMES         12          /* duração do dash em frames          */
#define DASH_VX             FIX32(8.0)  /* velocidade horizontal do dash      */

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * dash_input — chamar ao pressionar direção (press, não hold).
 * dir: +1 = direita, -1 = esquerda.
 * Retorna 1 se dash foi ativado, 0 caso contrário.
 */
u8 dash_input(Entity *e, s8 dir);

/*
 * dash_tick — atualiza timer a cada frame.
 * Decrementa timer; ao zerar com FLAG_DASHING ativo, encerra o dash
 * e limita vx a MAX_VX.
 */
void dash_tick(Entity *e);
