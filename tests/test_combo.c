/*
 * test_combo.c — Testes da suite US13: combo ground (3 hits).
 */
#include "framework.h"
#include "../inc/combo.h"
#include "../src/combo.c"

/* Helper: avança N ticks */
static void tick_n(Entity *e, u8 n) {
    u8 i;
    for (i = 0; i < n; i++) combo_tick(e);
}

void suite_combo(void) {
    Entity e;

    TEST("US13 combo_input — idle inicia hit 1");
    e.flags = 0; e.timer = 0; e.combo = 0;
    assert_equal(combo_input(&e), 1);
    assert_equal(e.combo, 1);
    assert_true(entity_has_flag(&e, FLAG_ATTACKING));
    assert_equal(e.timer, ATTACK_FRAMES);

    TEST("US13 combo_input — ignorado mid-animação");
    assert_equal(combo_input(&e), 0);
    assert_equal(e.combo, 1); /* combo inalterado */

    TEST("US13 combo_tick — animação hit1 expira, abre janela");
    tick_n(&e, ATTACK_FRAMES);
    assert_true(!entity_has_flag(&e, FLAG_ATTACKING));
    assert_equal(e.combo, 1);           /* ainda em combo */
    assert_equal(e.timer, COMBO_WINDOW_FRAMES); /* janela aberta */

    TEST("US13 combo_input — na janela avança para hit 2");
    assert_equal(combo_input(&e), 2);
    assert_equal(e.combo, 2);
    assert_true(entity_has_flag(&e, FLAG_ATTACKING));
    assert_equal(e.timer, ATTACK_FRAMES);

    TEST("US13 combo_tick — animação hit2 expira, abre janela");
    tick_n(&e, ATTACK_FRAMES);
    assert_true(!entity_has_flag(&e, FLAG_ATTACKING));
    assert_equal(e.combo, 2);
    assert_equal(e.timer, COMBO_WINDOW_FRAMES);

    TEST("US13 combo_input — na janela avança para hit 3");
    assert_equal(combo_input(&e), 3);
    assert_equal(e.combo, 3);
    assert_true(entity_has_flag(&e, FLAG_ATTACKING));

    TEST("US13 combo_tick — animação hit3 expira, reseta (sem janela)");
    tick_n(&e, ATTACK_FRAMES);
    assert_true(!entity_has_flag(&e, FLAG_ATTACKING));
    assert_equal(e.combo, 0);
    assert_equal(e.timer, 0); /* sem janela após 3º hit */

    TEST("US13 combo_tick — janela expira sem input, reseta combo");
    e.flags = 0; e.timer = 0; e.combo = 0;
    combo_input(&e);            /* hit 1 */
    tick_n(&e, ATTACK_FRAMES);  /* animação expira → janela */
    assert_equal(e.combo, 1);
    assert_equal(e.timer, COMBO_WINDOW_FRAMES);
    tick_n(&e, COMBO_WINDOW_FRAMES); /* janela expira sem input */
    assert_equal(e.combo, 0);
    assert_equal(e.timer, 0);

    TEST("US13 combo_input — após reset, começa do hit 1 novamente");
    assert_equal(combo_input(&e), 1);
    assert_equal(e.combo, 1);

    TEST("US13 sequência completa 3 hits → reset → novo hit 1");
    e.flags = 0; e.timer = 0; e.combo = 0;
    combo_input(&e);            /* hit 1 */
    tick_n(&e, ATTACK_FRAMES);  /* janela */
    combo_input(&e);            /* hit 2 */
    tick_n(&e, ATTACK_FRAMES);  /* janela */
    combo_input(&e);            /* hit 3 */
    tick_n(&e, ATTACK_FRAMES);  /* reset */
    assert_equal(e.combo, 0);
    assert_equal(combo_input(&e), 1); /* começa novo combo */
    assert_equal(e.combo, 1);

    TEST("US13 attack_is_active funciona no contexto do combo");
    e.flags = 0; e.timer = 0; e.combo = 0;
    combo_input(&e); /* inicia hit 1 */
    assert_true(!attack_is_active(&e)); /* início da animação: hitbox inativa */
    tick_n(&e, ATTACK_ACTIVE_START);    /* avança até a janela ativa */
    assert_true(attack_is_active(&e));  /* hitbox ativa */
}
