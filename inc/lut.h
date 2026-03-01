/*
 * lut.h — Tabelas de seno, cosseno e raiz quadrada em ROM.
 *
 * Ângulo: u8 0–255 mapeado em 0–360°.
 * Acesso sempre via máscara & 255 — evita divisão/módulo no 68000.
 * fix32 = ponto fixo 16.16 (FIX32(1.0) = 65536).
 */
#pragma once

#include "fix.h"

extern const fix32 sin_lut[256];
extern const fix32 cos_lut[256];
extern const fix32 sqrt_lut[256]; /* sqrt_lut[i] = sqrt(i) em fix32, i = 0..255 */

/* Acesso seguro com wrap automático */
#define lut_sin(angle)  (sin_lut[(u8)(angle)])
#define lut_cos(angle)  (cos_lut[(u8)(angle)])
#define lut_sqrt(i)     (sqrt_lut[(u8)(i)])
