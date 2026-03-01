/*
 * test_crouch.c — Testes da suite US09: agachar e teto baixo.
 */
#include "framework.h"
#include "../inc/crouch.h"
#include "../src/crouch.c"
#include <string.h>

/* Helper: entidade 16x16 em pé na posição (px, py) */
static Entity make_standing(s16 px, s16 py) {
    Entity e;
    memset(&e, 0, sizeof(e));
    e.x = intToFix32(px);
    e.y = intToFix32(py);
    e.w = 16;
    e.h = STAND_H;
    return e;
}

void suite_crouch(void) {
    memset(g_tilemap, 0, sizeof(g_tilemap));

    TEST("US09 crouch_start — reduz hitbox, pés permanecem no lugar");
    {
        Entity e = make_standing(32, 32);
        s16 feet_before = (s16)(fix32ToInt(e.y) + e.h);  /* 32+16 = 48 */

        crouch_start(&e);

        assert_equal(e.h, CROUCH_H);
        assert_true(entity_has_flag(&e, FLAG_CROUCHING));
        /* Pés: novo y + nova h = (32+8) + 8 = 48 — inalterado */
        assert_equal((s16)(fix32ToInt(e.y) + e.h), feet_before);
    }

    TEST("US09 crouch_start — idempotente (sem efeito se já agachado)");
    {
        Entity e = make_standing(32, 32);
        crouch_start(&e);
        fix32 y_after_first = e.y;
        crouch_start(&e);
        assert_equal(e.y, y_after_first);
        assert_equal(e.h, CROUCH_H);
    }

    TEST("US09 crouch_end — levanta quando espaço livre");
    {
        Entity e = make_standing(32, 32);
        crouch_start(&e);
        crouch_end(&e);
        assert_equal(e.h, STAND_H);
        assert_true(!entity_has_flag(&e, FLAG_CROUCHING));
        assert_equal(fix32ToInt(e.y), 32);  /* y restaurado */
    }

    TEST("US09 crouch_end — bloqueado por teto baixo (valida colisão)");
    {
        /*
         * Entidade em pé em (32,32): ocupa y=[32..47].
         * Ao agachar: y=40, h=8, ocupa y=[40..47].
         * Tile sólido em (2,2): pixels y=[32..39].
         * Ao tentar levantar: new_top = 40-8 = 32 → tile_y=2 → sólido → bloqueado.
         */
        g_tilemap[2 * MAP_W_TILES + 2] = 1;

        Entity e = make_standing(32, 32);
        crouch_start(&e);

        assert_equal(fix32ToInt(e.y), 40);
        assert_equal(e.h, CROUCH_H);

        crouch_end(&e);  /* deve ser bloqueado pelo teto */

        assert_equal(e.h, CROUCH_H);                       /* continua agachado */
        assert_true(entity_has_flag(&e, FLAG_CROUCHING));
        assert_equal(fix32ToInt(e.y), 40);                  /* y inalterado */

        /* Remove o teto — agora deve levantar */
        g_tilemap[2 * MAP_W_TILES + 2] = 0;
        crouch_end(&e);
        assert_equal(e.h, STAND_H);
        assert_true(!entity_has_flag(&e, FLAG_CROUCHING));
    }
}
