#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "relatorio.h"

#define LARGURA_ALTERNATIVA 1000

void iniciar_perguntas(Menu* alternativas, Menu* menu) { // monta o menu das 4 alternativas
    alternativas->fonte_titulo = menu->fonte_titulo;
    alternativas->fonte_botao = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 18, 0);
    alternativas->som = menu->som;

    alternativas->titulo_cima = "";
    alternativas->titulo_baixo = "RELATORIO";

    alternativas->opcao = 0;
    alternativas->total = TOTAL_ALTERNATIVAS;
    alternativas->y_inicial = 290;
    alternativas->espaco = 75;
}

void iniciar_relatorio(Relatorio* relatorio) { // escreve as perguntas do relatório
    relatorio->perguntas[0].enunciado = "Por que o Sputnik assustou os EUA?";
    relatorio->perguntas[0].alternativas[0] = "A) Porque ele estava programado para cair em Nova York";
    relatorio->perguntas[0].alternativas[1] = "B) Porque levava tripulantes armados para a Lua, representando uma ameaça direta";
    relatorio->perguntas[0].alternativas[2] = "C) Porque bloqueou as rádios americanas, dificultando o acesso à informação";
    relatorio->perguntas[0].alternativas[3] = "D) Porque significava que a URSS tinha foguetes de longo alcance";
    relatorio->perguntas[0].correta = 3;

    relatorio->perguntas[1].enunciado = "Como o sinal do Sputnik 1 era ouvido no rádio?";
    relatorio->perguntas[1].alternativas[0] = "A) Um bipe repetido, como se fosse uma mensagem subliminar";
    relatorio->perguntas[1].alternativas[1] = "B) Uma música militar, com informações escondidas na melodia";
    relatorio->perguntas[1].alternativas[2] = "C) Era a voz de um astronauta russo, transmitindo informações secretas";
    relatorio->perguntas[1].alternativas[3] = "D) Um ruído sem pausa, como se fosse um sinal de alerta";
    relatorio->perguntas[1].correta = 0;

    relatorio->perguntas[2].enunciado = "O que aconteceu com o foguete americano Vanguard em 6 de dezembro de 1957?";
    relatorio->perguntas[2].alternativas[0] = "A) Chegou à órbita com sucesso";
    relatorio->perguntas[2].alternativas[1] = "B) Explodiu logo após decolar, deixando os EUA atrás na corrida espacial";
    relatorio->perguntas[2].alternativas[2] = "C) Foi derrubado pela URSS, próximo da Estratosfera";
    relatorio->perguntas[2].alternativas[3] = "D) Foi cancelado antes do lançamento, por motivos de segurança";
    relatorio->perguntas[2].correta = 1;
}

void carregar_pergunta(Relatorio* relatorio, Menu* alternativas) { // coloca as alternativas da pergunta atual nos botões
    Pergunta* p = &relatorio->perguntas[relatorio->atual];

    for (int i = 0; i < TOTAL_ALTERNATIVAS; i++) {
        alternativas->nomes[i] = p->alternativas[i];
    }
    alternativas->opcao = 0;
    relatorio->respondeu = false;
    relatorio->escolhida = -1;
}

void reiniciar_relatorio(Relatorio* relatorio, Menu* alternativas) { // reinicia o relatorio
    relatorio->atual = 0;
    relatorio->fim = false;
    carregar_pergunta(relatorio, alternativas);
}

bool escolher_alternativa(Menu* alternativas, unsigned char* key, Mouse* mouse, float disp_x) { // igual ao escolher_opcao, mas a área do mouse é a alternativa inteira
    mover_opcao(alternativas, key);

    for (int i = 0; i < alternativas->total; i++) {
        float y = alternativas->y_inicial + i * alternativas->espaco;

        if (mouse_sobre(mouse, disp_x / 2 - LARGURA_ALTERNATIVA / 2, y - 8, LARGURA_ALTERNATIVA, alternativas->espaco - 10)) {
            if (mouse->mexeu) {
                if (alternativas->opcao != i) {
                    tocar_som(alternativas->som, SOM_MOVER);
                }
                alternativas->opcao = i;
            }
            if (mouse->clicou) {
                alternativas->opcao = i;
                tocar_som(alternativas->som, SOM_CONFIRMAR);
                return true;
            }
        }
    }

    if (key[ALLEGRO_KEY_ENTER] & MENU_KEY_SEEN) {
        tocar_som(alternativas->som, SOM_CONFIRMAR);
        return true;
    }
    return false;
}

