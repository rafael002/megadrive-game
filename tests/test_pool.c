/*
 * test_pool.c — Testes da suite US05: entity pool sem malloc.
 */
#include "framework.h"
#include "../inc/pool.h"
#include "../src/pool.c"

void suite_pool(void) {
    TEST("US05 pool_init — todos os slots livres");
    pool_init();
    assert_equal(g_pool_free_count, MAX_ENTITIES);

    TEST("US05 pool_alloc — retorna índice válido e seta FLAG_ACTIVE");
    s8 idx0 = pool_alloc();
    assert_true(idx0 >= 0);
    assert_true(idx0 < MAX_ENTITIES);
    assert_equal(g_pool_free_count, MAX_ENTITIES - 1);
    assert_true(entity_has_flag(&g_entities[(u8)idx0], FLAG_ACTIVE));

    TEST("US05 pool_alloc — esgota o pool, retorna -1 no overflow");
    pool_init();
    {
        u8 i;
        for (i = 0; i < MAX_ENTITIES; i++) {
            assert_true(pool_alloc() >= 0);
        }
    }
    assert_equal(g_pool_free_count, 0);
    assert_equal(pool_alloc(), -1);  /* pool cheio */

    TEST("US05 pool_free — libera slot, flag limpa, free_count sobe");
    pool_init();
    {
        s8 a = pool_alloc();
        s8 b = pool_alloc();
        assert_equal(g_pool_free_count, MAX_ENTITIES - 2);

        pool_free((u8)a);
        assert_equal(g_pool_free_count, MAX_ENTITIES - 1);
        assert_true(!entity_has_flag(&g_entities[(u8)a], FLAG_ACTIVE));

        /* b ainda ativo */
        assert_true(entity_has_flag(&g_entities[(u8)b], FLAG_ACTIVE));
    }

    TEST("US05 pool_free + pool_alloc — reutiliza slot liberado");
    pool_init();
    {
        s8 first = pool_alloc();
        pool_free((u8)first);
        s8 reused = pool_alloc();
        /* Stack LIFO: o índice liberado deve ser reutilizado */
        assert_equal(reused, first);
        assert_true(entity_has_flag(&g_entities[(u8)reused], FLAG_ACTIVE));
    }
}
