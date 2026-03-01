# Makefile — Mega Drive platformer (SGDK + host tests)
#
# Targets:
#   make test   — compila e roda a suite de testes no host
#   make clean  — remove artefatos de build

# ── Host test build ──────────────────────────────────────────────────────────
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -Itests

TEST_SRC = tests/run_tests.c
TEST_BIN = tests/run_tests

# ── SGDK build (requer GDK definido no ambiente) ─────────────────────────────
ifdef GDK
    include $(GDK)/makefile.gen
endif

# ── Targets ──────────────────────────────────────────────────────────────────
.PHONY: test clean

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) tests/framework.h tests/test_lut.c tests/test_fix.c \
             tests/test_entity.c tests/test_collision.c tests/test_pool.c \
             tests/test_physics.c tests/test_aabb.c tests/test_jump.c \
             tests/test_crouch.c tests/test_dash.c \
             inc/lut.h inc/fix.h inc/entity.h inc/collision.h inc/pool.h \
             inc/physics.h inc/config.h inc/dash.h \
             src/lut.c src/collision.c src/pool.c src/physics.c src/jump.c \
             src/crouch.c src/dash.c
	$(CC) $(CFLAGS) -Iinc -o $@ $(TEST_SRC)

clean:
	rm -f $(TEST_BIN)
