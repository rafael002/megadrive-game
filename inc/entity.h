/*
 * entity.h — Struct de entidade compacta (<32 bytes) com bitflags.
 *
 * Campos ordenados do maior para o menor para garantir alinhamento natural
 * dos membros fix32 (offsets 0,4,8,12) mesmo com __attribute__((packed)).
 * O 68000 requer alinhamento de word/longword — a ordenação garante isso.
 *
 * sizeof(Entity) == 27 bytes.
 */
#pragma once

#include "fix.h"

/* ── Tipo de entidade ────────────────────────────────────────────────────── */
typedef enum {
    ENTITY_NONE       = 0,
    ENTITY_PLAYER     = 1,
    ENTITY_ENEMY      = 2,
    ENTITY_PROJECTILE = 3
} EntityType;

/* ── Bitflags de estado (u16) ────────────────────────────────────────────── */
#define FLAG_ACTIVE           ((u16)(1 << 0))  /* slot em uso no pool        */
#define FLAG_ON_GROUND        ((u16)(1 << 1))  /* tocando o chão             */
#define FLAG_FACING_RIGHT     ((u16)(1 << 2))  /* direção do sprite          */
#define FLAG_DOUBLE_JUMP_USED ((u16)(1 << 3))  /* duplo pulo consumido       */
#define FLAG_CROUCHING        ((u16)(1 << 4))  /* agachado                   */
#define FLAG_ATTACKING        ((u16)(1 << 5))  /* hitbox de ataque ativa     */
#define FLAG_AIMING           ((u16)(1 << 6))  /* modo mira do arco          */
#define FLAG_DASHING          ((u16)(1 << 7))  /* em dash                    */
#define FLAG_DASH_DIR         ((u16)(1 << 8))  /* dir do 1º toque: 1=direita */

/* ── Struct principal ────────────────────────────────────────────────────── */
typedef struct {
    fix32 x, y;     /* posição world em fix32     (8 bytes, offset  0) */
    fix32 vx, vy;   /* velocidade por frame        (8 bytes, offset  8) */
    u16   flags;    /* bitflags de estado          (2 bytes, offset 16) */
    u8    w, h;     /* hitbox em pixels            (2 bytes, offset 18) */
    u8    hp;       /* pontos de vida              (1 byte,  offset 20) */
    u8    damage;   /* dano causado ao colidir     (1 byte,  offset 21) */
    u8    type;     /* EntityType                  (1 byte,  offset 22) */
    u8    angle;    /* ângulo do projétil 0-255    (1 byte,  offset 23) */
    u8    anim;     /* frame de animação atual     (1 byte,  offset 24) */
    u8    timer;    /* timer multipropósito/frames (1 byte,  offset 25) */
    u8    combo;    /* contador de hits do combo   (1 byte,  offset 26) */
} __attribute__((packed)) Entity;

/* ── Macros de acesso a flags ────────────────────────────────────────────── */
#define entity_has_flag(e, f)   (((e)->flags & (f)) != 0)
#define entity_set_flag(e, f)   ((e)->flags |=  (f))
#define entity_clr_flag(e, f)   ((e)->flags &= (u16)~(f))
#define entity_tog_flag(e, f)   ((e)->flags ^=  (f))
