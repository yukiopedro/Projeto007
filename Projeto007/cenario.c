#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "cenario.h"
#include "personagem.h"
#include "pontuacao.h"

Cenario plataformas[MAX_PLATAFORMAS];
int n_plataformas = 0;

Cenario coletaveis[TOTAL_DOCUMENTOS_FASE];
bool coletados[TOTAL_DOCUMENTOS_FASE];

void add_plataforma(float x, float y, float w, float h) {
    if (n_plataformas >= MAX_PLATAFORMAS)
        return;

    plataformas[n_plataformas].x = x;
    plataformas[n_plataformas].y = y;
    plataformas[n_plataformas].w = w;
    plataformas[n_plataformas].h = h;
    n_plataformas++;
}

bool se_tocam(Cenario a, Cenario b) {
    if (a.x < b.x + b.w) {
        if (a.x + a.w > b.x) {
            if (a.y < b.y + b.h) {
                if (a.y + a.h > b.y) {
                    return true;
                }
            }
        }
    }
    return false;
}

void iniciar_cenario(float largura, float altura) {
    n_plataformas = 0;

    float topo_chao = altura - ALTURA_CHAO;
    float altura_apoio = 130;
    float altura_grande = 260;

    add_plataforma(0, topo_chao, largura, ALTURA_CHAO);
    add_plataforma(0, -20, largura, 20);
    add_plataforma(-20, 0, 20, altura);
    add_plataforma(largura, 0, 20, altura);
    add_plataforma(320, topo_chao - altura_apoio, 140, 12);
    add_plataforma(512, topo_chao - altura_grande, 400, 12);

    for (int i = 0; i < TOTAL_DOCUMENTOS_FASE; i++) {
        coletaveis[i].w = 14;
        coletaveis[i].h = 18;
        coletados[i] = false;
    }

    coletaveis[0].x = largura - 80;
    coletaveis[0].y = topo_chao - coletaveis[0].h;

    coletaveis[1].x = 820;
    coletaveis[1].y = topo_chao - altura_grande - coletaveis[1].h;

    coletaveis[2].x = 380;
    coletaveis[2].y = topo_chao - altura_apoio - coletaveis[2].h;
}

bool colisao_cenario(Cenario r) { // retorna true se o personagem bate em alguma plataforma (chão, teto e paredes)
    for (int i = 0; i < n_plataformas; i++)
        if (se_tocam(r, plataformas[i]))
            return true;
    return false;
}

int coletar_cenario(Cenario jogador) { // retorna o número do documento coletado
    Cenario alcance = { jogador.x - ALCANCE_COLETA, jogador.y - ALCANCE_COLETA, jogador.w + 2 * ALCANCE_COLETA, jogador.h + 2 * ALCANCE_COLETA };

    for (int i = 0; i < TOTAL_DOCUMENTOS_FASE; i++) {
        if (!coletados[i] && se_tocam(alcance, coletaveis[i])) {
            coletados[i] = true;
            return i;
        }
    }
    return -1;
}

void desenhar_cenario(void) {
    ALLEGRO_COLOR cor_plat = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR cor_borda = al_map_rgb(140, 145, 160);
    ALLEGRO_COLOR cor_papel = al_map_rgb(90, 95, 110);

    for (int i = 0; i < n_plataformas; i++) {
        al_draw_filled_rectangle(plataformas[i].x, plataformas[i].y, plataformas[i].x + plataformas[i].w, plataformas[i].y + plataformas[i].h, cor_plat);
        al_draw_rectangle(plataformas[i].x, plataformas[i].y, plataformas[i].x + plataformas[i].w, plataformas[i].y + plataformas[i].h, cor_borda, 1.0f);
    }

    for (int i = 0; i < TOTAL_DOCUMENTOS_FASE; i++) {
        if (!coletados[i]) {
            al_draw_filled_rectangle(coletaveis[i].x, coletaveis[i].y, coletaveis[i].x + coletaveis[i].w, coletaveis[i].y + coletaveis[i].h, cor_papel);
        }
    }
}

void colisao_plataforma(Personagem* jogador, float y_anterior) {
    jogador->grounded = false;

    if (jogador->vel_y < 0)
        return;

    for (int i = 0; i < n_plataformas; i++) {
        Cenario plataforma = plataformas[i];

        bool sobrepoe_plataforma =
            jogador->x < plataforma.x + plataforma.w &&
            jogador->x + jogador->tam_x > plataforma.x;

        bool cruzou_plataforma =
            y_anterior + jogador->tam_y <= plataforma.y &&
            jogador->y + jogador->tam_y >= plataforma.y;

        if (sobrepoe_plataforma && cruzou_plataforma) {
            jogador->y = plataforma.y - jogador->tam_y;
            jogador->vel_y = 0;
            jogador->grounded = true;
            return;
        }
    }
}