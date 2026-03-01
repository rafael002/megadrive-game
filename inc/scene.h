/*
 * scene.h — Carregamento de cenário (tilemap) e cena padrão de uma tela.
 *
 * Lógica (host + SGDK): scene_load popula g_tilemap para colisão (US04).
 * Renderização (SGDK apenas): VDP_setTileMapEx com dados do rescomp.
 *
 * Cena padrão (g_scene_default):
 *   - Chão sólido na linha y=13 (fundo da tela)
 *   - Plataforma sólida em y=9, tiles x=7..12
 *   - Resto: ar livre
 *
 * Layout visual (20×14 tiles, cada tile=16px → tela 320×224px):
 *   y 0-8 : ar
 *   y 9   : . . . . . . . # # # # # # . . . . . . .   (plataforma)
 *   y10-12: ar
 *   y13   : # # # # # # # # # # # # # # # # # # # #   (chão)
 */
#pragma once

#include "collision.h"

/* ── Cena predefinida ────────────────────────────────────────────────────── */
extern const u8 g_scene_default[MAP_H_TILES * MAP_W_TILES];

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * scene_load — copia map_data para g_tilemap.
 * No SGDK: chamar também VDP_setTileMapEx para renderizar no VDP.
 */
void scene_load(const u8 *map_data);
