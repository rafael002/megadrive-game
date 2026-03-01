/*
 * test_patrol.c — Testes da suite US18: IA patrulha ping-pong.
 *
 * Usa g_tilemap de collision.c (já incluído via test_collision.c).
 * Layout do mapa de teste:
 *   Tile (2,1) sólido (32..47 px ×, 16..31 px y) — parede à direita
 *   Restante: livre
 */
#include "framework.h"
#include "../inc/patrol.h"
#include "../src/patrol.c"

/* Helper: reseta tilemap e seta paredes nas bordas do mapa */
static void map_clear_patrol(void) {
    int i;
    for (i = 0; i < MAP_W_TILES * MAP_H_TILES; i++) g_tilemap[i] = 0;
}

void suite_patrol(void) {
    Entity e;

    TEST("US18 patrol_start — seta tipo e direção direita");
    map_clear_patrol();
    e.flags = 0; e.x = intToFix32(0); e.y = intToFix32(0); e.w = 8; e.h = 8;
    patrol_start(&e, 1);
    assert_equal(e.type, ENTITY_ENEMY);
    assert_true(entity_has_flag(&e, FLAG_FACING_RIGHT));

    TEST("US18 patrol_start — direção esquerda");
    patrol_start(&e, -1);
    assert_true(!entity_has_flag(&e, FLAG_FACING_RIGHT));

    TEST("US18 patrol_tick — move para a direita em espaço livre");
    map_clear_patrol();
    e.flags = 0; e.x = intToFix32(0); e.y = intToFix32(0); e.w = 8; e.h = 8;
    patrol_start(&e, 1);
    patrol_tick(&e);
    assert_true(fix32ToInt(e.x) > 0);            /* avançou */
    assert_true(entity_has_flag(&e, FLAG_FACING_RIGHT)); /* mantém direção */

    TEST("US18 patrol_tick — move para a esquerda em espaço livre");
    map_clear_patrol();
    e.flags = 0; e.x = intToFix32(32); e.y = intToFix32(0); e.w = 8; e.h = 8;
    patrol_start(&e, -1);
    patrol_tick(&e);
    assert_true(fix32ToInt(e.x) < 32);           /* recuou */
    assert_true(!entity_has_flag(&e, FLAG_FACING_RIGHT));

    TEST("US18 patrol_tick — inverte ao bater em parede à direita");
    map_clear_patrol();
    /* Coloca parede sólida no tile (2,0) = x:[32,48) */
    g_tilemap[2] = 1;
    /* Entidade à esquerda da parede: x=24, w=8 → borda direita em 32 */
    e.flags = 0; e.x = intToFix32(24); e.y = intToFix32(0); e.w = 8; e.h = 8;
    patrol_start(&e, 1); /* indo para a direita */
    patrol_tick(&e);     /* deve colidir e inverter */
    assert_true(!entity_has_flag(&e, FLAG_FACING_RIGHT)); /* inverteu */

    TEST("US18 patrol_tick — inverte ao bater no limite esquerdo do mapa");
    map_clear_patrol();
    /* Entidade no tile 0; indo esquerda → sai do mapa (tile -1 = sólido) */
    e.flags = 0; e.x = intToFix32(0); e.y = intToFix32(0); e.w = 8; e.h = 8;
    patrol_start(&e, -1);
    patrol_tick(&e); /* bate no limite esquerdo */
    assert_true(entity_has_flag(&e, FLAG_FACING_RIGHT)); /* voltou para direita */

    TEST("US18 patrol_tick — ping-pong: vai, bate, volta");
    map_clear_patrol();
    g_tilemap[2] = 1; /* parede em x:[32,48) */
    e.flags = 0; e.x = intToFix32(24); e.y = intToFix32(0); e.w = 8; e.h = 8;
    patrol_start(&e, 1);
    patrol_tick(&e);  /* bate na parede → inverte para esquerda */
    assert_true(!entity_has_flag(&e, FLAG_FACING_RIGHT));
    patrol_tick(&e);  /* move para a esquerda */
    assert_true(fix32ToInt(e.x) < 24); /* recuou */
    assert_true(!entity_has_flag(&e, FLAG_FACING_RIGHT));
}
