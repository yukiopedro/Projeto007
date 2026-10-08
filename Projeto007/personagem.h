#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>

typedef struct Personagem {
    float x;
    float y;
    float tam_x;
    float tam_y;
    float vel_x;
    float vel_y;
    bool grounded;
} Personagem;

#endif // !personagem
