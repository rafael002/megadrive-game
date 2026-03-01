/*
 * test_aerial.c — Testes da suite US14: ataque aéreo com preservação de inércia.
 */
#include "framework.h"
#include "../inc/aerial.h"
#include "../src/aerial.c"

void suite_aerial(void) {
    Entity e;

    TEST("US14 aerial_attack_start — bloqueado se no chão");
    e.flags = 0; e.timer = 0;
    entity_set_flag(&e, FLAG_ON_GROUND);
    assert_equal(aerial_attack_start(&e), 0);
    assert_true(!entity_has_flag(&e, FLAG_ATTACKING));

    TEST("US14 aerial_attack_start — ativa no ar");
    e.flags = 0; e.timer = 0; /* sem FLAG_ON_GROUND */
    assert_equal(aerial_attack_start(&e), 1);
    assert_true(entity_has_flag(&e, FLAG_ATTACKING));
    assert_equal(e.timer, ATTACK_FRAMES);

    TEST("US14 aerial_attack_start — bloqueado mid-animação");
    assert_equal(aerial_attack_start(&e), 0);

    TEST("US14 aerial_attack_tick — decrementa timer, limpa FLAG_ATTACKING ao zerar");
    {
        u8 i;
        for (i = 0; i < ATTACK_FRAMES; i++) aerial_attack_tick(&e);
    }
    assert_equal(e.timer, 0);
    assert_true(!entity_has_flag(&e, FLAG_ATTACKING));

    TEST("US14 aerial_attack_tick — vx preservado durante e após o ataque");
    e.flags = 0; e.timer = 0;
    e.vx = FIX32(3.0); /* inércia inicial */
    aerial_attack_start(&e);
    {
        u8 i;
        for (i = 0; i < ATTACK_FRAMES; i++) aerial_attack_tick(&e);
    }
    assert_equal(e.vx, FIX32(3.0)); /* vx inalterado */

    TEST("US14 aerial_should_preserve_vx — true quando no ar atacando");
    e.flags = 0; e.timer = 0;
    aerial_attack_start(&e); /* no ar (sem FLAG_ON_GROUND) */
    assert_true(aerial_should_preserve_vx(&e));

    TEST("US14 aerial_should_preserve_vx — false quando no chão atacando (ground attack)");
    e.flags = 0; e.timer = 0;
    entity_set_flag(&e, FLAG_ON_GROUND);
    entity_set_flag(&e, FLAG_ATTACKING); /* simula ataque terrestre */
    assert_true(!aerial_should_preserve_vx(&e));

    TEST("US14 aerial_should_preserve_vx — false quando ataque termina");
    e.flags = 0; e.timer = 0;
    aerial_attack_start(&e);
    {
        u8 i;
        for (i = 0; i < ATTACK_FRAMES; i++) aerial_attack_tick(&e);
    }
    assert_true(!aerial_should_preserve_vx(&e));

    TEST("US14 attack_is_active funciona no ataque aéreo");
    e.flags = 0; e.timer = 0;
    aerial_attack_start(&e);
    assert_true(!attack_is_active(&e));              /* início: hitbox inativa */
    {
        u8 i;
        for (i = 0; i < ATTACK_ACTIVE_START; i++) aerial_attack_tick(&e);
    }
    assert_true(attack_is_active(&e));               /* dentro da janela */

    TEST("US14 attack_check_hit funciona no ataque aéreo");
    Entity tgt;
    e.flags = 0; e.timer = 0; e.damage = 5;
    e.x = intToFix32(0); e.y = intToFix32(0); e.w = 16; e.h = 16;
    tgt.x = intToFix32(8); tgt.y = intToFix32(0); tgt.w = 16; tgt.h = 16; tgt.hp = 10;
    aerial_attack_start(&e);
    {
        u8 i;
        for (i = 0; i < ATTACK_ACTIVE_START; i++) aerial_attack_tick(&e);
    }
    assert_equal(attack_check_hit(&e, &tgt), 1);
    assert_equal(tgt.hp, 5);
}
