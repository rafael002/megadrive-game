/*
 * test_dash.c — Testes da suite US10: dash por duplo toque.
 */
#include "framework.h"
#include "../inc/dash.h"
#include "../src/dash.c"

void suite_dash(void) {
    Entity e;

    TEST("US10 dash_input — toque único inicia timer (sem dash)");
    e.flags = 0; e.timer = 0; e.vx = 0;
    assert_equal(dash_input(&e, 1), 0);
    assert_equal(e.timer, DASH_WINDOW_FRAMES);
    assert_true(!entity_has_flag(&e, FLAG_DASHING));

    TEST("US10 dash_input — duplo toque direita ativa dash");
    e.flags = 0; e.timer = 0; e.vx = 0;
    dash_input(&e, 1);                  /* 1º toque direita */
    assert_equal(dash_input(&e, 1), 1); /* 2º toque direita → dash! */
    assert_true(entity_has_flag(&e, FLAG_DASHING));
    assert_equal(e.vx, DASH_VX);
    assert_equal(e.timer, DASH_FRAMES);

    TEST("US10 dash_input — duplo toque esquerda ativa dash");
    e.flags = 0; e.timer = 0; e.vx = 0;
    dash_input(&e, -1);
    assert_equal(dash_input(&e, -1), 1);
    assert_true(entity_has_flag(&e, FLAG_DASHING));
    assert_equal(e.vx, fix32Neg(DASH_VX));

    TEST("US10 dash_input — direções opostas não ativam dash");
    e.flags = 0; e.timer = 0; e.vx = 0;
    dash_input(&e, 1);                   /* 1º toque direita */
    assert_equal(dash_input(&e, -1), 0); /* 2º toque esquerda → sem dash */
    assert_true(!entity_has_flag(&e, FLAG_DASHING));

    TEST("US10 dash_tick — timer em frames: janela expira sem 2º toque");
    e.flags = 0; e.timer = 0; e.vx = 0;
    dash_input(&e, 1);  /* 1º toque: timer = DASH_WINDOW_FRAMES */
    assert_equal(e.timer, DASH_WINDOW_FRAMES);
    {
        u8 i;
        for (i = 0; i < DASH_WINDOW_FRAMES; i++) dash_tick(&e);
    }
    assert_equal(e.timer, 0);
    assert_true(!entity_has_flag(&e, FLAG_DASHING));
    /* 2º toque após janela → novo 1º toque, não dash */
    assert_equal(dash_input(&e, 1), 0);

    TEST("US10 dash_tick — encerra FLAG_DASHING ao expirar e limita vx");
    e.flags = 0; e.timer = 0; e.vx = 0;
    dash_input(&e, 1);
    dash_input(&e, 1);  /* ativa dash */
    assert_true(entity_has_flag(&e, FLAG_DASHING));
    {
        u8 i;
        for (i = 0; i < DASH_FRAMES; i++) dash_tick(&e);
    }
    assert_true(!entity_has_flag(&e, FLAG_DASHING));
    assert_equal(e.vx, MAX_VX);  /* vx limitado ao máximo normal */

    TEST("US10 dash_input — ignorado enquanto FLAG_DASHING ativo");
    e.flags = 0; e.timer = 0; e.vx = 0;
    dash_input(&e, 1);
    dash_input(&e, 1);  /* ativa dash */
    fix32 vx_dash = e.vx;
    assert_equal(dash_input(&e, 1), 0);  /* ignorado */
    assert_equal(e.vx, vx_dash);         /* vx inalterado */
}
