/*
 * test_hud.c — Testes da suite US20: HUD vida e munição.
 */
#include "framework.h"
#include "../inc/hud.h"
#include "../src/hud.c"

void suite_hud(void) {

    TEST("US20 hud_init — hp e ammo iniciam no máximo");
    hud_init(10, 30);
    assert_equal(g_hud.hp_max,   10);
    assert_equal(g_hud.ammo_max, 30);
    assert_equal(g_hud.hp,       10);
    assert_equal(g_hud.ammo,     30);

    TEST("US20 hud_set_hp — atualiza vida normalmente");
    hud_init(10, 30);
    hud_set_hp(7);
    assert_equal(g_hud.hp, 7);

    TEST("US20 hud_set_hp — clamp: não ultrapassa hp_max");
    hud_set_hp(255);
    assert_equal(g_hud.hp, g_hud.hp_max); /* clampado em 10 */

    TEST("US20 hud_set_hp — hp=0 (morto)");
    hud_set_hp(0);
    assert_equal(g_hud.hp, 0);

    TEST("US20 hud_set_ammo — atualiza munição normalmente");
    hud_init(10, 30);
    hud_set_ammo(15);
    assert_equal(g_hud.ammo, 15);

    TEST("US20 hud_set_ammo — clamp: não ultrapassa ammo_max");
    hud_set_ammo(200);
    assert_equal(g_hud.ammo, g_hud.ammo_max); /* clampado em 30 */

    TEST("US20 hud_set_ammo — ammo=0 (sem munição)");
    hud_set_ammo(0);
    assert_equal(g_hud.ammo, 0);

    TEST("US20 hud_set_hp — sincroniza com Entity.hp");
    hud_init(10, 30);
    {
        Entity player;
        player.hp = 5;
        hud_set_hp(player.hp);
        assert_equal(g_hud.hp, 5);
        /* dano ao jogador */
        player.hp = 3;
        hud_set_hp(player.hp);
        assert_equal(g_hud.hp, 3);
    }

    TEST("US20 hud_draw — não crasha no host (no-op)");
    hud_init(10, 30);
    hud_draw(); /* deve ser no-op sem SGDK */
    assert_equal(g_hud.hp,   10); /* estado inalterado */
    assert_equal(g_hud.ammo, 30);

    TEST("US20 hud_init — reset completo ao reinicializar");
    hud_init(10, 30);
    hud_set_hp(2);
    hud_set_ammo(5);
    hud_init(8, 20); /* novo jogo com outros valores */
    assert_equal(g_hud.hp_max,   8);
    assert_equal(g_hud.ammo_max, 20);
    assert_equal(g_hud.hp,       8);
    assert_equal(g_hud.ammo,     20);
}
