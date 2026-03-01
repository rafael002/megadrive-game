#!/usr/bin/env python3
"""
gen_lut.py — Gera src/lut.c com tabelas fix32 de seno, cosseno e raiz.

Uso: python3 tools/gen_lut.py
"""
import math
import os

FIX32_SCALE = 65536
OUT = os.path.join(os.path.dirname(__file__), "..", "src", "lut.c")

sin_lut  = [int(math.sin(i * 2.0 * math.pi / 256.0) * FIX32_SCALE) for i in range(256)]
cos_lut  = [int(math.cos(i * 2.0 * math.pi / 256.0) * FIX32_SCALE) for i in range(256)]
sqrt_lut = [int(math.sqrt(i) * FIX32_SCALE) for i in range(256)]


def fmt_table(name, values, comment=""):
    rows = []
    for i in range(0, 256, 8):
        row = ", ".join(f"{v:8d}" for v in values[i:i+8])
        rows.append(f"    {row},")
    body = "\n".join(rows)
    header = f"/* {comment} */\n" if comment else ""
    return f"{header}const fix32 {name}[256] = {{\n{body}\n}};"


with open(OUT, "w") as f:
    f.write("/*\n")
    f.write(" * lut.c — Tabelas de seno, cosseno e raiz quadrada em ROM.\n")
    f.write(" *\n")
    f.write(" * Gerado por tools/gen_lut.py. NÃO editar manualmente.\n")
    f.write(" * Ângulo: u8 0-255 mapeado em 0-360°. Acesso: lut[angle & 255].\n")
    f.write(" * fix32 = ponto fixo 16.16 (1.0 = 65536).\n")
    f.write(" */\n")
    f.write('#include "lut.h"\n\n')
    f.write(fmt_table("sin_lut", sin_lut))
    f.write("\n\n")
    f.write(fmt_table("cos_lut", cos_lut))
    f.write("\n\n")
    f.write(fmt_table("sqrt_lut", sqrt_lut,
                      "sqrt_lut[i] = sqrt(i) em fix32. Índice: valor inteiro 0-255."))
    f.write("\n")

print(f"Gerado: {OUT}")
