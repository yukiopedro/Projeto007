#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>

#define GRAVIDADE 10
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
    float cone_w;
    float cone_h;
    int tipo;
} NPC;

void criar_npc(NPC* endereco, float x, float y, int tipo);
void mover_player(Personagem* player, char key[]);
void mover_NPC(NPC* npc);

#endif // !personagem
