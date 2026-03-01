/*
 * collision.c — Implementação do mapa de colisão tile-based.
 */
#include "collision.h"

/* Tilemap global — inicializado com zeros (tudo vazio) */
u8 g_tilemap[MAP_H_TILES * MAP_W_TILES];

u8 tile_is_solid(s16 tx, s16 ty) {
    /* Fora dos limites → trata como sólido (impede saída do mapa) */
    if (tx < 0 || ty < 0 || tx >= MAP_W_TILES || ty >= MAP_H_TILES) {
        return 1;
    }
    return g_tilemap[(s16)(ty * MAP_W_TILES + tx)] != 0;
}

u8 collision_point(fix32 wx, fix32 wy) {
    s16 tx = (s16)world_to_tile_x(wx);
    s16 ty = (s16)world_to_tile_y(wy);
    return tile_is_solid(tx, ty);
}
