/*
 * test_entity.c — Testes da suite US03: Entity struct compacta.
 */
#include "framework.h"
#include "../inc/entity.h"
#include <stddef.h>  /* offsetof */

void suite_entity(void) {
    TEST("US03 sizeof(Entity) < 32 bytes");
    assert_true(sizeof(Entity) < 32);
    assert_equal((int)sizeof(Entity), 27);

    TEST("US03 offsets — fix32 alinhados no m68k");
    assert_equal((int)offsetof(Entity, x),  0);
    assert_equal((int)offsetof(Entity, y),  4);
    assert_equal((int)offsetof(Entity, vx), 8);
    assert_equal((int)offsetof(Entity, vy), 12);
    assert_equal((int)offsetof(Entity, flags), 16);

    TEST("US03 bitflags — set, check, clear, toggle");
    Entity e = {0};
    assert_true(!entity_has_flag(&e, FLAG_ACTIVE));

    entity_set_flag(&e, FLAG_ACTIVE);
    assert_true(entity_has_flag(&e, FLAG_ACTIVE));

    entity_set_flag(&e, FLAG_ON_GROUND);
    assert_true(entity_has_flag(&e, FLAG_ACTIVE));
    assert_true(entity_has_flag(&e, FLAG_ON_GROUND));

    entity_clr_flag(&e, FLAG_ACTIVE);
    assert_true(!entity_has_flag(&e, FLAG_ACTIVE));
    assert_true(entity_has_flag(&e, FLAG_ON_GROUND));

    entity_tog_flag(&e, FLAG_FACING_RIGHT);
    assert_true(entity_has_flag(&e, FLAG_FACING_RIGHT));
    entity_tog_flag(&e, FLAG_FACING_RIGHT);
    assert_true(!entity_has_flag(&e, FLAG_FACING_RIGHT));

    TEST("US03 campos u8/u16 — sem int 32-bit nos campos de estado");
    assert_equal((int)sizeof(e.flags),  2);  /* u16 */
    assert_equal((int)sizeof(e.hp),     1);  /* u8  */
    assert_equal((int)sizeof(e.type),   1);  /* u8  */
    assert_equal((int)sizeof(e.angle),  1);  /* u8  */
    assert_equal((int)sizeof(e.timer),  1);  /* u8  */
    assert_equal((int)sizeof(e.combo),  1);  /* u8  */
}
