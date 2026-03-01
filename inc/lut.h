/*
 * lut.h — Tabelas de seno, cosseno e raiz quadrada em ROM.
 *
 * Ângulo: u8 0–255 mapeado em 0–360°.
 * Acesso sempre via máscara & 255 — evita divisão/módulo no 68000.
 * fix32 = ponto fixo 16.16 (FIX32(1.0) = 65536).
 */
#pragma once

#ifdef __m68k__
    #include <genesis.h>
#else
    #include <stdint.h>
    typedef int32_t  fix32;
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef int16_t  s16;
    typedef int32_t  s32;
#endif

extern const fix32 sin_lut[256];
extern const fix32 cos_lut[256];
extern const fix32 sqrt_lut[256]; /* sqrt_lut[i] = sqrt(i) em fix32, i = 0..255 */

/* Acesso seguro com wrap automático */
#define lut_sin(angle)  (sin_lut[(u8)(angle)])
#define lut_cos(angle)  (cos_lut[(u8)(angle)])
#define lut_sqrt(i)     (sqrt_lut[(u8)(i)])
