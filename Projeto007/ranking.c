#define _CRT_SECURE_NO_WARNINGS // fopen, fscanf e sprintf dão erro sem ele
#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "ranking.h"

void iniciar_ranking(Ranking* ranking, Menu* menu) { // prepara fontes e zera as listas
    ranking->fonte_titulo = menu->fonte_titulo;
    ranking->fonte = menu->fonte_botao;
    ranking->fonte_texto = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 16, 0);
    ranking->som = menu->som;
    ranking->fase_vista = 0;

    for (int i = 0; i < TOTAL_FASES; i++) {
        ranking->total[i] = 0;
    }
}

void carregar_ranking(Ranking* ranking) { // lê os arquivos de ranking. aqui consultei a IA para me ajudar
    for (int i = 0; i < TOTAL_FASES; i++) {
        char arquivo[32];
        sprintf(arquivo, "ranking%d.txt", i + 1);

        ranking->total[i] = 0;

        FILE* arq = fopen(arquivo, "r");
        if (arq == NULL) {
            continue;
        }

        char nome[TAM_NOME];
        int pontos;

        while (ranking->total[i] < MAX_RANKING && fscanf(arq, "%15s %d", nome, &pontos) == 2) {
            int n = ranking->total[i];

            copiar_nome(ranking->lista[i][n].nome, nome);
            ranking->lista[i][n].pontos = pontos;
            ranking->total[i]++;
        }
        fclose(arq);
    }
}

void salvar_ranking(Ranking* ranking, int fase) { // grava a lista da fase no arquivo, uma linha por registro. aqui consultei a IA para me ajudar
    char arquivo[32];
    sprintf(arquivo, "ranking%d.txt", fase + 1);

    FILE* arq = fopen(arquivo, "w");
    if (arq == NULL) {
        return;
    }

    for (int i = 0; i < ranking->total[fase]; i++) {
        fprintf(arq, "%s %d\n", ranking->lista[fase][i].nome, ranking->lista[fase][i].pontos);
    }
    fclose(arq);
}

bool ordenar_ranking(Ranking* ranking, int fase, const char* nome, int pontos) { // ideia do insertion sort
    int n = ranking->total[fase];
    int pos = 0;

    while (pos < n && ranking->lista[fase][pos].pontos >= pontos) { // acha a posição. em caso de empate, quem chegou primeiro fica na frente
        pos++;
    }

    if (pos >= MAX_RANKING) {
        return false;
    }

    if (n < MAX_RANKING) {
        n++;
    }

    for (int i = n - 1; i > pos; i--) {
        ranking->lista[fase][i] = ranking->lista[fase][i - 1];
    }

    copiar_nome(ranking->lista[fase][pos].nome, nome);
    ranking->lista[fase][pos].pontos = pontos;
    ranking->total[fase] = n;
    return true;
}

int atualizar_ranking(Ranking* ranking, unsigned char* key) { // ESC ou ENTER voltam ao menu
    if ((key[ALLEGRO_KEY_ESCAPE] & MENU_KEY_SEEN) || (key[ALLEGRO_KEY_ENTER] & MENU_KEY_SEEN)) {
        tocar_som(ranking->som, SOM_CONFIRMAR);
        return ACAO_VOLTAR_MENU;
    }
    return ACAO_NENHUMA;
}

void desenhar_ranking(Ranking* ranking, float disp_x, float disp_y) { // desenha o top 10 da fase que está sendo vista
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR cinza = al_map_rgb(120, 120, 120);

    int f = ranking->fase_vista;

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(ranking->fonte_titulo, vermelho, disp_x / 2, 40, ALLEGRO_ALIGN_CENTER, "RANKING");

    for (int i = 0; i < MAX_RANKING; i++) {
        float y = 185 + i * 44;

        if (i < ranking->total[f]) {
            ALLEGRO_COLOR cor = branco;

            if (i == 0) {
                cor = vermelho;
            }
            al_draw_textf(ranking->fonte, cor, 340, y, ALLEGRO_ALIGN_RIGHT, "%d.", i + 1);
            al_draw_text(ranking->fonte, cor, 400, y, 0, ranking->lista[f][i].nome);
            al_draw_textf(ranking->fonte, cor, 940, y, ALLEGRO_ALIGN_RIGHT, "%d", ranking->lista[f][i].pontos);
        }
        else {
            al_draw_textf(ranking->fonte, cinza, 340, y, ALLEGRO_ALIGN_RIGHT, "%d.", i + 1);
            al_draw_text(ranking->fonte, cinza, 400, y, 0, "---");
        }
    }

    al_draw_text(ranking->fonte_texto, cinza, disp_x / 2, 675, ALLEGRO_ALIGN_CENTER, "ENTER PARA VOLTAR AO MENU");
}