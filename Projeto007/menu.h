#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>

#define TOTAL_OPCOES 5
#define MENU_KEY_SEEN 1

#define ACAO_NENHUMA 0
#define ACAO_JOGAR 1
#define ACAO_SAIR 2
#define ACAO_INSTRUCOES 3
#define ACAO_CREDITOS 4

typedef struct Menu {
    int opcao;
    int total;
    const char* nomes[TOTAL_OPCOES];
    ALLEGRO_FONT* fonte_titulo;
    ALLEGRO_FONT* fonte_botao;
} Menu;

bool iniciar_menu(Menu* menu);
int atualizar_menu(Menu* menu, unsigned char* key);
void desenhar_menu(Menu* menu, float disp_x);
void destruir_menu(Menu* menu);

#endif // !menu