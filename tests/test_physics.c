/*
 * test_physics.c — Testes da suite US06: física do jogador.
 */
#include "framework.h"
#include "../inc/physics.h"
#include "../src/physics.c"

void suite_physics(void) {
    Entity e;

    TEST("US06 physics_gravity — vy após 10 frames de queda");
    e.vy = 0;
    {
        u8 i;
        for (i = 0; i < 10; i++) physics_gravity(&e);
    }
    /* vy = 10 * GRAVITY = 10 * FIX32(0.25) = FIX32(2.5) */
    assert_equal(e.vy, FIX32(2.5));

    TEST("US06 physics_gravity — terminal velocity (cap em MAX_FALL_SPEED)");
    e.vy = 0;
    {
        u8 i;
        for (i = 0; i < 100; i++) physics_gravity(&e);
    }
    assert_equal(e.vy, MAX_FALL_SPEED);

    TEST("US06 physics_accel_x — aceleração direita e cap MAX_VX");
    e.vx = 0; e.flags = 0;
    physics_accel_x(&e, 1);
    assert_equal(e.vx, ACCEL_X);
    assert_true(entity_has_flag(&e, FLAG_FACING_RIGHT));
    /* Acumula até MAX_VX */
    {
        u8 i;
        for (i = 0; i < 20; i++) physics_accel_x(&e, 1);
    }
    assert_equal(e.vx, MAX_VX);

    TEST("US06 physics_accel_x — aceleração esquerda e cap -MAX_VX");
    e.vx = 0; e.flags = FLAG_FACING_RIGHT;
    physics_accel_x(&e, -1);
    assert_equal(e.vx, fix32Neg(ACCEL_X));
    assert_true(!entity_has_flag(&e, FLAG_FACING_RIGHT));
    {
        u8 i;
        for (i = 0; i < 20; i++) physics_accel_x(&e, -1);
    }
    assert_equal(e.vx, fix32Neg(MAX_VX));

    TEST("US06 physics_friction — reduz vx em direção a zero");
    e.vx = FIX32(0.1);  /* menor que FRICTION (0.3) → snap para 0 */
    physics_friction(&e);
    assert_equal(e.vx, 0);

    e.vx = MAX_VX;
    physics_friction(&e);
    assert_equal(e.vx, fix32Sub(MAX_VX, FRICTION));

    e.vx = fix32Neg(MAX_VX);
    physics_friction(&e);
    assert_equal(e.vx, fix32Add(fix32Neg(MAX_VX), FRICTION));

    TEST("US06 physics_integrate — posição acumulada por vx e vy");
    e.x = 0; e.y = 0;
    e.vx = intToFix32(2);
    e.vy = intToFix32(1);
    physics_integrate(&e);
    assert_equal(e.x, intToFix32(2));
    assert_equal(e.y, intToFix32(1));
    physics_integrate(&e);
    assert_equal(e.x, intToFix32(4));
    assert_equal(e.y, intToFix32(2));
}
