/*
 * test.h — Suite de testes in-emulator via KDebug_Alert
 *
 * Com DEBUG definido: macros emitem alertas no emulador (ex.: BlastEm, Gens).
 * Sem DEBUG (produção): todas as macros viram no-ops, zero overhead na ROM.
 *
 * Uso:
 *   TEST("nome do teste");
 *   assert_true(x > 0);
 *   assert_equal(calc_result, expected);
 */
#pragma once

#ifdef DEBUG
    #include <kdebug.h>

    #define TEST(name) \
        KDebug_Alert(">> " name)

    #define assert_true(expr) \
        do { \
            if (expr) KDebug_Alert("  pass: " #expr); \
            else      KDebug_Alert("  FAIL: " #expr); \
        } while (0)

    #define assert_equal(a, b) \
        do { \
            if ((a) == (b)) KDebug_Alert("  pass: " #a " == " #b); \
            else            KDebug_Alert("  FAIL: " #a " != " #b); \
        } while (0)

#else
    #define TEST(name)         ((void)0)
    #define assert_true(expr)  ((void)0)
    #define assert_equal(a, b) ((void)0)
#endif
