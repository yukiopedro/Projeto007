#ifndef RANKING_H
#define RANKING_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include "menu.h"
#include "fases.h"

#define MAX_RANKING 10

typedef struct Registro {
    char nome[TAM_NOME];
    int pontos;
} Registro;

typedef struct Ranking {
    Registro lista[TOTAL_FASES][MAX_RANKING];
    int total[TOTAL_FASES];
    int fase_vista;
    ALLEGRO_FONT* fonte_titulo;
    ALLEGRO_FONT* fonte;
    ALLEGRO_FONT* fonte_texto;
    Som* som;
} Ranking;

void iniciar_ranking(Ranking* ranking, Menu* menu);
void carregar_ranking(Ranking* ranking);
void salvar_ranking(Ranking* ranking, int fase);
bool ordenar_ranking(Ranking* ranking, int fase, const char* nome, int pontos);
int atualizar_ranking(Ranking* ranking, unsigned char* key);
void desenhar_ranking(Ranking* ranking, float disp_x, float disp_y);

#endif // !ranking