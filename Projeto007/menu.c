#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include "menu.h"

bool iniciar_menu(Menu* menu) { // carrega as fontes e preenche o menu principal
    if (!al_init_font_addon()) {
        return false;
    }
    if (!al_init_ttf_addon()) {
        return false;
    }

    menu->fonte_titulo = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 56, 0);
    menu->fonte_botao = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 24, 0);

    if (!menu->fonte_titulo || !menu->fonte_botao) {
        return false;
    }

    menu->opcao = 0;
    menu->som = NULL;
    menu->total = TOTAL_OPCOES;
    menu->y_inicial = 330;
    menu->espaco = 60;
    menu->titulo_cima = "ESPECTRO";
    menu->titulo_baixo = "VERMELHO";
    menu->nomes[0] = "JOGAR";
    menu->nomes[1] = "RANKING";
    menu->nomes[2] = "CONTROLES";
    menu->nomes[3] = "CRÉDITOS";
    menu->nomes[4] = "SAIR";

    return true;
}

void mover_opcao(Menu* menu, unsigned char* key) { // muda o botão escolhido com as setas cima e baixo
    if (key[ALLEGRO_KEY_UP] & MENU_KEY_SEEN) {
        menu->opcao--;

        if (menu->opcao < 0) {
            menu->opcao = menu->total - 1;
        }
        tocar_som(menu->som, SOM_MOVER);
    }
    if (key[ALLEGRO_KEY_DOWN] & MENU_KEY_SEEN) {
        menu->opcao++;

        if (menu->opcao >= menu->total) {
            menu->opcao = 0;
        }
        tocar_som(menu->som, SOM_MOVER);
    }
}

int botao_sob_mouse(Menu* menu, Mouse* mouse, float disp_x) { // retorna o número do botão que está embaixo do mouse, ou -1 se não tiver nenhum
    for (int i = 0; i < menu->total; i++) {
        float y = menu->y_inicial + i * menu->espaco;
        float larg = al_get_text_width(menu->fonte_botao, menu->nomes[i]);

        if (mouse_sobre(mouse, disp_x / 2 - larg / 2, y - 8, larg, 40)) {
            return i;
        }
    }
    return -1;
}

bool escolher_opcao(Menu* menu, unsigned char* key, Mouse* mouse, float disp_x) { // teclado e mouse mudam o botão escolhido. retorna true quando ele é confirmado
    mover_opcao(menu, key);

    int sob = botao_sob_mouse(menu, mouse, disp_x);
    if (sob != -1) {
        if (mouse->mexeu) { // só muda o botão quando o mouse se mexe pra não conflitar com as setas
            if (menu->opcao != sob) {
                tocar_som(menu->som, SOM_MOVER);
            }
            menu->opcao = sob;
        }
        if (mouse->clicou) {
            menu->opcao = sob;
            tocar_som(menu->som, SOM_CONFIRMAR);
            return true;
        }
    }

    if (key[ALLEGRO_KEY_ENTER] & MENU_KEY_SEEN) {
        tocar_som(menu->som, SOM_CONFIRMAR);
        return true;
    }
    return false;
}

int atualizar_menu(Menu* menu, unsigned char* key, Mouse* mouse, float disp_x) { // confere o que o jogador escolheu no menu principal
    if (escolher_opcao(menu, key, mouse, disp_x)) {
        if (menu->opcao == 0) {
            return ACAO_JOGAR;
        }
        if (menu->opcao == 1) {
            return ACAO_RANKING;
        }
        if (menu->opcao == 2) {
            return ACAO_INSTRUCOES;
        }
        if (menu->opcao == 3) {
            return ACAO_CREDITOS;
        }
        if (menu->opcao == 4) {
            return ACAO_SAIR;
        }
    }
    return ACAO_NENHUMA;
}

void desenhar_menu(Menu* menu, float disp_x) { // desenha o título e os botões
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR cinza = al_map_rgb(120, 120, 120);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(menu->fonte_titulo, cinza, disp_x / 2, 100, ALLEGRO_ALIGN_CENTER, menu->titulo_cima);
    al_draw_text(menu->fonte_titulo, vermelho, disp_x / 2, 180, ALLEGRO_ALIGN_CENTER, menu->titulo_baixo);

    for (int i = 0; i < menu->total; i++) {
        float y = menu->y_inicial + i * menu->espaco;
        ALLEGRO_COLOR cor = branco;

        if (i == menu->opcao) {
            cor = vermelho;
        }
        al_draw_text(menu->fonte_botao, cor, disp_x / 2, y, ALLEGRO_ALIGN_CENTER, menu->nomes[i]);
    }
}

void iniciar_pausa(Menu* pausa, Menu* menu) { // monta o menu de pausa
    pausa->fonte_titulo = menu->fonte_titulo;
    pausa->fonte_botao = menu->fonte_botao;
    pausa->som = menu->som;

    pausa->titulo_cima = "";
    pausa->titulo_baixo = "PAUSADO";

    pausa->opcao = 0;
    pausa->total = 4;
    pausa->y_inicial = 290;
    pausa->espaco = 70;
    pausa->nomes[0] = "CONTINUAR";
    pausa->nomes[1] = "CONTROLES";
    pausa->nomes[2] = "VOLTAR AO MENU";
    pausa->nomes[3] = "SAIR PARA ÁREA DE TRABALHO";
}

