#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "telas.h"

void iniciar_tela_texto(TelaTexto* tela, const char* titulo) { // define o título e zera a quantidade de linhas
    tela->titulo = titulo;
    tela->total = 0;
}

void add_linha(TelaTexto* tela, const char* esquerda, const char* direita) { // acrescenta uma linha na esquerda e direita na tela de texto
    if (tela->total >= MAX_LINHAS) {
        return;
    }
    tela->esquerda[tela->total] = esquerda;
    tela->direita[tela->total] = direita;
    tela->total++;
}

void iniciar_instrucoes(TelaTexto* tela) { // preenche a tela de controles
    iniciar_tela_texto(tela, "CONTROLES");
    add_linha(tela, "A / D", "ANDAR");
    add_linha(tela, "W / ESPAÇO", "PULAR");
    add_linha(tela, "SHIFT", "CORRER");
    add_linha(tela, "CTRL", "ANDAR DEVAGAR");
    add_linha(tela, "E", "INTERAGIR");
    add_linha(tela, "P", "ABRIR DIÁRIO");
    add_linha(tela, "ESC", "PAUSAR");
}

void iniciar_creditos(TelaTexto* tela) { // preenche a tela de créditos
    iniciar_tela_texto(tela, "CRÉDITOS");
    add_linha(tela, "DAVI GERMANO", NULL);
    add_linha(tela, "PEDRO AUGUSTO", NULL);
    add_linha(tela, "PEDRO YUKIO", NULL);
}

void desenhar_tela_texto(TelaTexto* tela, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, float disp_x) { // desenha título e linhas
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(fonte_titulo, vermelho, disp_x / 2, 70, ALLEGRO_ALIGN_CENTER, tela->titulo);

    for (int i = 0; i < tela->total; i++) {
        float y = 200 + i * 45;

        if (tela->direita[i] == NULL) {
            al_draw_text(fonte_texto, branco, disp_x / 2, y, ALLEGRO_ALIGN_CENTER, tela->esquerda[i]);
        }
        else {
            al_draw_text(fonte_texto, vermelho, disp_x / 2 - 40, y, ALLEGRO_ALIGN_RIGHT, tela->esquerda[i]);
            al_draw_text(fonte_texto, branco, disp_x / 2 + 40, y, 0, tela->direita[i]);
        }
    }
    al_draw_text(fonte_texto, branco, disp_x / 2, 660, ALLEGRO_ALIGN_CENTER, "ESC PARA VOLTAR");
}

void limpar_teclas(unsigned char* key) { // apaga as teclas apertadas e mantém as teclas seguradas para cada aperto valer uma vez só
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        if (key[i] == (KEY_SEEN | KEY_DOWN)) {
            key[i] = KEY_DOWN;
        }
        else if (key[i] == KEY_SEEN) {
            key[i] = 0;
        }
    }
}

bool iniciar_diario(Diario* diario, ALLEGRO_FONT* fonte_titulo) { // escreve os textos do diário e carrega a fonte
    diario->fonte_titulo = fonte_titulo;
    diario->fonte = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 16, 0);
    diario->fonte_aviso = al_load_ttf_font("assets/PressStart2P-Regular.ttf", AVISO_FONTE, 0);

    diario->contexto = "Escrever um contexto da fase aq";

    diario->titulo[0] = "DOCUMENTO 1";
    diario->texto[0] = "Escrever texto do documento aq";

    diario->titulo[1] = "DOCUMENTO 2";
    diario->texto[1] = "Escrever texto do documento aq";

    diario->titulo[2] = "DOCUMENTO 3";
    diario->texto[2] = "Escrever texto do documento aq";

    reiniciar_diario(diario);
    return true;
}

void reiniciar_diario(Diario* diario) { // marca todos os documentos como não coletados e apaga o aviso
    for (int i = 0; i < TOTAL_DOCUMENTOS; i++) {
        diario->coletado[i] = false;
    }
    diario->aviso = 0;
}

void coletar_documento(Diario* diario, int numero) { // guarda o documento no diário e mostra o aviso
    if (numero < 0 || numero >= TOTAL_DOCUMENTOS) {
        return;
    }
    diario->coletado[numero] = true;
    diario->aviso = AVISO_FRAMES;
}

void atualizar_aviso(Diario* diario) { // faz o aviso ir sumindo com o tempo
    if (diario->aviso > 0) {
        diario->aviso--;
    }
}

