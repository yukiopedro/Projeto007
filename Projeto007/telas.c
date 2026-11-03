#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "telas.h"

void iniciar_tela_texto(TelaTexto* tela, const char* titulo) {
    tela->titulo = titulo;
    tela->total = 0;
}

void add_linha(TelaTexto* tela, const char* esquerda, const char* direita) {
    if (tela->total >= MAX_LINHAS) {
        return;
    }
    tela->esquerda[tela->total] = esquerda;
    tela->direita[tela->total] = direita;
    tela->total++;
}

void iniciar_instrucoes(TelaTexto* tela) {
    iniciar_tela_texto(tela, "INSTRUÇÕES");
    add_linha(tela, "A / D", "ANDAR");
    add_linha(tela, "W / ESPAÇO", "PULAR");
    add_linha(tela, "SHIFT", "CORRER");
    add_linha(tela, "CTRL", "ANDAR DEVAGAR");
    add_linha(tela, "E", "INTERAGIR");
    add_linha(tela, "ESC", "SAIR DO JOGO");
}

void iniciar_creditos(TelaTexto* tela) {
    iniciar_tela_texto(tela, "CRÉDITOS");
    add_linha(tela, "DAVI GERMANO", NULL);
    add_linha(tela, "PEDRO AUGUSTO", NULL);
    add_linha(tela, "PEDRO YUKIO", NULL);
}

void desenhar_tela_texto(TelaTexto* tela, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, float disp_x) {
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

void limpar_teclas(unsigned char* key) {
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        if (key[i] == (KEY_SEEN | KEY_DOWN)) {
            key[i] = KEY_DOWN;
        }
        else if (key[i] == KEY_SEEN) {
            key[i] = 0;
        }
    }
}

int atualizar_telas(int tela, Menu* menu, unsigned char* key, bool* done) {
    if (tela == TELA_MENU) {
        int acao = atualizar_menu(menu, key);

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
    else if (tela == TELA_INSTRUCOES || tela == TELA_CREDITOS) {
        if (key[ALLEGRO_KEY_ESCAPE] & KEY_SEEN) {
            tela = TELA_MENU;
        }
    }

    limpar_teclas(key);
    return tela;
}

void desenhar_telas(int tela, Menu* menu, TelaTexto* instrucoes, TelaTexto* creditos, float disp_x) {
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