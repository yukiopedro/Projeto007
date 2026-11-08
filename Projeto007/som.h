#ifndef SOM_H
#define SOM_H

#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

// para criar um som novo: 1) numero novo aqui, 2) aumentar TOTAL_SONS, 3) arquivo novo em iniciar_som
#define SOM_MOVER 0
#define SOM_CONFIRMAR 1
#define SOM_PULAR 2
#define SOM_GRANADA 3
#define SOM_DIARIO 4
#define SOM_COLETA 5
#define SOM_GAMEOVER 6
#define TOTAL_SONS 7

typedef struct Som {
    ALLEGRO_SAMPLE* efeitos[TOTAL_SONS];
    float volume;
    bool ativo;
} Som;

bool iniciar_som(Som* som);
void tocar_som(Som* som, int qual);

#endif // !som