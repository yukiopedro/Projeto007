#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "fases.h"

#define CARTAO_W 300
#define CARTAO_H 110
#define CARTAO_Y 120
#define JOGAR_X 90
#define JOGAR_Y 540
#define JOGAR_W 300
#define JOGAR_H 80

float x_cartao(float disp_x, int i) { // posições das fases
    return disp_x / 2 - CARTAO_W / 2 + (i - 1) * 400;
}

void iniciar_fases(Fases* fases, Menu* menu, const char* contexto_fase1) { // monta a tela de seleção. só a fase 1 existe por enquanto
    fases->fonte_titulo = menu->fonte_titulo;
    fases->fonte = menu->fonte_botao;
    fases->fonte_texto = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 16, 0);
    fases->som = menu->som;

    fases->atual = 0;
    reiniciar_nome(fases);

    fases->nomes[0] = "1 - FACIL";
    fases->nomes[1] = "2 - MEDIO";
    fases->nomes[2] = "3 - DIFICIL";

    fases->existe[0] = true;
    fases->existe[1] = false;
    fases->existe[2] = false;

    fases->contexto[0] = contexto_fase1;
    fases->contexto[1] = "EM DESENVOLVIMENTO";
    fases->contexto[2] = "EM DESENVOLVIMENTO";
}

void reiniciar_nome(Fases* fases) { // apaga o nome digitado
    fases->nome[0] = '\0';
    fases->tam_nome = 0;
}

void digitar_nome(Fases* fases, int unichar, int keycode) { // chamada a cada letra digitada, backspace apaga
    if (keycode == ALLEGRO_KEY_BACKSPACE) {
        if (fases->tam_nome > 0) {
            fases->tam_nome--;
            fases->nome[fases->tam_nome] = '\0';
        }
    }
    else if (unichar > 32 && unichar < 127 && fases->tam_nome < TAM_NOME - 1) {
        fases->nome[fases->tam_nome] = (char)unichar;
        fases->tam_nome++;
        fases->nome[fases->tam_nome] = '\0';
    }
}

int atualizar_fases(Fases* fases, unsigned char* key, Mouse* mouse, float disp_x) { // retorna ACAO_JOGAR, ACAO_VOLTAR_MENU ou ACAO_NENHUMA
    if ((key[ALLEGRO_KEY_LEFT] & MENU_KEY_SEEN) && fases->atual > 0) {
        fases->atual--;
        tocar_som(fases->som, SOM_MOVER);
    }
    if ((key[ALLEGRO_KEY_RIGHT] & MENU_KEY_SEEN) && fases->atual < TOTAL_FASES - 1) {
        fases->atual++;
        tocar_som(fases->som, SOM_MOVER);
    }

    for (int i = 0; i < TOTAL_FASES; i++) {
        float x = x_cartao(disp_x, i);

        if (mouse_sobre(mouse, x, CARTAO_Y, CARTAO_W, CARTAO_H)) {
            if ((mouse->mexeu || mouse->clicou) && fases->atual != i) {
                fases->atual = i;
                tocar_som(fases->som, SOM_MOVER);
            }
        }
    }

    if (fases->existe[fases->atual] && fases->tam_nome > 0) { // só começa se a fase selecionada existir
        if (mouse->clicou && mouse_sobre(mouse, JOGAR_X, JOGAR_Y, JOGAR_W, JOGAR_H)) {
            tocar_som(fases->som, SOM_CONFIRMAR);
            return ACAO_JOGAR;
        }
        if (key[ALLEGRO_KEY_ENTER] & MENU_KEY_SEEN) {
            tocar_som(fases->som, SOM_CONFIRMAR);
            return ACAO_JOGAR;
        }
    }
    if (key[ALLEGRO_KEY_ESCAPE] & MENU_KEY_SEEN) {
        return ACAO_VOLTAR_MENU;
    }
    return ACAO_NENHUMA;
}

void desenhar_fases(Fases* fases, float disp_x, float disp_y) { // desenha os cartões, o contexto da fase e o botão jogar
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR cinza = al_map_rgb(120, 120, 120);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(fases->fonte_titulo, vermelho, disp_x / 2, 30, ALLEGRO_ALIGN_CENTER, "SELECIONE A FASE");

    for (int i = 0; i < TOTAL_FASES; i++) {
        float x = x_cartao(disp_x, i);
        ALLEGRO_COLOR cor = branco;

        if (i == fases->atual) {
            cor = vermelho;
        }

        al_draw_rectangle(x, CARTAO_Y, x + CARTAO_W, CARTAO_Y + CARTAO_H, cor, 4);
        al_draw_text(fases->fonte, cor, x + CARTAO_W / 2, CARTAO_Y + 40, ALLEGRO_ALIGN_CENTER, fases->nomes[i]);

        if (!fases->existe[i]) {
            al_draw_text(fases->fonte_texto, cinza, x + CARTAO_W / 2, CARTAO_Y + CARTAO_H + 15, ALLEGRO_ALIGN_CENTER, "EM DESENVOLVIMENTO");
        }
    }

    ALLEGRO_COLOR cor_jogar = cinza;

    if (fases->existe[fases->atual] && fases->tam_nome > 0) {
        cor_jogar = vermelho;
    }
    al_draw_rectangle(JOGAR_X, JOGAR_Y, JOGAR_X + JOGAR_W, JOGAR_Y + JOGAR_H, cor_jogar, 4);
    al_draw_text(fases->fonte, cor_jogar, JOGAR_X + JOGAR_W / 2, JOGAR_Y + 28, ALLEGRO_ALIGN_CENTER, "JOGAR");

    al_draw_line(40, 300, disp_x - 40, 300, branco, 4);
    al_draw_line(460, 300, 460, 680, branco, 4);

    al_draw_text(fases->fonte, vermelho, 60, 340, 0, "INSIRA SEU NOME");
    al_draw_rectangle(60, 400, 440, 450, branco, 4);
    al_draw_textf(fases->fonte, branco, 72, 413, 0, "%s_", fases->nome);

    al_draw_text(fases->fonte, vermelho, 480, 340, 0, "CONTEXTO HISTORICO");
    al_draw_multiline_text(fases->fonte_texto, branco, 480, 400, 760, 24, 0, fases->contexto[fases->atual]);

    al_draw_text(fases->fonte_texto, cinza, disp_x / 2, 695, ALLEGRO_ALIGN_CENTER, "ESC PARA VOLTAR");
}