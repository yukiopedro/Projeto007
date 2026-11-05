#pragma region libs e arquivos header

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <string.h>
#include "personagem.h"
#include "cenario.h"
#include "menu.h"
#include "telas.h"
#include "mouse.h"

#pragma endregion

typedef struct GRANADA {
    float x, y, dx, dy;
    int frame;
    bool used;
    bool chao;
} GRANADA;

#define GRANADA_LARGURA 20.0f
#define GRANADA_ALTURA 20.0f
#define GRANADA_GRAVIDADE 1.0f
#define GRANADA_FADE_TICKS 30

GRANADA granada = { 0 };
ALLEGRO_BITMAP* imagem_granada = NULL;
bool jogar_solicitado = false;

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

void check(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

bool colisao(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {

    if (ax1 > bx2) return false;
    if (ax2 < bx1) return false;
    if (ay1 > by2) return false;
    if (ay2 < by1) return false;

    return true;

}


int main()
{
    check(al_init(), "allegro");
    check(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    check(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    check(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    float disp_x = 1280;
    float disp_y = 720;

    ALLEGRO_DISPLAY* disp = al_create_display(disp_x, disp_y);
    check(disp, "display");

    imagem_granada = al_load_bitmap("granada.png");
    check(imagem_granada, "granada.png");

    iniciar_cenario(disp_x, disp_y);

    Menu menu;
    check(iniciar_menu(&menu), "menu");

    Menu pausa;
    iniciar_pausa(&pausa, &menu);

    Mouse mouse;
    iniciar_mouse(&mouse);

    TelaTexto instrucoes;
    iniciar_instrucoes(&instrucoes);

    TelaTexto creditos;
    iniciar_creditos(&creditos);

    ALLEGRO_FONT* font = al_create_builtin_font();
    check(font, "font");

    check(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    bool rodando = true;
    int tela = TELA_MENU;

    ALLEGRO_EVENT event;

    //Personagem spy = // x, y, tam_x, tam_y, vel_x, vel_y
    Personagem spy = { 200, 200, 20, 20, 0, 0, false};

    NPC guarda;
    criar_npc(&guarda, 80, 80, GUARDA);

    float gravidade = 10;

    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR azul = al_map_rgb(0, 0, 255);
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);

#define KEY_SEEN     1
#define KEY_DOWN     2

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));


    al_start_timer(timer);
    while (rodando)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {

        case ALLEGRO_EVENT_MOUSE_AXES:
            atualizar_mouse(&mouse, &event);
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            atualizar_mouse(&mouse, &event);
            break;

        case ALLEGRO_EVENT_TIMER:
            if ((key[ALLEGRO_KEY_W] || key[ALLEGRO_KEY_SPACE]) && spy.grounded) {
                spy.grounded = false;
                spy.vel_y = -50;
                gravidade = 8;
            }
            if (key[ALLEGRO_KEY_A])
                spy.vel_x = -10;
            if (key[ALLEGRO_KEY_D])
                spy.vel_x = 10;
            if (key[ALLEGRO_KEY_E]) {
                Cenario jogador = { spy.x, spy.y, spy.tam_x, spy.tam_y };
                coletar_cenario(jogador);
            }
            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= ~KEY_SEEN;
            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            if (event.keyboard.keycode == ALLEGRO_KEY_Q &&
                !(key[ALLEGRO_KEY_Q] & KEY_DOWN)) {
                jogar_solicitado = true;
            }
            key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= ~KEY_DOWN;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (tela != TELA_JOGO && tela != TELA_PAUSA) {
            if (redraw && al_is_event_queue_empty(queue)) {
                desenhar_telas(tela, &menu, &pausa, &instrucoes, &creditos, disp_x);
                al_flip_display();
                redraw = false;
            }
            continue;
        }

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            desenhar_cenario();
            desenhar_granada(&granada);
            al_draw_textf(font, branco, disp_x - 10, 5, ALLEGRO_ALIGN_RIGHT, "Documentos coletados: %d", documentos);
            al_draw_textf(font, branco, 0, 0, 0, "X: %.1f Y: %.1f", spy.x, spy.y);
            al_draw_filled_rectangle(spy.x, spy.y, spy.x + spy.tam_x, spy.y + spy.tam_y, vermelho);
            al_draw_filled_rectangle(guarda.p.x, guarda.p.y, guarda.p.x + guarda.p.tam_x, guarda.p.y + guarda.p.tam_y, azul);
            al_draw_filled_rectangle(guarda.p.x+guarda.p.tam_x*(guarda.p.sentido), guarda.p.y - guarda.cone_h, guarda.p.x + (guarda.p.tam_x + guarda.cone_w)*guarda.p.sentido, guarda.p.y + guarda.cone_h, branco);

            if (tela == TELA_PAUSA) {
                desenhar_pausa(&pausa, disp_x, disp_y);
                al_flip_display();
                redraw = false;
                continue;
            }

            if (tela == TELA_PAUSA) {
                desenhar_pausa(&pausa, disp_x, disp_y);
                al_flip_display();
                redraw = false;
                continue;
            }

            float y_anterior = spy.y;

            spy.x += spy.vel_x;
            spy.y += spy.vel_y;

            if (spy.vel_y < 30) 
                spy.vel_y += gravidade;

            colisao_plataforma(&spy, y_anterior);

            if (spy.grounded) {
                spy.vel_x -= spy.vel_x / 2.5;
            }

            if (spy.y + spy.tam_y >= disp_y || spy.y <= 0)
                spy.vel_y *= -1;

            if (guarda.p.x + guarda.p.tam_x >= disp_x || guarda.p.x <= 0) {
                guarda.p.sentido *= -1;
            }

            guarda.p.x += guarda.p.vel_x * guarda.p.sentido;

            if (spy.tam_x + spy.x >= disp_x) {
                spy.x = disp_x - spy.tam_x;
            }

            if (spy.x < 0) {

                spy.x = 0;

            }

            if (granada.x + GRANADA_LARGURA >= disp_x) {
                granada.x = disp_x - GRANADA_LARGURA;
            }

            if (granada.x < 0)
                granada.x = 0;
                
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_bitmap(imagem_granada);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}