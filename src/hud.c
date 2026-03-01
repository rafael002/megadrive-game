/*
 * hud.c — Implementação do HUD.
 *
 * hud_draw no SGDK usaria, por exemplo:
 *   char buf[8];
 *   intToStr(g_hud.hp, buf, 1);
 *   VDP_drawText(buf, 1, 1);          // Window layer col 1, row 1
 *   intToStr(g_hud.ammo, buf, 1);
 *   VDP_drawText(buf, 1, 2);          // Window layer col 1, row 2
 * Ou sprites pré-renderizados para cada dígito.
 */
#include "hud.h"

HUD g_hud;

void hud_init(u8 hp_max, u8 ammo_max) {
    g_hud.hp_max   = hp_max;
    g_hud.ammo_max = ammo_max;
    g_hud.hp       = hp_max;
    g_hud.ammo     = ammo_max;
}

void hud_set_hp(u8 hp) {
    g_hud.hp = (hp > g_hud.hp_max) ? g_hud.hp_max : hp;
}

void hud_set_ammo(u8 ammo) {
    g_hud.ammo = (ammo > g_hud.ammo_max) ? g_hud.ammo_max : ammo;
}

void hud_draw(void) {
#ifdef SGDK
    /* Renderização via VDP Window Layer — implementado no target */
    char buf[4];
    intToStr(g_hud.hp,   buf, 1); VDP_drawText(buf, 1, 1);
    intToStr(g_hud.ammo, buf, 1); VDP_drawText(buf, 1, 2);
#endif
    /* Host: no-op */
}
