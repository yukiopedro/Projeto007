#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "telas.h"

void iniciar_tela_texto(TelaTexto* tela, const char* titulo) { // define o título e zera a quantidade de linhas
    tela->titulo = titulo;
    tela->total = 0;
}

void add_linha(TelaTexto* tela, const char* esquerda, const char* direita) { // acrescenta uma linha na esquerda e direita na tela de texto
    if (tela->total >= MAX_LINHAS) {
        return;
    }
    tela->esquerda[tela->total] = esquerda;
    tela->direita[tela->total] = direita;
    tela->total++;
}

void iniciar_instrucoes(TelaTexto* tela) { // preenche a tela de controles
    iniciar_tela_texto(tela, "CONTROLES");
    add_linha(tela, "A / D", "ANDAR");
    add_linha(tela, "W / ESPAÇO", "PULAR");
    add_linha(tela, "SHIFT", "CORRER");
    add_linha(tela, "CTRL", "ANDAR DEVAGAR");
    add_linha(tela, "E", "INTERAGIR");
    add_linha(tela, "ESC", "PAUSAR");
}

void iniciar_creditos(TelaTexto* tela) { // preenche a tela de créditos
    iniciar_tela_texto(tela, "CRÉDITOS");
    add_linha(tela, "DAVI GERMANO", NULL);
    add_linha(tela, "PEDRO AUGUSTO", NULL);
    add_linha(tela, "PEDRO YUKIO", NULL);
}

void desenhar_tela_texto(TelaTexto* tela, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, float disp_x) { // desenha título e linhas
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(fonte_titulo, vermelho, disp_x / 2, 70, ALLEGRO_ALIGN_CENTER, tela->titulo);

    for (int i = 0; i < tela->total; i++) {
        float y = 200 + i * 45;

        if (tela->direita[i] == NULL) {
            al_draw_text(fonte_texto, branco, disp_x / 2, y, ALLEGRO_ALIGN_CENTER, tela->esquerda[i]);
        }
        else {
            al_draw_text(fonte_texto, vermelho, disp_x / 2 - 40, y, ALLEGRO_ALIGN_RIGHT, tela->esquerda[i]);
            al_draw_text(fonte_texto, branco, disp_x / 2 + 40, y, 0, tela->direita[i]);
        }
    }
    al_draw_text(fonte_texto, branco, disp_x / 2, 660, ALLEGRO_ALIGN_CENTER, "ESC PARA VOLTAR");
}

void limpar_teclas(unsigned char* key) { // apaga as teclas apertadas e mantém as teclas seguradas para cada aperto valer uma vez só
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        if (key[i] == (KEY_SEEN | KEY_DOWN)) {
            key[i] = KEY_DOWN;
        }
        else if (key[i] == KEY_SEEN) {
            key[i] = 0;
        }
    }
}

int atualizar_telas(int tela, Menu* menu, Menu* pausa, unsigned char* key, Mouse* mouse, float disp_x, bool* done) { // trata a tela atual e retorna a tela para onde o jogo vai
    if (tela == TELA_MENU) {
        int acao = atualizar_menu(menu, key, mouse, disp_x);

        if (acao == ACAO_JOGAR) {
            tela = TELA_JOGO;
        }
        if (acao == ACAO_INSTRUCOES) {
            tela = TELA_INSTRUCOES;
        }
        if (acao == ACAO_CREDITOS) {
            tela = TELA_CREDITOS;
        }
        if (acao == ACAO_SAIR) {
            *done = true;
        }
    }
    else if (tela == TELA_PAUSA) {
        int acao = atualizar_pausa(pausa, key, mouse, disp_x);

        if (acao == ACAO_CONTINUAR) {
            tela = TELA_JOGO;
        }
        if (acao == ACAO_VOLTAR_MENU) {
            tela = TELA_MENU;
        }
        if (acao == ACAO_SAIR) {
            *done = true;
        }
    }
    else if (tela == TELA_INSTRUCOES || tela == TELA_CREDITOS) {
        if (key[ALLEGRO_KEY_ESCAPE] & KEY_SEEN) {
            tela = TELA_MENU;
        }
    }

    limpar_teclas(key); // zera cliques e apertos para não valerem na proxima tela
    limpar_mouse(mouse);
    return tela;
}

void desenhar_telas(int tela, Menu* menu, Menu* pausa, TelaTexto* instrucoes, TelaTexto* creditos, float disp_x) { // desenha a tela atual
    if (tela == TELA_MENU) {
        desenhar_menu(menu, disp_x);
    }
    else if (tela == TELA_INSTRUCOES) {
        desenhar_tela_texto(instrucoes, menu->fonte_titulo, menu->fonte_botao, disp_x);
    }
    else if (tela == TELA_CREDITOS) {
        desenhar_tela_texto(creditos, menu->fonte_titulo, menu->fonte_botao, disp_x);
    }
}