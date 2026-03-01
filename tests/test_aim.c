/*
 * test_aim.c — Testes da suite US15: modo mira (ângulo 0°–180°).
 */
#include "framework.h"
#include "../inc/aim.h"
#include "../src/aim.c"

void suite_aim(void) {
    Entity e;

    TEST("US15 aim_start — seta FLAG_AIMING e zera vx");
    e.flags = 0; e.vx = FIX32(2.0); e.angle = 0;
    aim_start(&e);
    assert_true(entity_has_flag(&e, FLAG_AIMING));
    assert_equal(e.vx, 0);

    TEST("US15 aim_end — limpa FLAG_AIMING");
    aim_end(&e);
    assert_true(!entity_has_flag(&e, FLAG_AIMING));

    TEST("US15 aim_adjust — incrementa ângulo (+1)");
    e.angle = 0;
    aim_adjust(&e, 1);
    assert_equal(e.angle, AIM_ANGLE_STEP);
    aim_adjust(&e, 1);
    assert_equal(e.angle, 2 * AIM_ANGLE_STEP);

    TEST("US15 aim_adjust — decrementa ângulo (-1)");
    e.angle = 2 * AIM_ANGLE_STEP;
    aim_adjust(&e, -1);
    assert_equal(e.angle, AIM_ANGLE_STEP);

    TEST("US15 aim_adjust — clamp no mínimo (AIM_ANGLE_MIN)");
    e.angle = 0;
    aim_adjust(&e, -1);
    assert_equal(e.angle, AIM_ANGLE_MIN); /* não passa de 0 */

    TEST("US15 aim_adjust — clamp no máximo (AIM_ANGLE_MAX)");
    e.angle = AIM_ANGLE_MAX;
    aim_adjust(&e, 1);
    assert_equal(e.angle, AIM_ANGLE_MAX); /* não passa de 128 */

    TEST("US15 aim_adjust — percorre range completo sem overflow");
    e.angle = 0;
    {
        u8 i;
        /* sobe até o máximo */
        for (i = 0; i < 255; i++) aim_adjust(&e, 1);
        assert_equal(e.angle, AIM_ANGLE_MAX);
        /* desce até o mínimo */
        for (i = 0; i < 255; i++) aim_adjust(&e, -1);
        assert_equal(e.angle, AIM_ANGLE_MIN);
    }

    TEST("US15 aim_vx / aim_vy — ângulo 0: direita (cos=1, vy=0)");
    e.angle = 0;
    assert_equal(aim_vx(&e), lut_cos(0));   /* FIX32_ONE */
    assert_equal(aim_vy(&e), fix32Neg(lut_sin(0))); /* 0 */

    TEST("US15 aim_vx / aim_vy — ângulo 64: cima (vx=0, vy negativo)");
    e.angle = 64;
    assert_equal(aim_vx(&e), lut_cos(64));  /* 0 */
    assert_true(aim_vy(&e) < 0);            /* vy negativo = cima na tela */

    TEST("US15 aim_vx / aim_vy — ângulo 128: esquerda (cos=-1, vy=0)");
    e.angle = 128;
    assert_true(aim_vx(&e) < 0);           /* cos(180°) = -1 */
    assert_equal(aim_vy(&e), fix32Neg(lut_sin(128))); /* ~0 */
}
