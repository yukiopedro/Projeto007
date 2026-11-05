#include <stdio.h>

#include "personagem.h"
#include <allegro5/allegro5.h>

void criar_npc(NPC* endereco, float x, float y, int tipo) {
    NPC novo_npc = { {x, y, 30, 30, 0, 0, 0, 0, 1}, 50, 30, tipo };
    *endereco = novo_npc;
}

// Acelera o protagonista
void mover_player(Personagem* player, char key[]) {
    if (player->grounded)
        player->vel_x /= 1.5;
    else if (player->vel_y <= VEL_TERMINAL)
        player->vel_y += GRAVIDADE;

    if ((key[ALLEGRO_KEY_W] || key[ALLEGRO_KEY_SPACE]) && player->grounded) {
        player->grounded = false;
        player->vel_y = -50;
    }
    if (key[ALLEGRO_KEY_A]) {
        player->vel_x = VEL_PERSONAGENS * -1;
        player->sentido = 0;
    }
    if (key[ALLEGRO_KEY_D]) {
        player->vel_x = VEL_PERSONAGENS;
        player->sentido = 1;
    }
    if ((key[ALLEGRO_KEY_D] || key[ALLEGRO_KEY_A]) && key[ALLEGRO_KEY_LSHIFT])
        player->vel_x *= 2;
    if ((key[ALLEGRO_KEY_D] || key[ALLEGRO_KEY_A]) && key[ALLEGRO_KEY_LCTRL])
        player->vel_x *= 0.5;
    if (key[ALLEGRO_KEY_E]) {
        //Cenario jogador = { spy.x, spy.y, spy.tam_x, spy.tam_y };
        //coletar_cenario(jogador);
    }

}
// Acelera o NPC
void mover_NPC(NPC* npc) {
    if (!npc->p.grounded && npc->p.vel_y <= VEL_TERMINAL)
        npc->p.vel_y += GRAVIDADE;
    if (npc->p.grounded) {
        npc->p.vel_x = VEL_PERSONAGENS * (npc->p.sentido ? 1 : -1);
    }
    if (npc->p.x <= 0) {
        npc->p.sentido = !npc->p.sentido;
    }
}

// Funções que renderizam personagens e movem personagens
