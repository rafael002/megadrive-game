/*
 * framework.h — Mesmas macros de test.h adaptadas para rodar no host.
 *
 * Compilado com GCC nativo (não m68k-elf). Lógica compartilhada (fix32,
 * LUT, colisão, etc.) pode ser incluída diretamente aqui.
 *
 * Ao fim de cada suite, chamar TESTS_SUMMARY() — retorna exit(1) se falhou.
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>

static int _tests_run    = 0;
static int _tests_failed = 0;

#define TEST(name) \
    printf("\n>> %s\n", (name))

#define assert_true(expr) \
    do { \
        _tests_run++; \
        if (expr) { \
            printf("  pass: %s\n", #expr); \
        } else { \
            printf("  FAIL: %s  (%s:%d)\n", #expr, __FILE__, __LINE__); \
            _tests_failed++; \
        } \
    } while (0)

#define assert_equal(a, b) \
    do { \
        _tests_run++; \
        if ((a) == (b)) { \
            printf("  pass: %s == %s\n", #a, #b); \
        } else { \
            printf("  FAIL: %s != %s  (%s:%d)\n", #a, #b, __FILE__, __LINE__); \
            _tests_failed++; \
        } \
    } while (0)

#define TESTS_SUMMARY() \
    do { \
        printf("\n%d/%d passed\n", _tests_run - _tests_failed, _tests_run); \
        if (_tests_failed) exit(1); \
    } while (0)
