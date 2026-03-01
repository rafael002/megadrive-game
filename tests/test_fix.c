/*
 * test_fix.c — Testes da suite US02: operações fix32.
 */
#include "framework.h"
#include "../inc/fix.h"

void suite_fix(void) {
    TEST("US02 FIX32 — conversão int <-> fix32");
    assert_equal(intToFix32(1),  FIX32_ONE);
    assert_equal(intToFix32(0),  0);
    assert_equal(intToFix32(-1), -FIX32_ONE);
    assert_equal(fix32ToInt(FIX32_ONE),      1);
    assert_equal(fix32ToInt(FIX32_ONE * 3),  3);
    assert_equal(fix32ToInt(0),              0);

    TEST("US02 fix32Add / fix32Sub");
    assert_equal(fix32Add(intToFix32(2),  intToFix32(3)),  intToFix32(5));
    assert_equal(fix32Sub(intToFix32(5),  intToFix32(3)),  intToFix32(2));
    assert_equal(fix32Add(intToFix32(-1), intToFix32(1)),  0);

    TEST("US02 fix32Mul");
    assert_equal(fix32Mul(intToFix32(3),  intToFix32(4)),  intToFix32(12));
    assert_equal(fix32Mul(intToFix32(-2), intToFix32(5)),  intToFix32(-10));
    assert_equal(fix32Mul(FIX32(0.5),     intToFix32(4)),  intToFix32(2));

    TEST("US02 fix32Div");
    assert_equal(fix32Div(intToFix32(10), intToFix32(2)),  intToFix32(5));
    assert_equal(fix32Div(intToFix32(1),  intToFix32(4)),  FIX32(0.25));

    TEST("US02 fix32Neg / fix32Abs");
    assert_equal(fix32Neg(intToFix32(3)),  intToFix32(-3));
    assert_equal(fix32Abs(intToFix32(-5)), intToFix32(5));
    assert_equal(fix32Abs(intToFix32(5)),  intToFix32(5));
}
