#include <stdio.h>

#include "personagem.h"
#include "cenario.h"
#include <allegro5/allegro5.h>



void criar_player(Personagem* player, float x, float y) {
    Personagem novo_player = {x, y, 22, 22, 0, 0, SPY, false, true, true};
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
    if (key[ALLEGRO_KEY_E]) {
        Cenario jogador = {player->x, player->y, player->tam_x, player->tam_y };
        coletar_cenario(jogador);
    }
}

void mover_player(Personagem* p) {
    p->x += p->vel_x;
    p->y += p->vel_y;
    if (p->grounded && p->vel_x != 0) p->vel_x -= p->vel_x / ATRITO;
}

void criar_npc(NPC* endereco, float x, float y, int tipo) {
    NPC novo_npc = { {x, y, 20, 20, 0, 0, tipo, false, true, true}, 0, 0, 100, 30};
    alinhar_lanterna(&novo_npc);
    *endereco = novo_npc;
}

void alinhar_lanterna(NPC* npc) {
    npc->cone_x = npc->p.x + ((npc->p.sentido) * npc->p.tam_x) - (!(npc->p.sentido) * npc->cone_w);
    npc->cone_y = npc->p.y - npc->cone_h + npc->p.tam_y;
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
    alinhar_lanterna(npc);
    //TODO: trocar teste por uma fuñção de colisão com paredes, quando existir
    if (npc->p.x <= 0 || npc->p.x >= 1260-npc->p.tam_x) {
        npc->p.x -= npc->p.vel_x;
        npc->p.sentido = !npc->p.sentido;
    }
}

void colisao_lanterna(Personagem* player, NPC* inimigo) {
    if (player->x + player->tam_x <= inimigo->cone_x) return; //borda esquerda
    if (player->x >= inimigo->cone_x + inimigo->cone_w) return; //borda direita
    if (player->y + player->tam_y <= inimigo->cone_y) return; // borda superior
    if (player->y >= inimigo->cone_y + inimigo->cone_h) return; // borda inferior
    player->vivo = false;
}

// Função que renderizam personagens
void renderizar_personagem(Personagem* p, ALLEGRO_BITMAP* spritesheet, int frame) {
    int estado = PARADO;
    if (p->vel_y != 0) estado = PULANDO;
    else if (p->vel_x > 0.1 || p->vel_x < -0.1) {
        if (frame % 30 < 15) estado = ANDANDO1;
        else estado = ANDANDO2;
    }
    int bandeira = 0;
    if (!p->sentido) bandeira = ALLEGRO_FLIP_HORIZONTAL;

    al_draw_scaled_bitmap(
        spritesheet, 
        estado * SPRITE_W, p->tipo * SPRITE_H, SPRITE_W, SPRITE_H,
        p->x, p->y, p->tam_x, p->tam_y,
        bandeira);
}
void renderizar_lanterna(NPC* n, ALLEGRO_BITMAP* spritesheet) {
    int bandeira = 0;
    if (!n->p.sentido) bandeira = ALLEGRO_FLIP_HORIZONTAL;

    al_draw_scaled_bitmap(
        spritesheet,
        0, LANTERNA * SPRITE_H, SPRITE_W * 2, SPRITE_H,
        n->cone_x, n->cone_y, n->cone_w, n->cone_h,
        bandeira);
}

