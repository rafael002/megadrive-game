# Makefile — Mega Drive platformer (SGDK + host tests)
#
# Targets:
#   make test       — compila e roda a suite de testes no host (sem SGDK)
#   make rom        — compila a ROM via Docker (sem instalação local do SGDK)
#   make clean      — remove artefatos de testes
#   make clean-rom  — remove out/ (artefatos da ROM)
#
# ROM via SGDK local: export GDK=/path/to/sgdk && make

# ── Host test build ──────────────────────────────────────────────────────────
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -Itests

TEST_SRC = tests/run_tests.c
TEST_BIN = tests/run_tests

# ── ROM build via Docker (imagem oficial SGDK) ────────────────────────────────
SGDK_DOCKER ?= ghcr.io/stephane-d/sgdk:latest

# ── SGDK build local (requer GDK definido no ambiente) ────────────────────────
ifdef GDK
    include $(GDK)/makefile.gen
endif

# ── Targets ──────────────────────────────────────────────────────────────────
.PHONY: test rom clean clean-rom

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) tests/framework.h tests/test_lut.c tests/test_fix.c \
             tests/test_entity.c tests/test_collision.c tests/test_pool.c \
             tests/test_physics.c tests/test_aabb.c tests/test_jump.c \
             tests/test_crouch.c tests/test_dash.c tests/test_attack.c \
             tests/test_combo.c tests/test_aerial.c tests/test_aim.c \
             inc/lut.h inc/fix.h inc/entity.h inc/collision.h inc/pool.h \
             inc/physics.h inc/config.h inc/dash.h inc/attack.h inc/combo.h \
             inc/aerial.h inc/aim.h \
             src/lut.c src/collision.c src/pool.c src/physics.c src/jump.c \
             src/crouch.c src/dash.c src/attack.c src/combo.c src/aerial.c \
             src/aim.c
	$(CC) $(CFLAGS) -Iinc -o $@ $(TEST_SRC)

rom:
	docker run --rm -v "$(CURDIR)":/src $(SGDK_DOCKER)

clean:
	rm -f $(TEST_BIN)

clean-rom:
	rm -rf out/
