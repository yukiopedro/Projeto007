#include "granada.h"

bool jogar_solicitado = false;
ALLEGRO_BITMAP* imagem_granada = NULL;

void jogar_granada(GRANADA* g, const Personagem* spy)
{
    if (g->used)
        return;

    g->x = spy->x + (spy->tam_x - GRANADA_LARGURA) / 2.0f;
    g->y = spy->y + spy->tam_y / 2.0f;
    g->dx = spy->sentido ? 10.0f : -10.0f;
    g->dy = -18.0f;
    g->frame = 0;
    g->used = true;
    g->chao = false;
}

void atualizar_granada(GRANADA* g)
{
    if (!g->used)
        return;

    if (g->chao) {
        if (--g->frame <= 0)
            g->used = false;
        return;
    }

    float y_anterior = g->y;

    g->dy += GRANADA_GRAVIDADE;
    g->x += g->dx;
    g->y += g->dy;

    Personagem colisao = { 0 };
    colisao.x = g->x;
    colisao.y = g->y;
    colisao.tam_x = GRANADA_LARGURA;
    colisao.tam_y = GRANADA_ALTURA;
    colisao.vel_y = g->dy;

    colisao_plataforma(&colisao, y_anterior);

    if (colisao.grounded) {
        g->y = colisao.y;
        g->dy = 0;
        g->dx = 0;
        g->chao = true;
        g->frame = GRANADA_FADE_TICKS;
    }
}

void desenhar_granada(const GRANADA* g)
{
    if (!g->used || !imagem_granada)
        return;

    float alpha = g->chao
        ? (float)g->frame / GRANADA_FADE_TICKS
        : 1.0f;

    ALLEGRO_COLOR tint = al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha);
    al_draw_tinted_scaled_bitmap(
        imagem_granada,
        tint,
        0, 0,
        al_get_bitmap_width(imagem_granada),
        al_get_bitmap_height(imagem_granada),
        g->x, g->y,
        GRANADA_LARGURA, GRANADA_ALTURA,
        0
    );
}