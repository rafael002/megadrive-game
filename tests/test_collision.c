/*
 * test_collision.c — Testes da suite US04: collision grid tile 16x16.
 */
#include "framework.h"
#include "../inc/collision.h"
#include "../src/collision.c"
#include <string.h>

void suite_collision(void) {
    /* Zera o tilemap antes de cada suite */
    memset(g_tilemap, 0, sizeof(g_tilemap));

    TEST("US04 world_to_tile — shift >> 4 (sem divisão)");
    /* pixel 0..15 → tile 0 */
    assert_equal(world_to_tile_x(intToFix32(0)),  0);
    assert_equal(world_to_tile_x(intToFix32(15)), 0);
    /* pixel 16..31 → tile 1 */
    assert_equal(world_to_tile_x(intToFix32(16)), 1);
    assert_equal(world_to_tile_x(intToFix32(31)), 1);
    /* pixel 32 → tile 2 */
    assert_equal(world_to_tile_x(intToFix32(32)), 2);

    TEST("US04 tile_to_pixel — shift << 4");
    assert_equal(tile_to_pixel_x(0), 0);
    assert_equal(tile_to_pixel_x(1), 16);
    assert_equal(tile_to_pixel_x(2), 32);

    TEST("US04 tile_is_solid — tile vazio e sólido");
    /* Sem tiles sólidos → tudo vazio */
    assert_equal(tile_is_solid(0, 0), 0);
    assert_equal(tile_is_solid(5, 3), 0);

    /* Define tile (2,1) como sólido */
    g_tilemap[1 * MAP_W_TILES + 2] = 1;
    assert_equal(tile_is_solid(2, 1), 1);
    assert_equal(tile_is_solid(3, 1), 0);  /* vizinho vazio */

    TEST("US04 tile_is_solid — fora dos limites é sólido");
    assert_equal(tile_is_solid(-1,  0), 1);
    assert_equal(tile_is_solid( 0, -1), 1);
    assert_equal(tile_is_solid(MAP_W_TILES,     0), 1);
    assert_equal(tile_is_solid(0, MAP_H_TILES),  1);

    TEST("US04 collision_point — coordenadas world em fix32");
    /* Tile (2,1) = sólido → pixels 32..47 x, 16..31 y */
    assert_equal(collision_point(intToFix32(32), intToFix32(16)), 1);
    assert_equal(collision_point(intToFix32(47), intToFix32(31)), 1);
    /* Tile (3,1) = vazio */
    assert_equal(collision_point(intToFix32(48), intToFix32(16)), 0);
    /* Tile (2,0) = vazio */
    assert_equal(collision_point(intToFix32(32), intToFix32(0)),  0);
}
