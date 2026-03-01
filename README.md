# megadrive-game

Jogo 2D de plataforma com exploração e combate (melee + arco e flecha) para **Sega Mega Drive / Genesis**.

Desenvolvido em C com [SGDK](https://github.com/Stephane-D/SGDK). Pair programming **Rafael + Claude**.

---

## Hardware target

| Componente | Spec |
|---|---|
| CPU | Motorola 68000 @ 8 MHz |
| RAM | 64 KB |
| ROM | até 1 MB (8 Mbit) |
| VDP | 64 KB VRAM, 64 cores simultâneas |
| Som | Z80 + YM2612 (FM) + PSG |
| Resolução | 320×224 px (40 colunas, NTSC) |

---

## Stack

- **SGDK** — Sega Mega Drive Development Kit (GCC → m68k-elf)
- **C99** — sem C++, sem STL
- **fix32** — ponto fixo 16.16 para toda física; **zero float em runtime**
- **zero malloc** — pools estáticos, tamanhos em `config.h`
- Estrutura: `src/`, `inc/`, `res/`, `out/`, `tests/`, `tools/`

---

## Como rodar os testes

```bash
make test        # compila e roda a suite no host (GCC nativo)
```

Não requer SGDK instalado. Os testes rodam no host com zero dependências externas.

---

## Build para Mega Drive

```bash
# Requer GDK apontando para o SGDK
make             # gera out/rom.bin
```

---

## Progresso

### Sprint 0 — Fundação ✅
| US | Entregável |
|---|---|
| US_TDD00 | Framework de testes: `inc/test.h` (KDebug) + `tests/framework.h` (host) |
| US00 | Loop vsync (`SYS_doVBlankProcess`) + `inc/config.h` |
| US01 | LUT sin/cos/sqrt — 256 entradas `fix32` em ROM, geradas por `tools/gen_lut.py` |
| US02 | Camada `fix32`: `inc/fix.h` — tipos SGDK + operações para host e m68k |
| US03 | `Entity` struct 27 bytes packed + bitflags `u16` + enum `EntityType` |
| US04 | Collision grid tile 16×16: `tile_is_solid`, `collision_point`, shifts `>>4`/`<<4` |
| US05 | Entity pool estático sem malloc — free-list stack O(1) |

### Sprint 1 — Física e movimentação 🔄
| US | Entregável |
|---|---|
| US06 | Física: gravidade, aceleração horizontal e atrito em `fix32` ✅ |
| US07 | Resposta AABB X/Y com deslize em paredes ✅ |
| US08 | Pulo e duplo pulo 🔄 |
| US09 | Agachar (hitbox reduzida, bloqueio em teto baixo) |
| US10 | Dash (duplo toque < 300 ms) |

### Sprint 2 — Combate melee
US12 ataque básico · US13 combo ground (3 hits) · US14 combo aerial

### Sprint 3 — Arco e flecha
US15 modo mira · US16 disparo via pool · US17 ajuste de ângulo

### Sprint 4 — Inimigos e cenário
US18 IA patrulha · US19 tilemap · US20 HUD

---

## Arquitetura de decisões

| Decisão | Escolha |
|---|---|
| Test framework | Custom `framework.h` (zero deps) — espelha macros KDebug do target |
| Tipos SGDK no host | `inc/fix.h` — única fonte de `u8/u16/fix32/...` para host e m68k |
| LUTs | Pré-computadas em ROM, geradas por `tools/gen_lut.py` |
| Entity struct | Campos `fix32` primeiro → alinhamento natural no 68000 com `__attribute__((packed))` |
| Collision AABB | Resolução X antes de Y → deslize natural em paredes |
| Sem malloc | Tudo em arrays estáticos; `MAX_*` definidos em `config.h` |

---

## Convenções

- **Commit por US**: `USxx: descrição curta`
- **Aprovação**: Rafael aprova via comentário no card Planka antes do commit
- **Testes**: `make test` deve passar antes de todo commit
