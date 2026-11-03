#include <stdio.h>

#include "personagem.h"
#include <allegro5/allegro5.h>

// Funções mover adicionam às velocidades dos personagens com bases nos inputs/IA e no ambiente
void mover_player(Personagem* player, char key[]) {
    if ((key[ALLEGRO_KEY_W] || key[ALLEGRO_KEY_SPACE]) && player->grounded) {
        player->grounded = false;
        player->vel_y = -50;
    }
    if (key[ALLEGRO_KEY_A])
        player->vel_x = VEL_PERSONAGENS * -1;
    if (key[ALLEGRO_KEY_D])
        player->vel_x = VEL_PERSONAGENS;
    if ((key[ALLEGRO_KEY_D] || key[ALLEGRO_KEY_A]) && key[ALLEGRO_KEY_LSHIFT])
        player->vel_x *= 2;
    if ((key[ALLEGRO_KEY_D] || key[ALLEGRO_KEY_A]) && key[ALLEGRO_KEY_LCTRL])
        player->vel_x *= 0.5;
    if (key[ALLEGRO_KEY_E]) {
        //Cenario jogador = { spy.x, spy.y, spy.tam_x, spy.tam_y };
        //coletar_cenario(jogador);
    }
}

void mover_NPC(NPC* npc) {
    if (!npc->p.grounded && npc->p.vel_y <= VEL_TERMINAL)
        npc->p.vel_y += GRAVIDADE;
    if (npc->p.grounded) {
        npc->p.vel_x -= npc->p.vel_x / 2.5;
    }
}

// Função renderizar adicionam as velocidades em 
