/*
 * combo.h — Combo de 3 hits com janela de timing entre ataques.
 *
 * Usa FLAG_ATTACKING (de entity.h) e os mesmos parâmetros de hitbox de attack.h.
 * Entity.combo rastreia o hit atual (0=idle, 1=1º hit, 2=2º, 3=3º).
 * Entity.timer é reutilizado: animação (ATTACK_FRAMES) e janela (COMBO_WINDOW_FRAMES).
 *
 * Máquina de estados por frame:
 *   Idle(combo=0) → combo_input → Hit1(FLAG_ATTACKING, timer=ATTACK_FRAMES)
 *   Hit1 expira   →              Janela1(timer=COMBO_WINDOW_FRAMES, !FLAG_ATTACKING)
 *   Janela1 + input →            Hit2(FLAG_ATTACKING, timer=ATTACK_FRAMES)
 *   Hit2 expira   →              Janela2(timer=COMBO_WINDOW_FRAMES)
 *   Janela2 + input →            Hit3(FLAG_ATTACKING, timer=ATTACK_FRAMES)
 *   Hit3 expira   →              Idle(combo=0)
 *   Janela expira sem input →    Idle(combo=0)
 *   combo_input mid-animação →   ignorado (retorna 0)
 *
 * attack_is_active() e attack_check_hit() de attack.h continuam válidos.
 */
#pragma once

#include "entity.h"
#include "attack.h"

/* ── Constantes ──────────────────────────────────────────────────────────── */
#define COMBO_WINDOW_FRAMES  12  /* frames para aceitar o próximo hit */

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * combo_input — chamar ao pressionar o botão de ataque.
 * Ignorado se FLAG_ATTACKING estiver ativo (mid-animação).
 * Retorna o número do hit iniciado (1, 2 ou 3), ou 0 se ignorado.
 */
u8 combo_input(Entity *e);

/*
 * combo_tick — atualiza timer e estado do combo a cada frame.
 * Ao fim da animação: abre janela (hits 1 e 2) ou reseta (hit 3).
 * Ao fim da janela sem input: reseta combo.
 */
void combo_tick(Entity *e);
