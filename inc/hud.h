/*
 * hud.h — HUD: estado de vida e munição do jogador.
 *
 * Estado (host + SGDK): g_hud rastreia hp e ammo; funções de update clampam
 * em [0, max] para evitar overflow em u8.
 *
 * Renderização (SGDK apenas):
 *   hud_draw() chama VDP_drawText ou sprites via Window Layer do SGDK.
 *   No host: hud_draw é no-op (compilado fora com #ifdef SGDK).
 *
 * Fluxo por frame:
 *   1. hud_set_hp(player->hp)   — sincroniza com Entity.hp
 *   2. hud_set_ammo(ammo_count) — atualiza contagem de flechas
 *   3. hud_draw()               — renderiza (SGDK only)
 */
#pragma once

#include "fix.h"  /* tipos u8 */

/* ── Estado global do HUD ────────────────────────────────────────────────── */
typedef struct {
    u8 hp;       /* vida atual        */
    u8 hp_max;   /* vida máxima       */
    u8 ammo;     /* munição atual     */
    u8 ammo_max; /* munição máxima    */
} HUD;

extern HUD g_hud;

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * hud_init — inicializa HUD com vida e munição máximas cheias.
 */
void hud_init(u8 hp_max, u8 ammo_max);

/*
 * hud_set_hp — atualiza vida; clampado em [0, g_hud.hp_max].
 */
void hud_set_hp(u8 hp);

/*
 * hud_set_ammo — atualiza munição; clampado em [0, g_hud.ammo_max].
 */
void hud_set_ammo(u8 ammo);

/*
 * hud_draw — renderiza HUD na tela.
 * SGDK: VDP_drawText / Window Layer / sprites.
 * Host: no-op.
 */
void hud_draw(void);
