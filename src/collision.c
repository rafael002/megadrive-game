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

/* Verifica se o pixel (px, py) está em tile sólido — sem fix32 overhead */
static u8 px_solid(s16 px, s16 py) {
    return tile_is_solid((s16)(px >> 4), (s16)(py >> 4));
}

void collision_move_x(Entity *e) {
    s16 px, py, bottom, right, tile_x;

    e->x = fix32Add(e->x, e->vx);

    px     = (s16)fix32ToInt(e->x);
    py     = (s16)fix32ToInt(e->y);
    right  = (s16)(px + e->w - 1);
    bottom = (s16)(py + e->h - 1);

    if (e->vx > 0) {
        /* Movendo direita — verifica canto superior e inferior direitos */
        if (px_solid(right, py) || px_solid(right, bottom)) {
            tile_x = (s16)(right >> 4);
            e->x   = intToFix32(tile_to_pixel_x(tile_x) - e->w);
            e->vx  = 0;
        }
    } else if (e->vx < 0) {
        /* Movendo esquerda — verifica canto superior e inferior esquerdos */
        if (px_solid(px, py) || px_solid(px, bottom)) {
            tile_x = (s16)(px >> 4);
            e->x   = intToFix32(tile_to_pixel_x(tile_x + 1));
            e->vx  = 0;
        }
    }
}

void collision_move_y(Entity *e) {
    s16 px, py, bottom, right, tile_y;

    /* Limpa flag a cada frame — re-seta apenas se chão detectado */
    entity_clr_flag(e, FLAG_ON_GROUND);

    e->y = fix32Add(e->y, e->vy);

    px     = (s16)fix32ToInt(e->x);
    py     = (s16)fix32ToInt(e->y);
    right  = (s16)(px + e->w - 1);
    bottom = (s16)(py + e->h - 1);

    if (e->vy > 0) {
        /* Movendo para baixo — verifica canto inferior esquerdo e direito */
        if (px_solid(px, bottom) || px_solid(right, bottom)) {
            tile_y = (s16)(bottom >> 4);
            e->y   = intToFix32(tile_to_pixel_y(tile_y) - e->h);
            e->vy  = 0;
            entity_set_flag(e, FLAG_ON_GROUND);
        }
    } else if (e->vy < 0) {
        /* Movendo para cima — verifica canto superior esquerdo e direito */
        if (px_solid(px, py) || px_solid(right, py)) {
            tile_y = (s16)(py >> 4);
            e->y   = intToFix32(tile_to_pixel_y(tile_y + 1));
            e->vy  = 0;
        }
    }
}
