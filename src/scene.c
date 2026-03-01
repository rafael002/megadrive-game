/*
 * scene.c — Cenário padrão e carregamento de tilemap.
 *
 * SGDK: rescomp converte imagem de tiles em array C.
 * Aqui o array é definido manualmente para testes host.
 * VDP_setTileMapEx seria chamado após scene_load em main.c.
 */
#include "scene.h"

/*
 * Cena de uma tela: chão (y=13) + plataforma central (y=9, x=7..12).
 * 20 colunas × 14 linhas = 280 bytes em ROM.
 */
const u8 g_scene_default[MAP_H_TILES * MAP_W_TILES] = {
    /* y=0  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=1  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=2  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=3  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=4  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=5  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=6  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=7  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=8  */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=9  */ 0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
    /* y=10 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=11 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=12 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* y=13 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

void scene_load(const u8 *map_data) {
    u16 i;
    for (i = 0; i < (u16)(MAP_H_TILES * MAP_W_TILES); i++)
        g_tilemap[i] = map_data[i];
}
