#ifndef FASES_H
#define FASES_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include "menu.h"

#define TOTAL_FASES 3

typedef struct Fases {
    int atual;
    bool existe[TOTAL_FASES];
    const char* nomes[TOTAL_FASES];
    const char* contexto[TOTAL_FASES];
    ALLEGRO_FONT* fonte_titulo;
    ALLEGRO_FONT* fonte;
    ALLEGRO_FONT* fonte_texto;
    Som* som;
    char nome[TAM_NOME];
    int tam_nome;
} Fases;

void iniciar_fases(Fases* fases, Menu* menu, const char* contexto_fase1);
int atualizar_fases(Fases* fases, unsigned char* key, Mouse* mouse, float disp_x);
void digitar_nome(Fases* fases, int unichar, int keycode);
void desenhar_fases(Fases* fases, float disp_x, float disp_y);
void reiniciar_nome(Fases* fases);

#endif // !fases