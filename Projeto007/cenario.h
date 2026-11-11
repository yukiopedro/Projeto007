#ifndef CENARIO_H
#define CENARIO_H

#include "personagem.h"

#define MAX_PLATAFORMAS 15
#define ALCANCE_COLETA  20.0f
#define ALTURA_CHAO 30
#define ALTURA_TELA 720
#define LOCAL_SPRITE_DOCUMENTO "assets/documento.png"
#define SPRITE_DOC 32
#define LOCAL_SPRITE_PLATAFORMA "assets/plataforma.png"
#define SPRITE_PLAT 16

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
int coletar_cenario(Cenario jogador);
void renderizar_plataformas(ALLEGRO_BITMAP* sprite);
void colisao_plataforma(Personagem* jogador, float y_anterior);
void renderizar_documentos(ALLEGRO_BITMAP* sprite);
bool chegou_saida(Cenario jogador);
void desenhar_saida();

#endif // !cenario
