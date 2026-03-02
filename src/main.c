/*
 * main.c — Loop principal integrado (US21).
 *
 * Liga toda a lógica de jogo ao game loop SGDK:
 *   - Cenário carregado de g_scene_default
 *   - Jogador: física, colisão, pulo, dash, agachar, combo, ataque aéreo, arco
 *   - Inimigo: patrulha ping-pong com gravidade
 *   - HUD: vida e munição via Window Layer
 *
 * Mapeamento de controles (Mega Drive 3-botões):
 *   D-pad Left / Right  : movimento; double-tap = dash
 *   Botão A             : pulo / duplo pulo
 *   Botão B             : combo melee (chão) ou ataque aéreo (ar)
 *   Botão C (hold)      : modo mira; C+A = disparo; C+Left/Right = ângulo
 *   D-pad Down          : agachar / soltar = levantar
 *
 * Cada sistema é chamdao apenas 1× por frame, após ler o joypad.
 */
#include <genesis.h>
#include "config.h"
#include "physics.h"
#include "collision.h"
#include "jump.h"
#include "crouch.h"
#include "dash.h"
#include "combo.h"
#include "aerial.h"
#include "aim.h"
#include "arrow.h"
#include "patrol.h"
#include "scene.h"
#include "hud.h"

/* ── Constantes do jogador ───────────────────────────────────────────────── */
#define PLAYER_HP_MAX  10
#define PLAYER_AMMO    30
#define PLAYER_W       12
#define PLAYER_H       16
#define PLAYER_DAMAGE   3

/* ── Estado global ───────────────────────────────────────────────────────── */
static s8  g_player_idx = -1;
static u16 g_joy_prev   = 0;
static u16 g_joy_cur    = 0;

/* Retorna 1 apenas no frame em que o botão foi pressionado (press, não hold) */
static u8 btn_pressed(u16 btn) {
    return ((g_joy_cur & btn) && !(g_joy_prev & btn)) ? 1u : 0u;
}

/* ── Inicialização ───────────────────────────────────────────────────────── */
static void game_init(void) {
    Entity *p;
    Entity *enemy;
    s8 eidx;

    pool_init();
    scene_load(g_scene_default);
    hud_init(PLAYER_HP_MAX, PLAYER_AMMO);

    /* Aloca jogador */
    g_player_idx = pool_alloc();
    if (g_player_idx >= 0) {
        p         = &g_entities[(u8)g_player_idx];
        p->type   = ENTITY_PLAYER;
        p->x      = intToFix32(80);
        p->y      = intToFix32(176); /* acima do chão (y=13*16=208, h=16 → y=192) */
        p->vx     = 0; p->vy = 0;
        p->w      = PLAYER_W; p->h = PLAYER_H;
        p->hp     = PLAYER_HP_MAX;
        p->damage = PLAYER_DAMAGE;
        p->angle  = 0; p->timer = 0; p->combo = 0;
        entity_set_flag(p, FLAG_ACTIVE);
    }

    /* Aloca inimigo de patrulha */
    eidx = pool_alloc();
    if (eidx >= 0) {
        enemy         = &g_entities[(u8)eidx];
        enemy->type   = ENTITY_ENEMY;
        enemy->x      = intToFix32(200);
        enemy->y      = intToFix32(176);
        enemy->vx     = 0; enemy->vy = 0;
        enemy->w      = 12; enemy->h = 16;
        enemy->hp     = 3;
        enemy->damage = 1;
        enemy->timer  = 0; enemy->combo = 0;
        entity_set_flag(enemy, FLAG_ACTIVE);
        patrol_start(enemy, 1);
    }
}

/* ── Update do jogador ───────────────────────────────────────────────────── */
static void player_update(void) {
    Entity *p;

    if (g_player_idx < 0) return;
    p = &g_entities[(u8)g_player_idx];
    if (p->hp == 0) return;

    /* Lê joypad */
    g_joy_prev = g_joy_cur;
    g_joy_cur  = JOY_readJoypad(JOY_1);

    /* ── Modo mira (C segurado) ── */
    if (g_joy_cur & JOYPAD_BTN_C) {
        if (!entity_has_flag(p, FLAG_AIMING)) aim_start(p);

        /* Ajuste de ângulo com d-pad */
        if (g_joy_cur & JOYPAD_BTN_LEFT)  aim_hold_tick(p, -1);
        if (g_joy_cur & JOYPAD_BTN_RIGHT) aim_hold_tick(p,  1);

        /* A = disparo (press) */
        if (btn_pressed(JOYPAD_BTN_A) && g_hud.ammo > 0) {
            arrow_fire(p);
            hud_set_ammo((u8)(g_hud.ammo - 1));
        }

    } else {
        /* ── Modo ação ── */
        if (entity_has_flag(p, FLAG_AIMING)) aim_end(p);

        /* Movimento horizontal */
        if (!entity_has_flag(p, FLAG_DASHING)) {
            if (g_joy_cur & JOYPAD_BTN_LEFT) {
                physics_accel_x(p, -1);
                if (btn_pressed(JOYPAD_BTN_LEFT)) dash_input(p, -1);
            } else if (g_joy_cur & JOYPAD_BTN_RIGHT) {
                physics_accel_x(p, 1);
                if (btn_pressed(JOYPAD_BTN_RIGHT)) dash_input(p, 1);
            } else if (!aerial_should_preserve_vx(p)) {
                physics_friction(p);
            }
        }

        /* Agachar */
        if (g_joy_cur & JOYPAD_BTN_DOWN) crouch_start(p);
        else                              crouch_end(p);

        /* Pulo (A) */
        if (btn_pressed(JOYPAD_BTN_A)) jump_start(p);

        /* Ataque (B): combo no chão, aéreo no ar */
        if (btn_pressed(JOYPAD_BTN_B)) {
            if (entity_has_flag(p, FLAG_ON_GROUND))
                combo_input(p);
            else
                aerial_attack_start(p);
        }
    }

    /* ── Ticks de sistema ── */
    dash_tick(p);

    /* Gerencia animação de ataque / combo */
    if (p->combo > 0)
        combo_tick(p);
    else if (entity_has_flag(p, FLAG_ATTACKING))
        attack_tick(p); /* ataque aéreo ou isolado */

    /* ── Física e colisão ── */
    physics_gravity(p);
    collision_move_x(p);
    collision_move_y(p);

    /* Reset de duplo-pulo ao tocar o chão */
    if (entity_has_flag(p, FLAG_ON_GROUND)) jump_land(p);

    /* Sincroniza HUD */
    hud_set_hp(p->hp);
}

/* ── Update dos inimigos ─────────────────────────────────────────────────── */
static void enemies_update(void) {
    u8 i;
    for (i = 0; i < MAX_ENTITIES; i++) {
        Entity *e = &g_entities[i];
        if (!entity_has_flag(e, FLAG_ACTIVE)) continue;
        if (e->type != ENTITY_ENEMY)          continue;
        physics_gravity(e);
        patrol_tick(e);
        collision_move_y(e);
    }
}

/* ── Ponto de entrada ────────────────────────────────────────────────────── */
int main(void) {
    VDP_setScreenWidth320();

    game_init();

    while (1) {
        player_update();
        enemies_update();
        hud_draw();

        SYS_doVBlankProcess();
    }

    return 0;
}
