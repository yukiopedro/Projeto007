#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>

#define GRAVIDADE 8
#define VEL_TERMINAL 50
#define VEL_PERSONAGENS 8

typedef struct Personagem {
    float x;
    float y;
    float tam_x;
    float tam_y;
    float vel_x;
    float vel_y;
    bool grounded;
    bool sentido;
    bool vivo;
} Personagem;

enum tipos_npc{
    GUARDA
};
typedef struct NPC {
    Personagem p;
    float cone_x;
    float cone_y;
    float cone_w;
    float cone_h;
    int tipo;
} NPC;

void criar_player(Personagem* player, float x, float y);
void inputs_player(Personagem* player, char key[]);
void criar_npc(NPC* endereco, float x, float y, int tipo);
void alinhar_lanterna(NPC* npc);
void mover_NPC(NPC* npc);
void colisao_lanterna(Personagem* jogador, NPC* inimigo);

#endif // !personagem
