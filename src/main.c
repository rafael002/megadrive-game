/*
 * main.c — Entry point do jogo.
 *
 * Loop principal sincronizado com VSync via SYS_doVBlankProcess().
 * Toda lógica de jogo deve terminar antes do próximo VSync (16.6ms @ 60Hz).
 */
#include <genesis.h>
#include "config.h"
#include "test.h"

int main(void) {
    /* Inicialização do VDP */
    VDP_setScreenWidth320();

    while (1) {
        /* --- lógica de jogo (cada US adiciona aqui) --- */

        /* Aguarda VSync, processa DMA e lê joypads */
        SYS_doVBlankProcess();
    }

    return 0;
}
