/*
 * test_attack.c — Testes da suite US12: ataque básico melee.
 */
#include "framework.h"
#include "../inc/attack.h"
#include "../src/attack.c"

void suite_attack(void) {
    Entity atk, tgt;

    TEST("US12 attack_start — inicia ataque, seta FLAG_ATTACKING e timer");
    atk.flags = 0; atk.timer = 0;
    assert_equal(attack_start(&atk), 1);
    assert_true(entity_has_flag(&atk, FLAG_ATTACKING));
    assert_equal(atk.timer, ATTACK_FRAMES);

    TEST("US12 attack_start — bloqueado se já atacando");
    assert_equal(attack_start(&atk), 0);
    assert_equal(atk.timer, ATTACK_FRAMES); /* timer inalterado */

    TEST("US12 attack_tick — decrementa timer e limpa flag ao zerar");
    atk.flags = 0; atk.timer = 0;
    attack_start(&atk);
    {
        u8 i;
        for (i = 0; i < ATTACK_FRAMES; i++) attack_tick(&atk);
    }
    assert_equal(atk.timer, 0);
    assert_true(!entity_has_flag(&atk, FLAG_ATTACKING));

    TEST("US12 attack_is_active — inativo antes da janela");
    atk.flags = 0; atk.timer = 0;
    attack_start(&atk); /* timer = ATTACK_FRAMES (18) */
    /* frames 0..3: timer em 18..15 — antes da janela */
    assert_true(!attack_is_active(&atk));          /* timer=18, fora */
    attack_tick(&atk);                              /* timer=17 */
    assert_true(!attack_is_active(&atk));
    attack_tick(&atk);                              /* timer=16 */
    assert_true(!attack_is_active(&atk));
    attack_tick(&atk);                              /* timer=15 */
    assert_true(!attack_is_active(&atk));

    TEST("US12 attack_is_active — ativo na janela (frames 4..9)");
    attack_tick(&atk);                              /* timer=14 = HITBOX_HI */
    assert_true(attack_is_active(&atk));
    {
        u8 i;
        for (i = 0; i < 5; i++) attack_tick(&atk); /* timer = 13..9 */
        assert_true(attack_is_active(&atk));        /* timer=9 > HITBOX_LO(8) */
    }

    TEST("US12 attack_is_active — inativo após a janela");
    attack_tick(&atk);                              /* timer=8 = HITBOX_LO — não ativo */
    assert_true(!attack_is_active(&atk));
    attack_tick(&atk);                              /* timer=7 */
    assert_true(!attack_is_active(&atk));

    TEST("US12 attack_check_hit — miss quando hitbox inativa");
    atk.flags = 0; atk.timer = 0; atk.damage = 5;
    atk.x = intToFix32(0); atk.y = intToFix32(0); atk.w = 16; atk.h = 16;
    tgt.x = intToFix32(0); tgt.y = intToFix32(0); tgt.w = 16; tgt.h = 16; tgt.hp = 10;
    /* sem FLAG_ATTACKING: check_hit deve retornar 0 */
    assert_equal(attack_check_hit(&atk, &tgt), 0);
    assert_equal(tgt.hp, 10); /* hp inalterado */

    TEST("US12 attack_check_hit — hit quando sobreposição e hitbox ativa");
    atk.flags = 0; atk.timer = 0; atk.damage = 5;
    atk.x = intToFix32(0); atk.y = intToFix32(0); atk.w = 16; atk.h = 16;
    tgt.x = intToFix32(8); tgt.y = intToFix32(0); tgt.w = 16; tgt.h = 16; tgt.hp = 10;
    attack_start(&atk);
    /* avança até a janela ativa (frame 4: timer = ATTACK_FRAMES - 4 = 14) */
    {
        u8 i;
        for (i = 0; i < ATTACK_ACTIVE_START; i++) attack_tick(&atk);
    }
    assert_true(attack_is_active(&atk));
    assert_equal(attack_check_hit(&atk, &tgt), 1);
    assert_equal(tgt.hp, 5); /* 10 - 5 */

    TEST("US12 attack_check_hit — miss quando sem sobreposição AABB");
    atk.flags = 0; atk.timer = 0; atk.damage = 5;
    atk.x = intToFix32(0);  atk.y = intToFix32(0); atk.w = 16; atk.h = 16;
    tgt.x = intToFix32(32); tgt.y = intToFix32(0); tgt.w = 16; tgt.h = 16; tgt.hp = 10;
    attack_start(&atk);
    {
        u8 i;
        for (i = 0; i < ATTACK_ACTIVE_START; i++) attack_tick(&atk);
    }
    assert_true(attack_is_active(&atk));
    assert_equal(attack_check_hit(&atk, &tgt), 0);
    assert_equal(tgt.hp, 10); /* hp inalterado */

    TEST("US12 attack_check_hit — dano não vai abaixo de zero (hp usat)");
    atk.flags = 0; atk.timer = 0; atk.damage = 20;
    atk.x = intToFix32(0); atk.y = intToFix32(0); atk.w = 16; atk.h = 16;
    tgt.x = intToFix32(0); tgt.y = intToFix32(0); tgt.w = 16; tgt.h = 16; tgt.hp = 5;
    attack_start(&atk);
    {
        u8 i;
        for (i = 0; i < ATTACK_ACTIVE_START; i++) attack_tick(&atk);
    }
    assert_equal(attack_check_hit(&atk, &tgt), 1);
    assert_equal(tgt.hp, 0); /* clampado em 0, sem underflow */

    TEST("US12 attack_check_hit — ignora alvo com hp=0 (já morto)");
    atk.flags = 0; atk.timer = 0; atk.damage = 5;
    atk.x = intToFix32(0); atk.y = intToFix32(0); atk.w = 16; atk.h = 16;
    tgt.x = intToFix32(0); tgt.y = intToFix32(0); tgt.w = 16; tgt.h = 16; tgt.hp = 0;
    attack_start(&atk);
    {
        u8 i;
        for (i = 0; i < ATTACK_ACTIVE_START; i++) attack_tick(&atk);
    }
    assert_equal(attack_check_hit(&atk, &tgt), 0);
}
