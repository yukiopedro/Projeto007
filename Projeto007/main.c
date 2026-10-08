#pragma region libs e arquivos header

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "personagem.h"
#include "npc.h"
#include "cenario.h"

#pragma endregion

void check(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
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

    iniciar_cenario(disp_x, disp_y);

    ALLEGRO_FONT* font = al_create_builtin_font();
    check(font, "font");

    check(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    bool rodando = true;

    ALLEGRO_EVENT event;

    //Personagem spy = // x, y, tam_x, tam_y, vel_x, vel_y
    Personagem spy = { 200, 200, 20, 20, 0, 0, false};

    NPC guarda = {600, disp_y-30-20, 20, 20, 100, 30, 5, 0, 1};

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

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            desenhar_cenario();
            al_draw_textf(font, branco, disp_x - 10, 5, ALLEGRO_ALIGN_RIGHT, "Documentos coletados: %d", documentos);
            al_draw_textf(font, branco, 0, 0, 0, "X: %.1f Y: %.1f", spy.x, spy.y);
            al_draw_filled_rectangle(spy.x, spy.y, spy.x + spy.tam_x, spy.y + spy.tam_y, vermelho);
            al_draw_filled_rectangle(guarda.x, guarda.y, guarda.x + guarda.tam_x, guarda.y + guarda.tam_y, azul);
            al_draw_filled_rectangle(guarda.x+guarda.tam_x*(guarda.sentido==1), guarda.y - guarda.cone_h, guarda.x + (guarda.tam_x + guarda.cone_w)*guarda.sentido, guarda.y + guarda.cone_h, branco);

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

            if (guarda.x + guarda.tam_x >= disp_x || guarda.x <= 0) {
                guarda.sentido *= -1;
            }

            guarda.x += guarda.vel_x * guarda.sentido;

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}