/*
 * test_arrow.c — Testes da suite US16: disparo de flecha via Pool.
 */
#include "framework.h"
#include "../inc/arrow.h"
#include "../src/arrow.c"

void suite_arrow(void) {
    Entity shooter;
    s8 idx;
    Entity *proj;

    /* Inicializa pool antes de cada grupo de testes */
    pool_init();

    TEST("US16 arrow_fire — aloca entidade do pool");
    shooter.x = intToFix32(100); shooter.y = intToFix32(50);
    shooter.angle = 0; /* direita */
    idx = arrow_fire(&shooter);
    assert_true(idx >= 0);
    assert_true(idx < MAX_ENTITIES);
    assert_equal(g_pool_free_count, MAX_ENTITIES - 1);

    TEST("US16 arrow_fire — projétil herda posição do atirador");
    proj = &g_entities[(u8)idx];
    assert_equal(proj->x, intToFix32(100));
    assert_equal(proj->y, intToFix32(50));

    TEST("US16 arrow_fire — tipo, dano, hp e hitbox corretos");
    assert_equal(proj->type,   ENTITY_PROJECTILE);
    assert_equal(proj->damage, ARROW_DAMAGE);
    assert_equal(proj->hp,     1);
    assert_equal(proj->w,      ARROW_W);
    assert_equal(proj->h,      ARROW_H);
    assert_true(entity_has_flag(proj, FLAG_ACTIVE));

    TEST("US16 arrow_fire — ângulo 0 (direita): vx=ARROW_SPEED, vy≈0");
    /* cos(0)=1, sin(0)=0 → vx=ARROW_SPEED, vy=0 */
    assert_equal(proj->vx, fix32Mul(lut_cos(0), ARROW_SPEED));
    assert_equal(proj->vy, fix32Mul(fix32Neg(lut_sin(0)), ARROW_SPEED));
    assert_equal(proj->vx, ARROW_SPEED); /* vx exatamente ARROW_SPEED */
    assert_equal(proj->vy, 0);           /* vy exatamente 0 */

    TEST("US16 arrow_fire — ângulo 64 (cima): vx≈0, vy negativo");
    pool_init();
    shooter.angle = 64;
    idx = arrow_fire(&shooter);
    proj = &g_entities[(u8)idx];
    assert_equal(proj->vx, fix32Mul(lut_cos(64), ARROW_SPEED)); /* ≈ 0 */
    assert_true(proj->vy < 0); /* negativo = cima na tela */

    TEST("US16 arrow_fire — ângulo 128 (esquerda): vx negativo, vy≈0");
    pool_init();
    shooter.angle = 128;
    idx = arrow_fire(&shooter);
    proj = &g_entities[(u8)idx];
    assert_true(proj->vx < 0); /* vx negativo = esquerda */

    TEST("US16 arrow_fire — retorna -1 quando pool cheio");
    pool_init();
    {
        u8 i;
        for (i = 0; i < MAX_ENTITIES; i++) pool_alloc();
    }
    assert_equal(arrow_fire(&shooter), -1);

    TEST("US16 arrow_fire — ângulo preservado na entidade");
    pool_init();
    shooter.angle = 48;
    idx = arrow_fire(&shooter);
    proj = &g_entities[(u8)idx];
    assert_equal(proj->angle, 48);
}
