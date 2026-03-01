/*
 * run_tests.c — Runner principal do host test suite.
 *
 * Cada US adiciona seu próprio arquivo de teste e chama a suite aqui.
 * Rodar com: make test
 */
#include "framework.h"

/* US_TDD00 — smoke test: valida que o próprio framework funciona */
static void suite_tdd00(void) {
    TEST("TDD00 framework smoke test");
    assert_true(1 == 1);
    assert_true(0 == 0);
    assert_equal(2 + 2, 4);
    assert_equal(10 - 3, 7);
}

/* US01 — LUT seno, cosseno e raiz */
#include "test_lut.c"

/* US02 — Fixed Point fix32 */
#include "test_fix.c"

/* US03 — Entity Struct compacta */
#include "test_entity.c"

/* US04 — Collision Grid tile 16x16 */
#include "test_collision.c"

/* US05 — Entity Pool sem malloc */
#include "test_pool.c"

/* US06 — Física do jogador */
#include "test_physics.c"

/* US07 — Colisão AABB X/Y e deslize */
#include "test_aabb.c"

int main(void) {
    suite_tdd00();
    suite_lut();
    suite_fix();
    suite_entity();
    suite_collision();
    suite_pool();
    suite_physics();
    suite_aabb();

    TESTS_SUMMARY();
    return 0;
}
