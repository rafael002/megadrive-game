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

/* US08 — Pulo e duplo pulo */
#include "test_jump.c"

/* US09 — Agachar */
#include "test_crouch.c"

/* US10 — Dash duplo toque */
#include "test_dash.c"

/* US12 — Ataque básico melee */
#include "test_attack.c"

/* US13 — Combo ground (3 hits) */
#include "test_combo.c"

/* US14 — Combo aerial */
#include "test_aerial.c"

/* US15 — Modo mira (ângulo 0°–180°) */
#include "test_aim.c"

/* US16 — Disparo (projétil via Pool) */
#include "test_arrow.c"

int main(void) {
    suite_tdd00();
    suite_lut();
    suite_fix();
    suite_entity();
    suite_collision();
    suite_pool();
    suite_physics();
    suite_aabb();
    suite_jump();
    suite_crouch();
    suite_dash();
    suite_attack();
    suite_combo();
    suite_aerial();
    suite_aim();
    suite_arrow();

    TESTS_SUMMARY();
    return 0;
}
