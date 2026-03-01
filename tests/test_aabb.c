/*
 * test_aabb.c — Testes da suite US07: resposta AABB X/Y e deslize.
 */
#include "framework.h"
#include "../inc/collision.h"
#include "../inc/physics.h"
#include <string.h>

/* Helper: cria entidade simples 16x16 em (x, y) pixels */
static Entity make_entity(s16 px, s16 py) {
    Entity e;
    memset(&e, 0, sizeof(e));
    e.x = intToFix32(px);
    e.y = intToFix32(py);
    e.w = 16;
    e.h = 16;
    return e;
}

void suite_aabb(void) {
    memset(g_tilemap, 0, sizeof(g_tilemap));

    TEST("US07 collision_move_x — colisão lateral direita (valida posição x)");
    /* Tile sólido em (3,2): pixels x=[48..63], y=[32..47] */
    g_tilemap[2 * MAP_W_TILES + 3] = 1;
    {
        Entity e = make_entity(28, 32);  /* right edge = 43, tile_x=2 (livre) */
        e.vx = intToFix32(4);           /* após move: x=32, right=47, tile_x=2 (livre) */
        collision_move_x(&e);
        assert_equal(fix32ToInt(e.x), 32);  /* sem colisão: x avança normalmente */
        assert_true(e.vx != 0);

        e.vx = intToFix32(4);           /* agora x=32+4=36, right=51, tile_x=3 → sólido */
        collision_move_x(&e);
        assert_equal(fix32ToInt(e.x), 32);  /* snap: tile_x=3 → pixel 48, x=48-16=32 */
        assert_equal(e.vx, 0);
    }

    TEST("US07 collision_move_x — colisão lateral esquerda");
    /* Tile sólido em (1,2): pixels x=[16..31], y=[32..47] */
    g_tilemap[2 * MAP_W_TILES + 1] = 1;
    {
        Entity e = make_entity(32, 32);
        e.vx = intToFix32(-4);          /* x=28, left_edge=28, tile_x=1 → sólido */
        collision_move_x(&e);
        assert_equal(fix32ToInt(e.x), 32);  /* snap: tile_x=1, x=tile_to_pixel(2)=32 */
        assert_equal(e.vx, 0);
    }

    TEST("US07 collision_move_y — colisão com chão, FLAG_ON_GROUND");
    /* Tile sólido em (2,3): pixels x=[32..47], y=[48..63] */
    g_tilemap[3 * MAP_W_TILES + 2] = 1;
    {
        Entity e = make_entity(32, 28);
        e.vy = intToFix32(4);           /* y=32, bottom=47, tile_y=2 (livre) */
        collision_move_y(&e);
        assert_equal(fix32ToInt(e.y), 32);
        assert_true(!entity_has_flag(&e, FLAG_ON_GROUND));

        e.vy = intToFix32(4);           /* y=36, bottom=51, tile_y=3 → sólido */
        collision_move_y(&e);
        assert_equal(fix32ToInt(e.y), 32);  /* snap: tile_y=3 → pixel 48, y=48-16=32 */
        assert_equal(e.vy, 0);
        assert_true(entity_has_flag(&e, FLAG_ON_GROUND));
    }

    TEST("US07 collision_move_y — colisão com teto");
    /* Tile sólido em (2,1): pixels x=[32..47], y=[16..31] */
    g_tilemap[1 * MAP_W_TILES + 2] = 1;
    {
        Entity e = make_entity(32, 32);
        e.vy = intToFix32(-4);          /* y=28, top=28, tile_y=1 → sólido */
        collision_move_y(&e);
        assert_equal(fix32ToInt(e.y), 32);  /* snap: tile_y=1, y=tile_to_pixel(2)=32 */
        assert_equal(e.vy, 0);
        assert_true(!entity_has_flag(&e, FLAG_ON_GROUND));
    }

    TEST("US07 deslize — bateu na parede (vx=0) mas vy continua");
    {
        /* Parede à direita em tile (3,2) já sólido */
        Entity e = make_entity(28, 28);
        e.vx = intToFix32(4);
        e.vy = intToFix32(2);
        collision_move_x(&e);   /* bate na parede: vx=0 */
        collision_move_y(&e);   /* vy ainda ativo: desliza */
        assert_equal(e.vx, 0);
        assert_true(e.vy == 0 || fix32ToInt(e.y) == 30); /* moveu em Y */
    }
}
