#ifndef TELAS_H
#define TELAS_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "menu.h"

#define TELA_MENU 0
#define TELA_JOGO 1
#define TELA_INSTRUCOES 2
#define TELA_CREDITOS 3
#define TELA_PAUSA 4
#define TELA_OPCOES 5
#define TELA_DIARIO 6
#define TELA_GAMEOVER 7

#ifndef KEY_SEEN
#define KEY_SEEN 1
#endif

#ifndef KEY_DOWN
#define KEY_DOWN 2
#endif

#define MAX_LINHAS 10

typedef struct TelaTexto {
    const char* titulo;
    const char* esquerda[MAX_LINHAS];
    const char* direita[MAX_LINHAS];
    int total;
} TelaTexto;

#define TOTAL_DOCUMENTOS 3
#define AVISO_FONTE 12
#define AVISO_FRAMES 150
#define AVISO_LINHAS 4

typedef struct Diario {
    const char* contexto;
    const char* titulo[TOTAL_DOCUMENTOS];
    const char* texto[TOTAL_DOCUMENTOS];
    bool coletado[TOTAL_DOCUMENTOS];
    int aviso;
    ALLEGRO_FONT* fonte;
    ALLEGRO_FONT* fonte_aviso;
    ALLEGRO_FONT* fonte_titulo;
} Diario;

bool iniciar_diario(Diario* diario, ALLEGRO_FONT* fonte_titulo);
void reiniciar_diario(Diario* diario);
void coletar_documento(Diario* diario, int numero);
void atualizar_aviso(Diario* diario);
void desenhar_aviso(Diario* diario, float disp_x);
void desenhar_diario(Diario* diario, float disp_x);

int atualizar_telas(int tela, Menu* menu, Menu* pausa, Menu* gameover, unsigned char* key, Mouse* mouse, float disp_x, bool* done);
void iniciar_tela_texto(TelaTexto* tela, const char* titulo);
void add_linha(TelaTexto* tela, const char* esquerda, const char* direita);
void iniciar_instrucoes(TelaTexto* tela);
void iniciar_creditos(TelaTexto* tela);
void desenhar_tela_texto(TelaTexto* tela, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, float disp_x);
void limpar_teclas(unsigned char* key);
void desenhar_telas(int tela, Menu* menu, Menu* pausa, TelaTexto* instrucoes, TelaTexto* creditos, Diario* diario, float disp_x);

#endif // !telas