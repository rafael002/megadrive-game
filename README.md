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

## Build da ROM para Mega Drive

### Via Docker (recomendado — sem instalação local)

```bash
make rom         # usa ghcr.io/stephane-d/sgdk:latest
```

Requer apenas Docker instalado. Gera `out/rom.bin` que pode ser carregado em qualquer emulador (Blastem, Gens, RetroArch).

Imagem customizável:

```bash
SGDK_DOCKER=ghcr.io/stephane-d/sgdk:v2.00 make rom
```

### Via SGDK local

```bash
export GDK=/path/to/sgdk   # aponta para o diretório do SGDK
make                        # gera out/rom.bin
```

### Limpar artefatos

```bash
make clean        # remove binário de testes
make clean-rom    # remove out/ (artefatos da ROM)
```

### Emuladores recomendados

| Emulador | Plataforma | Observação |
|---|---|---|
| [Blastem](https://www.retrodev.com/blastem/) | Linux/macOS/Win | Precisão alta, debug integrado |
| [Gens/GS](http://segaretro.org/Gens/GS) | Linux/Win | Clássico, fácil de usar |
| [RetroArch](https://www.retroarch.com/) (Genesis Plus GX) | Multi | Accuracy + shader support |

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

### Sprint 1 — Física e movimentação ✅
| US | Entregável |
|---|---|
| US06 | Física: gravidade, aceleração horizontal e atrito em `fix32` ✅ |
| US07 | Resposta AABB X/Y com deslize em paredes ✅ |
| US08 | Pulo e duplo pulo ✅ |
| US09 | Agachar (hitbox reduzida, bloqueio em teto baixo) ✅ |
| US10 | Dash (duplo toque < 300 ms) ✅ |
| US11 | Build da ROM via Docker + local SGDK ✅ |

### Sprint 2 — Combate melee ✅
| US | Entregável |
|---|---|
| US12 | Ataque melee: hitbox ativa em janela de frames (6/18), `attack_check_hit` AABB + dano ✅ |
| US13 | Combo ground 3 hits: máquina de estados, janela de timing entre hits ✅ |
| US14 | Combo aerial: `aerial_attack_start` bloqueado no chão, inércia `vx` preservada ✅ |

### Sprint 3 — Arco e flecha ✅
| US | Entregável |
|---|---|
| US15 | Modo mira: `FLAG_AIMING`, ângulo u8 [0°–180°], trava `vx`, `aim_vx/vy` via LUT ✅ |
| US16 | Disparo: `arrow_fire` aloca pool, `vx/vy = cos/sin(angle) × ARROW_SPEED` ✅ |
| US17 | Mira segurada: `aim_hold_tick` com rate limiting via `Entity.timer` ✅ |

### Sprint 4 — Inimigos e cenário ✅
| US | Entregável |
|---|---|
| US18 | IA patrulha ping-pong: `patrol_tick` inverte em `collision_move_x` ✅ |
| US19 | Cenário tilemap: `scene_load` + `g_scene_default` (chão y=13, plataforma y=9) ✅ |
| US20 | HUD: `HUD` struct, `hud_set_hp/ammo` com clamp, `hud_draw` (SGDK Window Layer) ✅ |

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
| Combo vs ataque isolado | `combo.h` encapsula sequência; `attack.h` trata hitbox/dano — reutilizados por aerial/combo sem duplicação |
| Mira contínua | `aim_hold_tick` rate limiting via `Entity.timer`: ~10 ajustes/s sem campo extra |
| IA patrulha | `patrol_tick` detecta parede via `vx==0` pós `collision_move_x` — zero estado extra |
| HUD rendering | `#ifdef SGDK` isola VDP do host; `hud_draw` no-op no host, `VDP_drawText` no target |

---

## Convenções

- **Commit por US**: `USxx: descrição curta`
- **Aprovação**: Rafael aprova via comentário no card Planka antes do commit
- **Testes**: `make test` deve passar antes de todo commit
