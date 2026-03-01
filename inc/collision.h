/*
 * collision.h — Mapa de colisão tile-based (16×16 px por tile).
 *
 * O tilemap é um array plano de u8 em RAM estática.
 * Valores: 0 = vazio, 1 = sólido. Outros valores reservados para futuros
 * tipos de tile (rampa, espinho, etc.).
 *
 * Otimização chave: conversão mundo→tile via >> 4 (evita divisão no 68000).
 *
 * Base para US07 — resposta AABB em X e Y.
 */
#pragma once

#include "fix.h"
#include "config.h"

/* ── Tilemap global ──────────────────────────────────────────────────────── */
/* Preenchido pela lógica de jogo antes do loop principal.                   */
extern u8 g_tilemap[MAP_H_TILES * MAP_W_TILES];

/* ── Conversão mundo (fix32) → índice de tile via shift ─────────────────── */
#define world_to_tile_x(wx)  (fix32ToInt(wx) >> 4)
#define world_to_tile_y(wy)  (fix32ToInt(wy) >> 4)

/* ── Conversão tile → pixel (canto superior esquerdo do tile) ────────────── */
#define tile_to_pixel_x(tx)  ((s32)(tx) << 4)
#define tile_to_pixel_y(ty)  ((s32)(ty) << 4)

/* ── API ─────────────────────────────────────────────────────────────────── */

/*
 * tile_is_solid — retorna 1 se o tile (tx, ty) é sólido, 0 caso contrário.
 * Retorna 1 também para coordenadas fora dos limites do mapa (borda sólida).
 */
u8 tile_is_solid(s16 tx, s16 ty);

/*
 * collision_point — retorna 1 se o ponto world (wx, wy) em fix32 está
 * sobre um tile sólido.
 */
u8 collision_point(fix32 wx, fix32 wy);

/* ── Resposta AABB ───────────────────────────────────────────────────────── */
#include "entity.h"

/*
 * collision_move_x — aplica vx à posição x e resolve colisão AABB no eixo X.
 * Se colidir com parede: snapa x encostado no tile e zera vx.
 * Deslize vertical fica garantido pela separação X/Y.
 */
void collision_move_x(Entity *e);

/*
 * collision_move_y — aplica vy à posição y e resolve colisão AABB no eixo Y.
 * Limpa FLAG_ON_GROUND antes de mover.
 * Colisão com chão: snapa y, zera vy, seta FLAG_ON_GROUND.
 * Colisão com teto: snapa y, zera vy.
 */
void collision_move_y(Entity *e);
