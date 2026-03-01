/*
 * test_lut.c — Testes da suite US01: LUT seno, cosseno e raiz.
 */
#include "framework.h"
#include "../inc/lut.h"
#include "../src/lut.c"   /* inclui a implementação diretamente no host build */

#define FIX32_ONE 65536

void suite_lut(void) {
    TEST("US01 sin_lut — valores cardinais");
    assert_equal(sin_lut[0],   0);           /* sin   0° = 0    */
    assert_equal(sin_lut[64],  FIX32_ONE);   /* sin  90° = 1.0  */
    assert_equal(sin_lut[128], 0);           /* sin 180° = 0    */
    assert_equal(sin_lut[192], -FIX32_ONE);  /* sin 270° = -1.0 */

    TEST("US01 cos_lut — valores cardinais");
    assert_equal(cos_lut[0],   FIX32_ONE);   /* cos   0° = 1.0  */
    assert_equal(cos_lut[64],  0);           /* cos  90° = 0    */
    assert_equal(cos_lut[128], -FIX32_ONE);  /* cos 180° = -1.0 */
    assert_equal(cos_lut[192], 0);           /* cos 270° = 0    */

    TEST("US01 sqrt_lut — quadrados perfeitos");
    assert_equal(sqrt_lut[0], 0);              /* sqrt(0) = 0   */
    assert_equal(sqrt_lut[1], FIX32_ONE);      /* sqrt(1) = 1.0 */
    assert_equal(sqrt_lut[4], 2 * FIX32_ONE);  /* sqrt(4) = 2.0 */
    assert_equal(sqrt_lut[9], 3 * FIX32_ONE);  /* sqrt(9) = 3.0 */

    TEST("US01 lut_sin macro — wrap via & 255");
    assert_equal(lut_sin(0),   lut_sin(256)); /* 256 & 255 = 0  */
    assert_equal(lut_cos(0),   lut_cos(256));
}
