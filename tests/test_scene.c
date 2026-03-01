/*
 * test_scene.c — Testes da suite US19: cenário tilemap e colisão.
 */
#include "framework.h"
#include "../inc/scene.h"
#include "../src/scene.c"

void suite_scene(void) {

    TEST("US19 scene_load — popula g_tilemap corretamente");
    {
        u8 custom[MAP_H_TILES * MAP_W_TILES];
        u16 i;
        for (i = 0; i < (u16)(MAP_H_TILES * MAP_W_TILES); i++) custom[i] = 0;
        custom[5] = 1; /* tile (5,0) sólido */
        scene_load(custom);
        assert_equal(g_tilemap[5], 1);
        assert_equal(g_tilemap[0], 0);
    }

    TEST("US19 g_scene_default — chão sólido em y=13 (todos os tiles)");
    scene_load(g_scene_default);
    {
        int x;
        for (x = 0; x < MAP_W_TILES; x++)
            assert_true(tile_is_solid(x, 13));
    }

    TEST("US19 g_scene_default — plataforma sólida em y=9, x=7..12");
    scene_load(g_scene_default);
    {
        int x;
        for (x = 7; x <= 12; x++)
            assert_true(tile_is_solid(x, 9));
    }

    TEST("US19 g_scene_default — ar livre fora da plataforma e chão");
    scene_load(g_scene_default);
    assert_true(!tile_is_solid(0, 0));  /* canto superior esquerdo */
    assert_true(!tile_is_solid(5, 5));  /* meio do ar */
    assert_true(!tile_is_solid(6, 9));  /* tile adjacente à plataforma */
    assert_true(!tile_is_solid(13, 9)); /* tile adjacente à plataforma */
    assert_true(!tile_is_solid(10, 8)); /* acima da plataforma */
    assert_true(!tile_is_solid(10, 10));/* abaixo da plataforma */

    TEST("US19 colisão: entidade cai e pousa no chão (y=13*16=208px)");
    scene_load(g_scene_default);
    {
        Entity e;
        e.flags = 0; e.w = 16; e.h = 16;
        /* Posição: y=100px, caindo com vy=5 */
        e.x = intToFix32(0); e.y = intToFix32(100); e.vy = FIX32(5.0);
        /* Simula queda até pousar */
        {
            u8 i;
            for (i = 0; i < 25; i++) {
                collision_move_y(&e);
                if (entity_has_flag(&e, FLAG_ON_GROUND)) break;
                e.vy = FIX32(5.0); /* mantém vy constante */
            }
        }
        assert_true(entity_has_flag(&e, FLAG_ON_GROUND));
        /* Pés em y=208 (tile y=13 = 13*16=208) */
        assert_equal(fix32ToInt(e.y) + e.h, 208);
    }

    TEST("US19 colisão: entidade pousa na plataforma (y=9*16=144px)");
    scene_load(g_scene_default);
    {
        Entity e;
        e.flags = 0; e.w = 16; e.h = 16;
        /* Posição acima da plataforma: x=112 (tile 7), y=80 */
        e.x = intToFix32(112); e.y = intToFix32(80); e.vy = FIX32(5.0);
        {
            u8 i;
            for (i = 0; i < 15; i++) {
                collision_move_y(&e);
                if (entity_has_flag(&e, FLAG_ON_GROUND)) break;
                e.vy = FIX32(5.0);
            }
        }
        assert_true(entity_has_flag(&e, FLAG_ON_GROUND));
        /* Pés em y=144 (plataforma tile y=9 → 9*16=144) */
        assert_equal(fix32ToInt(e.y) + e.h, 144);
    }
}
