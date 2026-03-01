/*
 * pool.c — Implementação do entity pool estático.
 */
#include "pool.h"
#include <string.h>

Entity g_entities[MAX_ENTITIES];
u8     g_pool_free[MAX_ENTITIES];
u8     g_pool_free_count;

void pool_init(void) {
    u8 i;
    memset(g_entities, 0, sizeof(g_entities));
    g_pool_free_count = MAX_ENTITIES;
    /* Empilha índices em ordem inversa: topo = 0, fundo = MAX_ENTITIES-1 */
    for (i = 0; i < MAX_ENTITIES; i++) {
        g_pool_free[i] = (u8)(MAX_ENTITIES - 1u - i);
    }
}

s8 pool_alloc(void) {
    u8 idx;
    if (g_pool_free_count == 0) return -1;
    g_pool_free_count--;
    idx = g_pool_free[g_pool_free_count];
    entity_set_flag(&g_entities[idx], FLAG_ACTIVE);
    return (s8)idx;
}

void pool_free(u8 idx) {
    entity_clr_flag(&g_entities[idx], FLAG_ACTIVE);
    g_pool_free[g_pool_free_count] = idx;
    g_pool_free_count++;
}
