/*
 * config.h — Constantes globais do jogo (ficam na ROM via #define).
 *
 * Todos os limites de pool são definidos aqui para serem ajustados em um
 * único lugar. Usar apenas #define — sem const, sem variáveis globais.
 */
#pragma once

/* ── Tela ────────────────────────────────────────────────────────────────── */
#define SCREEN_W        320     /* pixels */
#define SCREEN_H        224     /* pixels */

/* ── Tile / colisão ──────────────────────────────────────────────────────── */
#ifndef TILE_SIZE
#define TILE_SIZE       16      /* pixels por tile */
#endif
#define MAP_W_TILES     20      /* tiles por linha (320 / 16) */
#define MAP_H_TILES     14      /* tiles por coluna (224 / 16) */

/* ── Entity pool ─────────────────────────────────────────────────────────── */
#define MAX_ENTITIES    32      /* entidades ativas simultâneas (jogador + inimigos + projéteis) */
#define MAX_ENEMIES      8      /* subconjunto de MAX_ENTITIES */
#define MAX_PROJECTILES 16      /* subconjunto de MAX_ENTITIES */
