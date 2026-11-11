#ifndef GRANADA_H
#define GRANADA_H

#include <stdbool.h>
#include <allegro5/allegro5.h>
#include "personagem.h"

typedef struct GRANADA {
    float x, y, dx, dy;
    int frame;
    bool used;
    bool chao;
} GRANADA;

#define GRANADA_LARGURA 20.0f
#define GRANADA_ALTURA 20.0f
#define GRANADA_GRAVIDADE 1.0f
#define GRANADA_FADE_TICKS 30

extern bool jogar_solicitado;
extern ALLEGRO_BITMAP* imagem_granada;

void jogar_granada(GRANADA* g, const Personagem* spy);
void atualizar_granada(GRANADA* g);
void desenhar_granada(const GRANADA* g);


#endif // !GRANADA_H
