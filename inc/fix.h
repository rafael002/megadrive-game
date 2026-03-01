/*
 * fix.h — Camada de compatibilidade fix32 entre SGDK (m68k) e host (testes).
 *
 * No target (m68k): inclui <genesis.h> — tipos e operações vêm do SGDK.
 * No host:          define tipos SGDK e implementa operações em macros C99.
 *
 * Regras do projeto:
 *   - Toda posição e velocidade usa fix32. Zero float em runtime.
 *   - Multiplicação: fix32Mul (evita overflow com int64_t intermediário).
 *   - Divisão: fix32Div (evitar quando possível — usar shifts).
 */
#pragma once

#ifdef __m68k__
    #include <genesis.h>

#else
    /* ── Tipos SGDK para o host ─────────────────────────────────────────── */
    #include <stdint.h>
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef int8_t   s8;
    typedef int16_t  s16;
    typedef int32_t  s32;
    typedef int32_t  fix32;   /* ponto fixo 16.16: 1.0 = 65536 */

    /* ── Constantes ─────────────────────────────────────────────────────── */
    #define FIX32_ONE   ((fix32)65536)

    /* Conversão de literal float/int para fix32 (compile-time) */
    #define FIX32(v)            ((fix32)((v) * 65536.0))

    /* ── Operações básicas ──────────────────────────────────────────────── */
    #define fix32Add(a, b)      ((fix32)((a) + (b)))
    #define fix32Sub(a, b)      ((fix32)((a) - (b)))

    /* Mul: produto 64-bit >> 16 para manter precisão */
    #define fix32Mul(a, b)      ((fix32)(((int64_t)(a) * (b)) >> 16))

    /* Div: shift 64-bit antes de dividir */
    #define fix32Div(a, b)      ((fix32)(((int64_t)(a) << 16) / (b)))

    /* ── Conversões ─────────────────────────────────────────────────────── */
    #define fix32ToInt(v)       ((s32)((v) >> 16))
    /* Cast para u32 antes do shift evita UB de C99 com valores negativos */
    #define intToFix32(v)       ((fix32)((u32)(s32)(v) << 16))

    /* ── Utilitários ────────────────────────────────────────────────────── */
    #define fix32Neg(v)         ((fix32)(-(v)))
    #define fix32Abs(v)         ((fix32)((v) < 0 ? -(v) : (v)))

#endif /* __m68k__ */
