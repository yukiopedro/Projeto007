#ifndef TELAS_H
#define TELAS_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include "menu.h"

#define TELA_MENU 0
#define TELA_JOGO 1
#define TELA_INSTRUCOES 2
#define TELA_CREDITOS 3
#define TELA_PAUSA 4

#ifndef KEY_SEEN
#define KEY_SEEN 1
#endif

#ifndef KEY_DOWN
#define KEY_DOWN 2
#endif

#define MAX_LINHAS 10

typedef struct TelaTexto {
    const char* titulo;
    const char* esquerda[MAX_LINHAS];
    const char* direita[MAX_LINHAS];
    int total;
} TelaTexto;

int atualizar_telas(int tela, Menu* menu, Menu* pausa, unsigned char* key, Mouse* mouse, float disp_x, bool* done);
void iniciar_tela_texto(TelaTexto* tela, const char* titulo);
void add_linha(TelaTexto* tela, const char* esquerda, const char* direita);
void iniciar_instrucoes(TelaTexto* tela);
void iniciar_creditos(TelaTexto* tela);
void desenhar_tela_texto(TelaTexto* tela, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, float disp_x);
void limpar_teclas(unsigned char* key);
void desenhar_telas(int tela, Menu* menu, Menu* pausa, TelaTexto* instrucoes, TelaTexto* creditos, float disp_x);

#endif // !telas