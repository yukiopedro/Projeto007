#include <stdio.h>

#include "personagem.h"
#include <allegro5/allegro5.h>

extern float gravidade;

void mover(Personagem* player, char key[]) {
    if ((key[ALLEGRO_KEY_W] || key[ALLEGRO_KEY_SPACE]) && player->grounded) {
        player->grounded = false;
        player->vel_y = -50;
    }
    if (key[ALLEGRO_KEY_A])
        player->vel_x = -8;
    if (key[ALLEGRO_KEY_D])
        player->vel_x = 8;
    if ((key[ALLEGRO_KEY_D] || key[ALLEGRO_KEY_A]) && key[ALLEGRO_KEY_LSHIFT])
        player->vel_x *= 2;
    if ((key[ALLEGRO_KEY_D] || key[ALLEGRO_KEY_A]) && key[ALLEGRO_KEY_LCTRL])
        player->vel_x *= 0.5;
    if (key[ALLEGRO_KEY_E]) {
        //Cenario jogador = { spy.x, spy.y, spy.tam_x, spy.tam_y };
        //coletar_cenario(jogador);
    }

}
