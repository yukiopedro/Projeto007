#include <allegro5/allegro5.h>
#include "mouse.h"

void iniciar_mouse(Mouse* mouse) { // zera a posição, clique e movimento do mouse
    mouse->x = 0;
    mouse->y = 0;
    mouse->clicou = false;
    mouse->mexeu = false;
}

void atualizar_mouse(Mouse* mouse, ALLEGRO_EVENT* evento) { // guarda a posição do mouse e avisa se ele se mexeu ou se o botão esquerdo foi clicado
    if (evento->type == ALLEGRO_EVENT_MOUSE_AXES) {
        mouse->x = evento->mouse.x;
        mouse->y = evento->mouse.y;
        mouse->mexeu = true;
    }

    if (evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento->mouse.button == 1) {
        mouse->x = evento->mouse.x;
        mouse->y = evento->mouse.y;
        mouse->clicou = true;
    }
}

void limpar_mouse(Mouse* mouse) { // apaga cliques e movimento no fim, para cada clique valer uma vez só
    mouse->clicou = false;
    mouse->mexeu = false;
}

bool mouse_sobre(Mouse* mouse, float x, float y, float w, float h) { // retorna true se o mouse está dentro do retângulo informado
    if (mouse->x >= x && mouse->x <= x + w) {
        if (mouse->y >= y && mouse->y <= y + h) {
            return true;
        }
    }
    return false;
}