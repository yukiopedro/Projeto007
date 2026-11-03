#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>

const float GRAVIDADE = 10;
const float VEL_TERMINAL = 50;
const float VEL_PERSONAGENS = 8;

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

typedef struct NPC {
    Personagem p;
    float cone_w;
    float cone_h;
} NPC;

void mover_player(Personagem* player, char key[]);
void mover_NPC(NPC* npc);

#endif // !personagem
