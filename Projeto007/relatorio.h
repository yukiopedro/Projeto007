#ifndef RELATORIO_H
#define RELATORIO_H

#include <stdbool.h>
#include "menu.h"
#include "pontuacao.h"

#define TOTAL_PERGUNTAS 3
#define TOTAL_ALTERNATIVAS 4

typedef struct Pergunta {
    const char* enunciado;
    const char* alternativas[TOTAL_ALTERNATIVAS];
    int correta;
} Pergunta;

typedef struct Relatorio {
    Pergunta perguntas[TOTAL_PERGUNTAS];
    int atual;
    bool respondeu;
    int escolhida;
    bool fim;
} Relatorio;

void iniciar_perguntas(Menu* alternativas, Menu* menu);
void iniciar_relatorio(Relatorio* relatorio);
void carregar_pergunta(Relatorio* relatorio, Menu* alternativas);
void reiniciar_relatorio(Relatorio* relatorio, Menu* alternativas);
int atualizar_relatorio(Relatorio* relatorio, Menu* alternativas, Partida* partida, unsigned char* key, Mouse* mouse, float disp_x);
void desenhar_relatorio(Relatorio* relatorio, Menu* alternativas, Partida* partida, float disp_x, float disp_y);

#endif // !relatorio