int atualizar_relatorio(Relatorio* relatorio, Menu* alternativas, Partida* partida, unsigned char* key, Mouse* mouse, float disp_x) { // retorna ao menu quando termina

    if (relatorio->fim) {
        if (key[ALLEGRO_KEY_ENTER] & MENU_KEY_SEEN) {
            tocar_som(alternativas->som, SOM_CONFIRMAR);
            return ACAO_VOLTAR_MENU;
        }
        return ACAO_NENHUMA;
    }

    if (!relatorio->respondeu) { // esperando o palpite
        if (escolher_alternativa(alternativas, key, mouse, disp_x)) {
            relatorio->escolhida = alternativas->opcao;
            relatorio->respondeu = true;

            if (relatorio->escolhida == relatorio->perguntas[relatorio->atual].correta) {
                partida->acertos++;
            }
        }
        return ACAO_NENHUMA;
    }

    if (key[ALLEGRO_KEY_ENTER] & MENU_KEY_SEEN) { // já respondeu e viu a correção, vai pra próxima
        tocar_som(alternativas->som, SOM_CONFIRMAR);
        relatorio->atual++;

        if (relatorio->atual >= TOTAL_PERGUNTAS) {
            relatorio->fim = true;
            calcular_pontuacao(partida);
            tocar_som(alternativas->som, SOM_CONCLUSAO);
        }
        else {
            carregar_pergunta(relatorio, alternativas);
        }
    }
    return ACAO_NENHUMA;
}

void desenhar_relatorio(Relatorio* relatorio, Menu* alternativas, Partida* partida, float disp_x, float disp_y) { // desenha as telas
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR verde = al_map_rgb(0, 200, 0);
    ALLEGRO_COLOR preto = al_map_rgb(0, 0, 0);

    float x1 = disp_x / 2 - 580;
    float x2 = disp_x / 2 + 580;
    float y1 = 40;
    float y2 = 680;

    al_clear_to_color(preto);
    al_draw_rectangle(x1, y1, x2, y2, branco, 4);

    if (relatorio->fim) {
        int segundos = partida->frames / 30;

        al_draw_text(alternativas->fonte_titulo, verde, disp_x / 2, y1 + 50, ALLEGRO_ALIGN_CENTER, "MISSÃO CUMPRIDA");
        al_draw_textf(alternativas->fonte_botao, branco, disp_x / 2, 250, ALLEGRO_ALIGN_CENTER, "TEMPO: %02d:%02d", segundos / 60, segundos % 60);
        al_draw_textf(alternativas->fonte_botao, branco, disp_x / 2, 310, ALLEGRO_ALIGN_CENTER, "DOCUMENTOS: %d DE %d", partida->documentos, TOTAL_DOCUMENTOS_FASE);
        al_draw_textf(alternativas->fonte_botao, branco, disp_x / 2, 370, ALLEGRO_ALIGN_CENTER, "ACERTOS: %d DE %d", partida->acertos, TOTAL_PERGUNTAS);
        al_draw_textf(alternativas->fonte_botao, vermelho, disp_x / 2, 450, ALLEGRO_ALIGN_CENTER, "PONTUAÇÃO FINAL: %d", partida->pontuacao);
        al_draw_text(alternativas->fonte_botao, branco, disp_x / 2, 600, ALLEGRO_ALIGN_CENTER, "ENTER PARA VOLTAR AO MENU");
        return;
    }

    Pergunta* p = &relatorio->perguntas[relatorio->atual];

    al_draw_text(alternativas->fonte_titulo, vermelho, disp_x / 2, y1 + 30, ALLEGRO_ALIGN_CENTER, alternativas->titulo_baixo);
    al_draw_textf(alternativas->fonte_botao, branco, disp_x / 2, 150, ALLEGRO_ALIGN_CENTER, "PERGUNTA %d DE %d", relatorio->atual + 1, TOTAL_PERGUNTAS);
    al_draw_multiline_text(alternativas->fonte_botao, branco, disp_x / 2, 210, 1050, 34, ALLEGRO_ALIGN_CENTER, p->enunciado);

    for (int i = 0; i < alternativas->total; i++) {
        float y = alternativas->y_inicial + i * alternativas->espaco;
        ALLEGRO_COLOR cor = branco;

        if (relatorio->respondeu) {
            if (i == p->correta) {
                cor = verde;
            }
            else if (i == relatorio->escolhida) {
                cor = vermelho;
            }
        }
        else if (i == alternativas->opcao) {
            cor = vermelho;
        }
        al_draw_multiline_text(alternativas->fonte_botao, cor, disp_x / 2, y, LARGURA_ALTERNATIVA, 0, ALLEGRO_ALIGN_CENTER, alternativas->nomes[i]);
    }

    if (relatorio->respondeu) {
        if (relatorio->escolhida == p->correta) {
            al_draw_textf(alternativas->fonte_botao, verde, disp_x / 2, 590, ALLEGRO_ALIGN_CENTER, "CORRETO! +%d PONTOS", PONTOS_POR_ACERTO);
        }
        else {
            al_draw_textf(alternativas->fonte_botao, vermelho, disp_x / 2, 590, ALLEGRO_ALIGN_CENTER, "ERRADO. RESPOSTA CERTA: %c", 'A' + p->correta);
        }
        al_draw_text(alternativas->fonte_botao, branco, disp_x / 2, 635, ALLEGRO_ALIGN_CENTER, "ENTER PARA CONTINUAR");
    }
}