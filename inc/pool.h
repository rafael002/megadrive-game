/*
 * pool.h — Gerenciador estático de entidades (sem malloc).
 *
 * Free-list implementada como stack de u8 — alloc e free em O(1).
 * Tamanho fixo definido em config.h (MAX_ENTITIES).
 * Nenhuma alocação dinâmica; tudo em arrays estáticos em RAM.
 */
#pragma once

#include "entity.h"
#include "config.h"

/* ── Arrays estáticos ────────────────────────────────────────────────────── */
extern Entity g_entities[MAX_ENTITIES];    /* pool de entidades             */
extern u8     g_pool_free[MAX_ENTITIES];   /* stack de índices livres       */
extern u8     g_pool_free_count;           /* topo da stack (# slots livres)*/

/* ── API ─────────────────────────────────────────────────────────────────── */

/* Inicializa o pool — todos os slots livres, flags zeradas. */
void pool_init(void);

/*
 * pool_alloc — retira um índice da free-list, seta FLAG_ACTIVE e retorna
 * o índice. Retorna -1 (como s8) se o pool estiver cheio.
 */
s8 pool_alloc(void);

/*
 * pool_free — libera o slot idx: limpa FLAG_ACTIVE e devolve à free-list.
 * Não valida idx — responsabilidade do chamador.
 */
void pool_free(u8 idx);
