#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#define GRAVIDADE 10

typedef struct Personagem {
    float x;
    float y;
    float tam_x;
    float tam_y;
    float vel_x;
    float vel_y;
} Personagem;

int main()
{
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool rodando = true;


    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);

    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);

    
    Personagem spy = { 200, 200, 20, 20, 10, 10 };
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (rodando) {

        al_clear_to_color(branco);

        al_draw_filled_rectangle(spy.x, spy.y, spy.x + spy.tam_x, spy.y + spy.tam_y, vermelho);

        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    spy.y -= spy.vel_y;
                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    spy.y += spy.vel_y;
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    spy.x -= spy.vel_x;
                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    spy.x += spy.vel_x;
        }



        al_flip_display();

    }

    al_destroy_display(display);
    return 0;
}