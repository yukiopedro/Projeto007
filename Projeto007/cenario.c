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

Cenario saida;

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

    add_plataforma(0, topo_chao, largura, ALTURA_CHAO);
    add_plataforma(0, -20, largura, 20); 
    add_plataforma(-20, 0, 20, altura); 
    add_plataforma(largura, 0, 20, altura);

    add_plataforma(210, 560, 230, 12);
    add_plataforma(465, 430, 470, 12);
    add_plataforma(800, 300, 130, 12);
    add_plataforma(0, 170, largura, 12);

    for (int i = 0; i < TOTAL_DOCUMENTOS_FASE; i++) {
        coletaveis[i].w = 26;
        coletaveis[i].h = 26;
        coletados[i] = false;
    }

    coletaveis[0].x = 1190;
    coletaveis[0].y = topo_chao - coletaveis[0].h;

    coletaveis[1].x = 680;
    coletaveis[1].y = 430 - coletaveis[1].h;

    coletaveis[2].x = 45;
    coletaveis[2].y = 170 - coletaveis[2].h;

    saida.w = 44;
    saida.h = 64;
    saida.x = 1150;
    saida.y = 170 - saida.h;
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

void renderizar_plataformas(ALLEGRO_BITMAP* sprite) { // desenha o sprite da plataforma
    for (int i = 0; i < n_plataformas; i++) {
        for (float py = 0; py < plataformas[i].h; py += SPRITE_PLAT) {
            for (float px = 0; px < plataformas[i].w; px += SPRITE_PLAT) {
                float larg = SPRITE_PLAT;
                float alt = SPRITE_PLAT;

                if (px + larg > plataformas[i].w) { 
                    larg = plataformas[i].w - px;
                }
                if (py + alt > plataformas[i].h) { 
                    alt = plataformas[i].h - py;
                }
                al_draw_bitmap_region(sprite, 0, 0, larg, alt, plataformas[i].x + px, plataformas[i].y + py, 0);
            }
        }
    }
}

void renderizar_documentos(ALLEGRO_BITMAP* sprite) { // desenha o sprite do documento
    for (int i = 0; i < TOTAL_DOCUMENTOS_FASE; i++) {
        if (!coletados[i]) {
            al_draw_scaled_bitmap(sprite, 0, 0, SPRITE_DOC, SPRITE_DOC, coletaveis[i].x, coletaveis[i].y, coletaveis[i].w, coletaveis[i].h, 0);
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

bool chegou_saida(Cenario jogador) { // retorna true se o jogador está encostado na porta de saída
    return se_tocam(jogador, saida);
}

void desenhar_saida() { // desenha a porta de saída
    ALLEGRO_COLOR vermelho = al_map_rgb(200, 30, 30);
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);

    al_draw_filled_rectangle(saida.x, saida.y, saida.x + saida.w, saida.y + saida.h, vermelho);
    al_draw_rectangle(saida.x, saida.y, saida.x + saida.w, saida.y + saida.h, branco, 2.0f);
}