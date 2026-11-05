#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <allegro5/allegro5.h>

typedef struct Mouse {
    float x;
    float y;
    bool clicou;
    bool mexeu;
} Mouse;

bool mouse_sobre(Mouse* mouse, float x, float y, float w, float h);
void iniciar_mouse(Mouse* mouse);
void atualizar_mouse(Mouse* mouse, ALLEGRO_EVENT* evento);
void limpar_mouse(Mouse* mouse);

#endif // !mouse