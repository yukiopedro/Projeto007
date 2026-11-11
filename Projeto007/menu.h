#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include "mouse.h"
#include "som.h"
#include "pontuacao.h"

#define TOTAL_OPCOES 5
#define MENU_KEY_SEEN 1

#define ACAO_NENHUMA 0
#define ACAO_JOGAR 1
#define ACAO_SAIR 2
#define ACAO_INSTRUCOES 3
#define ACAO_CREDITOS 4
#define ACAO_CONTINUAR 5
#define ACAO_VOLTAR_MENU 6
#define ACAO_TENTAR_NOVAMENTE 7

typedef struct Menu {
    int opcao;
    int total;
    float y_inicial;
    float espaco;
    const char* titulo_cima;
    const char* titulo_baixo;
    const char* nomes[TOTAL_OPCOES];
    ALLEGRO_FONT* fonte_titulo;
    ALLEGRO_FONT* fonte_botao;
    Som* som;
} Menu;

bool iniciar_menu(Menu* menu);
bool escolher_opcao(Menu* menu, unsigned char* key, Mouse* mouse, float disp_x);
int atualizar_menu(Menu* menu, unsigned char* key, Mouse* mouse, float disp_x);
int atualizar_pausa(Menu* pausa, unsigned char* key, Mouse* mouse, float disp_x);
int botao_sob_mouse(Menu* menu, Mouse* mouse, float disp_x);
int atualizar_gameover(Menu* gameover, unsigned char* key, Mouse* mouse, float disp_x);
void desenhar_menu(Menu* menu, float disp_x);
void mover_opcao(Menu* menu, unsigned char* key);
void iniciar_pausa(Menu* pausa, Menu* menu);
void desenhar_pausa(Menu* pausa, float disp_x, float disp_y);
void iniciar_gameover(Menu* gameover, Menu* menu);
void desenhar_gameover(Menu* gameover, Partida* partida, float disp_x, float disp_y);

#endif // !menu