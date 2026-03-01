/*
 * test_jump.c — Testes da suite US08: pulo e duplo pulo.
 */
#include "framework.h"
#include "../inc/jump.h"
#include "../src/jump.c"

void suite_jump(void) {
    Entity e;

    TEST("US08 jump_start — pulo do chão");
    e.flags = 0; e.vy = 0;
    entity_set_flag(&e, FLAG_ON_GROUND);
    assert_equal(jump_start(&e), 1);
    assert_equal(e.vy, JUMP_IMPULSE);
    assert_true(!entity_has_flag(&e, FLAG_ON_GROUND));

    TEST("US08 jump_start — duplo pulo no ar seta FLAG_DOUBLE_JUMP_USED");
    e.flags = 0; e.vy = 0;  /* no ar, sem FLAG_ON_GROUND */
    assert_equal(jump_start(&e), 1);
    assert_equal(e.vy, DOUBLE_JUMP_IMPULSE);
    assert_true(entity_has_flag(&e, FLAG_DOUBLE_JUMP_USED));

    TEST("US08 jump_start — terceiro pulo bloqueado");
    /* FLAG_DOUBLE_JUMP_USED já setada, sem FLAG_ON_GROUND */
    assert_equal(jump_start(&e), 0);
    assert_equal(e.vy, DOUBLE_JUMP_IMPULSE);  /* vy não muda */

    TEST("US08 jump_land — reset de FLAG_DOUBLE_JUMP_USED ao tocar o chão");
    entity_set_flag(&e, FLAG_DOUBLE_JUMP_USED);
    jump_land(&e);
    assert_true(!entity_has_flag(&e, FLAG_DOUBLE_JUMP_USED));

    TEST("US08 sequência completa: chão → pulo → duplo → terra → pulo de novo");
    e.flags = 0; e.vy = 0;
    entity_set_flag(&e, FLAG_ON_GROUND);

    /* 1º pulo */
    assert_equal(jump_start(&e), 1);
    assert_true(!entity_has_flag(&e, FLAG_ON_GROUND));

    /* duplo pulo */
    assert_equal(jump_start(&e), 1);
    assert_true(entity_has_flag(&e, FLAG_DOUBLE_JUMP_USED));

    /* no ar — bloqueado */
    assert_equal(jump_start(&e), 0);

    /* toca o chão */
    entity_set_flag(&e, FLAG_ON_GROUND);
    jump_land(&e);
    assert_true(!entity_has_flag(&e, FLAG_DOUBLE_JUMP_USED));

    /* novo pulo disponível */
    assert_equal(jump_start(&e), 1);
    assert_equal(e.vy, JUMP_IMPULSE);
}
