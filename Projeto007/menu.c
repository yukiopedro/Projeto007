#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "menu.h"

bool iniciar_menu(Menu* menu) {
    if (!al_init_font_addon()) {
        return false;
    }
    if (!al_init_ttf_addon()) {
        return false;
    }

    menu->fonte_titulo = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 56, 0);
    menu->fonte_botao = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 24, 0);

    if (!menu->fonte_titulo || !menu->fonte_botao) {
        return false;
    }

    menu->opcao = 0;
    menu->total = TOTAL_OPCOES;
    menu->nomes[0] = "JOGAR";
    menu->nomes[1] = "OPÇÕES";
    menu->nomes[2] = "INSTRUÇÕES";
    menu->nomes[3] = "CRÉDITOS";
    menu->nomes[4] = "SAIR";

    return true;
}

int atualizar_menu(Menu* menu, unsigned char* key) {
    if (key[ALLEGRO_KEY_UP] & MENU_KEY_SEEN) {
        menu->opcao--;

        if (menu->opcao < 0) {
            menu->opcao = menu->total - 1;
        }
    }
    if (key[ALLEGRO_KEY_DOWN] & MENU_KEY_SEEN) {
        menu->opcao++;

        if (menu->opcao >= menu->total) {
            menu->opcao = 0;
        }
    }

    if (key[ALLEGRO_KEY_ENTER] & MENU_KEY_SEEN) {
        if (menu->opcao == 0) {
            return ACAO_JOGAR;
        }
        if (menu->opcao == 2) {
            return ACAO_INSTRUCOES;
        }
        if (menu->opcao == 3) {
            return ACAO_CREDITOS;
        }
        if (menu->opcao == 4) {
            return ACAO_SAIR;
        }
    }

    if (key[ALLEGRO_KEY_ESCAPE] & MENU_KEY_SEEN) {
        return ACAO_SAIR;
    }

    return ACAO_NENHUMA;
}

void desenhar_menu(Menu* menu, float disp_x) {
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(menu->fonte_titulo, branco, disp_x / 2, 100, ALLEGRO_ALIGN_CENTER, "ESPECTRO");
    al_draw_text(menu->fonte_titulo, vermelho, disp_x / 2, 180, ALLEGRO_ALIGN_CENTER, "VERMELHO");

    for (int i = 0; i < menu->total; i++) {
        float y = 330 + i * 60;
        ALLEGRO_COLOR cor = branco;

        if (i == menu->opcao) {
            cor = vermelho;
        }
        al_draw_text(menu->fonte_botao, cor, disp_x / 2, y, ALLEGRO_ALIGN_CENTER, menu->nomes[i]);
    }
}

void destruir_menu(Menu* menu) {
    al_destroy_font(menu->fonte_titulo);
    al_destroy_font(menu->fonte_botao);
}