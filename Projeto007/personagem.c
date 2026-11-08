#include <stdio.h>

#include "personagem.h"
#include "cenario.h"
#include <allegro5/allegro5.h>

void criar_player(Personagem* player, float x, float y) {
    Personagem novo_player = {x, y, 22, 22, 0, 0, false, true, true};
    *player = novo_player;
}
// Aceita os inputs
void inputs_player(Personagem* player, char key[]) {
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
}

void criar_npc(NPC* endereco, float x, float y, int tipo) {
    NPC novo_npc = { {x, y, 20, 20, 0, 0, false, false, true}, 0, 0, 100, 30, tipo };
    alinhar_lanterna(&novo_npc);
    *endereco = novo_npc;
}

void alinhar_lanterna(NPC* npc) {
    npc->cone_x = npc->p.x + !(npc->p.sentido) * npc->p.tam_x - !(npc->p.sentido) * npc->cone_w;
    npc->cone_y = npc->p.y - npc->cone_h + npc->p.tam_y;
    npc->cone_w *= npc->p.sentido? 1: -1;
}

// Move o NPC
void mover_NPC(NPC* npc) {
    if (!npc->p.grounded && npc->p.vel_y <= VEL_TERMINAL)
        npc->p.vel_y += GRAVIDADE;
    if (npc->p.grounded) {
        npc->p.vel_x = VEL_PERSONAGENS * (npc->p.sentido ? 1 : -1);
    }

    npc->p.x += npc->p.vel_x;
    npc->p.y += npc->p.vel_y;
    npc->cone_x += npc->p.vel_x;
    npc->cone_y += npc->p.vel_y;
    //TODO: trocar teste por uma fuñção de colisão com paredes, quando existir
    if (npc->p.x <= 0 || npc->p.x >= 1260-npc->p.tam_x) {
        alinhar_lanterna(npc);
        npc->p.x -= npc->p.vel_x;
        npc->p.sentido = !npc->p.sentido;
    }
}

void colisao_lanterna(Personagem* player, NPC* inimigo) {
    if (player->x + player->tam_x <= inimigo->p.x) return; //borda esquerda
    if (player->x >= inimigo->p.x + inimigo->p.tam_x) return; //borda direita
    if (player->y + player->tam_y <= inimigo->p.y) return; // borda superior
    if (player->y >= inimigo->p.y + inimigo->p.tam_y) return; // borda inferior
    player->vivo = false;
    
}

// Funções que renderizam personagens e movem personagens

