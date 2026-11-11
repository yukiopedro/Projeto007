#include <stdio.h>
#include "som.h"

bool iniciar_som(Som* som) { // liga o áudio e carrega todos os efeitos da lista
    const char* arquivos[TOTAL_SONS] = {
        "assets/mover.wav",
        "assets/confirmar.wav",
        "assets/pulo.wav",
        "assets/jogar_granada.wav",
        "assets/diario.wav",
        "assets/coletar.wav",
        "assets/gameover.wav",
        "assets/gg.wav",
    };

    som->volume = 0.3f;
    som->ativo = false;

    for (int i = 0; i < TOTAL_SONS; i++) {
        som->efeitos[i] = NULL;
    }

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(8);

    for (int i = 0; i < TOTAL_SONS; i++) {
        som->efeitos[i] = al_load_sample(arquivos[i]);

        if (!som->efeitos[i]) {
            printf("nao achou %s\n", arquivos[i]);
        }
    }

    som->ativo = true;
    return true;
}

void tocar_som(Som* som, int qual) { // toca o efeito pedido
    if (som == NULL || !som->ativo) {
        return;
    }
    if (qual < 0 || qual >= TOTAL_SONS || som->efeitos[qual] == NULL) {
        return;
    }

    al_play_sample(som->efeitos[qual], som->volume, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
}