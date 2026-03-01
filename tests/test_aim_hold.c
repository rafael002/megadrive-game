/*
 * test_aim_hold.c — Testes da suite US17: ajuste de ângulo com mira segurada.
 */
#include "framework.h"
/* aim.h/c já incluídos via test_aim.c — apenas usamos a API */

void suite_aim_hold(void) {
    Entity e;

    TEST("US17 aim_hold_tick — ajusta imediatamente na 1ª chamada (timer=0)");
    e.flags = 0; e.angle = 64; e.timer = 0;
    aim_start(&e);
    /* aim_start zera vx mas não altera timer: timer ainda 0 */
    e.timer = 0;
    aim_hold_tick(&e, 1);
    assert_equal(e.angle, 64 + AIM_ANGLE_STEP);
    assert_equal(e.timer, AIM_HOLD_RATE_FRAMES);

    TEST("US17 aim_hold_tick — sem ajuste durante o cooldown (RATE ticks)");
    {
        u8 i;
        u8 angle_before = e.angle;
        /* timer=RATE após o 1º ajuste; RATE decrementos até 0, sem ajuste */
        for (i = 0; i < AIM_HOLD_RATE_FRAMES; i++) {
            aim_hold_tick(&e, 1);
            assert_equal(e.angle, angle_before); /* ângulo não muda */
        }
    }

    TEST("US17 aim_hold_tick — ajusta novamente após RATE+1 chamadas");
    {
        u8 angle_before = e.angle;
        aim_hold_tick(&e, 1); /* timer==0: ajusta e reinicia */
        assert_equal(e.angle, (u8)(angle_before + AIM_ANGLE_STEP));
        assert_equal(e.timer, AIM_HOLD_RATE_FRAMES);
    }

    TEST("US17 aim_hold_tick — direção -1 decrementa ângulo");
    e.angle = 64; e.timer = 0;
    aim_hold_tick(&e, -1);
    assert_equal(e.angle, 64 - AIM_ANGLE_STEP);

    TEST("US17 aim_hold_tick — clamp inferior respeitado ao segurar");
    e.angle = AIM_ANGLE_STEP; e.timer = 0;
    aim_hold_tick(&e, -1); /* vai para 0 */
    assert_equal(e.angle, AIM_ANGLE_MIN);
    e.timer = 0;
    aim_hold_tick(&e, -1); /* tenta abaixo de 0 */
    assert_equal(e.angle, AIM_ANGLE_MIN);

    TEST("US17 aim_hold_tick — clamp superior respeitado ao segurar");
    e.angle = AIM_ANGLE_MAX - AIM_ANGLE_STEP; e.timer = 0;
    aim_hold_tick(&e, 1); /* vai para MAX */
    assert_equal(e.angle, AIM_ANGLE_MAX);
    e.timer = 0;
    aim_hold_tick(&e, 1); /* tenta acima de MAX */
    assert_equal(e.angle, AIM_ANGLE_MAX);

    TEST("US17 aim_hold_tick — ciclo completo: 2 ajustes separados por RATE+1 calls");
    e.angle = 0; e.timer = 0;
    aim_hold_tick(&e, 1);                              /* 1º ajuste imediato */
    assert_equal(e.angle, AIM_ANGLE_STEP);
    {
        u8 i;
        for (i = 0; i < AIM_HOLD_RATE_FRAMES; i++) aim_hold_tick(&e, 1); /* cooldown */
    }
    aim_hold_tick(&e, 1);                              /* 2º ajuste */
    assert_equal(e.angle, 2 * AIM_ANGLE_STEP);
}
