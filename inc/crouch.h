/*
 * crouch.h — Agachar: redução de hitbox e bloqueio em teto baixo.
 *
 * Agachado: h = CROUCH_H (8 px). Pés mantidos na mesma posição (y += delta).
 * Em pé:    h = STAND_H  (16 px).
 *
 * crouch_end verifica teto antes de restaurar a hitbox — se houver tile
 * sólido acima, o jogador permanece agachado até ter espaço.
 */
#pragma once

#include "entity.h"
#include "collision.h"

/* ── Constantes (ROM) ────────────────────────────────────────────────────── */
#define STAND_H  16   /* altura em pé (px)      */
#define CROUCH_H  8   /* altura agachado (px)   */
#define CROUCH_DELTA (STAND_H - CROUCH_H)  /* = 8 */

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * crouch_start — inicia o agachamento.
 * Seta FLAG_CROUCHING, reduz h para CROUCH_H e desce y em CROUCH_DELTA
 * para que os pés (y+h) permaneçam na mesma posição.
 * Sem efeito se já agachado.
 */
void crouch_start(Entity *e);

/*
 * crouch_end — tenta levantar.
 * Verifica se há espaço acima (CROUCH_DELTA pixels) usando tile_is_solid.
 * Se livre: restaura h=STAND_H, sobe y em CROUCH_DELTA, limpa FLAG_CROUCHING.
 * Se teto sólido: permanece agachado (sem efeito).
 */
void crouch_end(Entity *e);
