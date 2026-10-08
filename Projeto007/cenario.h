#ifndef CENARIO_H
#define CENARIO_H

#include "personagem.h"

#define MAX_PLATAFORMAS 10
#define ALCANCE_COLETA  20.0f
#define ALTURA_CHAO 30

extern bool coletado;
extern int documentos;

typedef struct Cenario {
    float x;
    float y;
    float w;
    float h;
} Cenario;

void add_plataforma(float x, float y, float w, float h);
bool se_tocam(Cenario a, Cenario b);
void iniciar_cenario(float largura, float altura);
bool colisao_cenario(Cenario r);
void coletar_cenario(Cenario jogador);
void desenhar_cenario(void);
void colisao_plataforma(Personagem* jogador, float y_anterior);


#endif // !cenario