void desenhar_aviso(Diario* diario, float disp_x) { // desenha a notificação
    if (diario->aviso <= 0) {
        return;
    }

    const char* linhas[AVISO_LINHAS] = { "NOVA INFORMAÇÃO", "CONFIDENCIAL COLETADA.", "APERTE P PARA", "VER OS DETALHES." };

    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);

    float altura_linha = al_get_font_line_height(diario->fonte_aviso) + 6;
    float margem = 14;

    float larg_texto = 0;

    for (int i = 0; i < AVISO_LINHAS; i++) {
        float larg = al_get_text_width(diario->fonte_aviso, linhas[i]);

        if (larg > larg_texto) {
            larg_texto = larg;
        }
    }

    float w = larg_texto + margem * 2;
    float h = altura_linha * AVISO_LINHAS + margem * 2 - 6;
    float x = disp_x - w - 10;
    float y = 10;

    al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(20, 20, 20));
    al_draw_rectangle(x, y, x + w, y + h, vermelho, 3);

    for (int i = 0; i < AVISO_LINHAS; i++) {
        ALLEGRO_COLOR cor = branco;

        if (i >= 2) {
            cor = vermelho;
        }
        al_draw_text(diario->fonte_aviso, cor, x + margem, y + margem + i * altura_linha, 0, linhas[i]);
    }
}

void desenhar_diario(Diario* diario, float disp_x) { // desenha o contexto e os documentos coletados
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR cinza = al_map_rgb(120, 120, 120);

    int total = 0;

    for (int i = 0; i < TOTAL_DOCUMENTOS; i++) {
        if (diario->coletado[i]) {
            total++;
        }
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(diario->fonte_titulo, vermelho, disp_x / 2, 40, ALLEGRO_ALIGN_CENTER, "DIÁRIO");

    al_draw_text(diario->fonte, vermelho, 140, 150, 0, "CONTEXTO");
    al_draw_multiline_text(diario->fonte, branco, 140, 180, 1000, 0, 0, diario->contexto);

    al_draw_textf(diario->fonte, vermelho, 140, 330, 0, "DOCUMENTOS %d/%d", total, TOTAL_DOCUMENTOS);

    for (int i = 0; i < TOTAL_DOCUMENTOS; i++) {
        float y = 370 + i * 90;

        if (diario->coletado[i]) {
            al_draw_text(diario->fonte, vermelho, 140, y, 0, diario->titulo[i]);
            al_draw_multiline_text(diario->fonte, branco, 140, y + 26, 1000, 0, 0, diario->texto[i]);
        }
        else {
            al_draw_text(diario->fonte, cinza, 140, y, 0, "??? DOCUMENTO NÃO ENCONTRADO");
        }
    }

    al_draw_text(diario->fonte, branco, disp_x / 2, 670, ALLEGRO_ALIGN_CENTER, "ESC PARA FECHAR");
}

int atualizar_telas(int tela, Menu* menu, Menu* pausa, unsigned char* key, Mouse* mouse, float disp_x, bool* done) { // trata a tela atual e retorna a tela para onde o jogo vai
    if (tela == TELA_MENU) {
        int acao = atualizar_menu(menu, key, mouse, disp_x);

        if (acao == ACAO_JOGAR) {
            tela = TELA_JOGO;
        }
        if (acao == ACAO_INSTRUCOES) {
            tela = TELA_INSTRUCOES;
        }
        if (acao == ACAO_CREDITOS) {
            tela = TELA_CREDITOS;
        }
        if (acao == ACAO_SAIR) {
            *done = true;
        }
    }
    else if (tela == TELA_PAUSA) {
        int acao = atualizar_pausa(pausa, key, mouse, disp_x);

        if (acao == ACAO_CONTINUAR) {
            tela = TELA_JOGO;
        }
        if (acao == ACAO_VOLTAR_MENU) {
            tela = TELA_MENU;
        }
        if (acao == ACAO_SAIR) {
            *done = true;
        }
    }
    else if (tela == TELA_DIARIO) {
        if (key[ALLEGRO_KEY_ESCAPE] & KEY_SEEN) {
            tela = TELA_JOGO;
        }
    }
    else if (tela == TELA_INSTRUCOES || tela == TELA_CREDITOS) {
        if (key[ALLEGRO_KEY_ESCAPE] & KEY_SEEN) {
            tela = TELA_MENU;
        }
    }

    limpar_teclas(key); // zera cliques e apertos para não valerem na proxima tela
    limpar_mouse(mouse);
    return tela;
}

void desenhar_telas(int tela, Menu* menu, Menu* pausa, TelaTexto* instrucoes, TelaTexto* creditos, Diario* diario, float disp_x) { // desenha a tela atual
    if (tela == TELA_MENU) {
        desenhar_menu(menu, disp_x);
    }
    else if (tela == TELA_INSTRUCOES) {
        desenhar_tela_texto(instrucoes, menu->fonte_titulo, menu->fonte_botao, disp_x);
    }
    else if (tela == TELA_CREDITOS) {
        desenhar_tela_texto(creditos, menu->fonte_titulo, menu->fonte_botao, disp_x);
    }
    else if (tela == TELA_DIARIO) {
        desenhar_diario(diario, disp_x);
    }
}