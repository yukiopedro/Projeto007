#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>
#include <allegro5/allegro5.h>

// Tamanho em pixels dos spirtes dos personagens
#define SPRITE_H 8
#define SPRITE_W 8
// Spritesheet
#define LOCAL_SPRITESHEET "assets/spritesheet_teste.png"
// Constantes de velocidaade
#define GRAVIDADE 8
#define VEL_TERMINAL 50
#define VEL_PERSONAGENS 8
#define ATRITO 2.0f

enum estado_personagem {
    PARADO,
    ANDANDO1,
    ANDANDO2,
    PULANDO,
    ATIRANDO
};

typedef struct Personagem {
    float x;
    float y;
    float tam_x;
    float tam_y;
    float vel_x;
    float vel_y;
    int tipo;
    bool grounded;
    bool sentido;
    bool vivo;
} Personagem;

enum tipos{
    SPY,
    GUARDA,
    LANTERNA
};
typedef struct NPC {
    Personagem p;
    float cone_x;
    float cone_y;
    float cone_w;
    float cone_h;
} NPC;

void criar_player(Personagem* player, float x, float y);
void inputs_player(Personagem* player, char key[]);
void mover_player(Personagem* p);

void criar_npc(NPC* endereco, float x, float y, int tipo);
void alinhar_lanterna(NPC* npc);
void mover_NPC(NPC* npc);
void colisao_lanterna(Personagem* jogador, NPC* inimigo);

void renderizar_personagem(Personagem* p, ALLEGRO_BITMAP* spritesheet, int frame);
void renderizar_lanterna(NPC* n, ALLEGRO_BITMAP* spritesheet);

#endif // !personagem