int atualizar_pausa(Menu* pausa, unsigned char* key, Mouse* mouse, float disp_x) { // confere o que o jogador escolheu na pausa
    if (escolher_opcao(pausa, key, mouse, disp_x)) {
        if (pausa->opcao == 0) {
            return ACAO_CONTINUAR;
        }
        if (pausa->opcao == 1) {
            return ACAO_INSTRUCOES;
        }
        if (pausa->opcao == 2) {
            return ACAO_VOLTAR_MENU;
        }
        if (pausa->opcao == 3) {
            return ACAO_SAIR;
        }
    }

    if (key[ALLEGRO_KEY_ESCAPE] & MENU_KEY_SEEN) {
        return ACAO_CONTINUAR;
    }

    return ACAO_NENHUMA;
}

void desenhar_pausa(Menu* pausa, float disp_x, float disp_y) { // desenha o quadrado da pausa e os botões
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR preto = al_map_rgb(0, 0, 0);

    float x1 = disp_x / 2 - 380;
    float x2 = disp_x / 2 + 380;
    float y1 = 120;
    float y2 = 600;

    al_draw_filled_rectangle(x1, y1, x2, y2, preto);
    al_draw_rectangle(x1, y1, x2, y2, branco, 4);

    al_draw_text(pausa->fonte_titulo, vermelho, disp_x / 2, y1 + 40, ALLEGRO_ALIGN_CENTER, pausa->titulo_baixo);

    for (int i = 0; i < pausa->total; i++) {
        float y = pausa->y_inicial + i * pausa->espaco;
        ALLEGRO_COLOR cor = branco;

        if (i == pausa->opcao) {
            cor = vermelho;
        }
        al_draw_text(pausa->fonte_botao, cor, disp_x / 2, y, ALLEGRO_ALIGN_CENTER, pausa->nomes[i]);
    }
}

void iniciar_gameover(Menu* gameover, Menu* menu) { // monta o menu da tela de game over
    gameover->fonte_titulo = menu->fonte_titulo;
    gameover->fonte_botao = menu->fonte_botao;
    gameover->som = menu->som;

    gameover->titulo_cima = "";
    gameover->titulo_baixo = "GAME OVER";

    gameover->opcao = 0;
    gameover->total = 2;
    gameover->y_inicial = 460;
    gameover->espaco = 70;
    gameover->nomes[0] = "TENTAR NOVAMENTE";
    gameover->nomes[1] = "VOLTAR AO MENU";
}

int atualizar_gameover(Menu* gameover, unsigned char* key, Mouse* mouse, float disp_x) { // confere o que o jogador escolheu no game over
    if (escolher_opcao(gameover, key, mouse, disp_x)) {
        if (gameover->opcao == 0) {
            return ACAO_TENTAR_NOVAMENTE;
        }
        if (gameover->opcao == 1) {
            return ACAO_VOLTAR_MENU;
        }
    }
    return ACAO_NENHUMA;
}

void desenhar_gameover(Menu* gameover, Partida* partida, float disp_x, float disp_y) { // desenha o quadrado do game over, a mensagem e os botões
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR preto = al_map_rgb(0, 0, 0);

    float x1 = disp_x / 2 - 440;
    float x2 = disp_x / 2 + 440;
    float y1 = 80;
    float y2 = 640;

    al_draw_filled_rectangle(x1, y1, x2, y2, preto);
    al_draw_rectangle(x1, y1, x2, y2, branco, 4);

    al_draw_text(gameover->fonte_titulo, vermelho, disp_x / 2, y1 + 50, ALLEGRO_ALIGN_CENTER, gameover->titulo_baixo);

    al_draw_text(gameover->fonte_botao, branco, disp_x / 2, 230, ALLEGRO_ALIGN_CENTER, "VOCÊ FOI DETECTADO.");
    al_draw_text(gameover->fonte_botao, branco, disp_x / 2, 265, ALLEGRO_ALIGN_CENTER, "SUA ESPIONAGEM FALHOU.");

    int segundos = partida->frames / 30;
    al_draw_textf(gameover->fonte_botao, branco, disp_x / 2, 325, ALLEGRO_ALIGN_CENTER, "TEMPO: %02d:%02d", segundos / 60, segundos % 60);
    al_draw_textf(gameover->fonte_botao, branco, disp_x / 2, 365, ALLEGRO_ALIGN_CENTER, "PONTUAÇÃO: %d", partida->pontuacao);

    for (int i = 0; i < gameover->total; i++) {
        float y = gameover->y_inicial + i * gameover->espaco;
        ALLEGRO_COLOR cor = branco;

        if (i == gameover->opcao) {
            cor = vermelho;
        }
        al_draw_text(gameover->fonte_botao, cor, disp_x / 2, y, ALLEGRO_ALIGN_CENTER, gameover->nomes[i]);
    }
}