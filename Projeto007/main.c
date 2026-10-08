#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

void check();

typedef struct Personagem {
    float x;
    float y;
    float tam_x;
    float tam_y;
    float vel_x;
    float vel_y;
    bool grounded;
} Personagem;

typedef struct NPC {
    float x;
    float y;
    float tam_x;
    float tam_y;
    float cone_w;
    float cone_h;
    float vel_x;
    float vel_y;
    int sentido;
} NPC;

typedef struct Cenario {
    float x;
    float y;
	float w;
    float h;
} Cenario;

#define MAX_PLATAFORMAS 10
#define ALCANCE_COLETA  20.0f
#define ALTURA_CHAO 30

Cenario plataformas[MAX_PLATAFORMAS];
int n_plataformas = 0;

Cenario coletavel;
bool coletado = false;
int documentos = 0;

void add_plataforma(float x, float y, float w, float h){
    if (n_plataformas >= MAX_PLATAFORMAS) 
        return;

    plataformas[n_plataformas].x = x;
    plataformas[n_plataformas].y = y;
    plataformas[n_plataformas].w = w;
    plataformas[n_plataformas].h = h;
    n_plataformas++;
}

bool se_tocam(Cenario a, Cenario b){
    if (a.x < b.x + b.w){
        if (a.x + a.w > b.x){
            if (a.y < b.y + b.h){
                if (a.y + a.h > b.y){
                    return true;
                }
            }
        }
    }
    return false;
}

void iniciar_cenario(float largura, float altura){
    n_plataformas = 0;
    coletado = false;
    documentos = 0;

    float topo_chao = altura - ALTURA_CHAO;
    float altura_apoio = 130;
    float altura_grande = 260;

    add_plataforma(0, topo_chao, largura, ALTURA_CHAO);
    add_plataforma(0, -20, largura, 20);
    add_plataforma(-20, 0, 20, altura);
    add_plataforma(largura, 0, 20, altura);

    add_plataforma(320, topo_chao - altura_apoio, 140, 12);

    add_plataforma(512, topo_chao - altura_grande, 400, 12);
    
    coletavel.w = 14;
    coletavel.h = 18;
    coletavel.x = largura - 80;
    coletavel.y = topo_chao - coletavel.h;
}   

bool colisao_cenario(Cenario r){             // retorna true se o personagem bate em alguma plataforma (chão, teto e paredes)
    for (int i = 0; i < n_plataformas; i++)
        if (se_tocam(r, plataformas[i])) 
            return true;
    return false;
}

void coletar_cenario(Cenario jogador){
    if (coletado) 
        return;

    Cenario alcance = {jogador.x - ALCANCE_COLETA, jogador.y - ALCANCE_COLETA, jogador.w + 2 * ALCANCE_COLETA, jogador.h + 2 * ALCANCE_COLETA};

    if (se_tocam(alcance, coletavel)){
        coletado = true;
        documentos++;
    }
}

void desenhar_cenario(void){
    ALLEGRO_COLOR cor_plat = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR cor_borda = al_map_rgb(140, 145, 160);
    ALLEGRO_COLOR cor_papel = al_map_rgb(90, 95, 110);

    for (int i = 0; i < n_plataformas; i++){
        al_draw_filled_rectangle(plataformas[i].x, plataformas[i].y, plataformas[i].x + plataformas[i].w, plataformas[i].y + plataformas[i].h, cor_plat);
        al_draw_rectangle(plataformas[i].x, plataformas[i].y, plataformas[i].x + plataformas[i].w, plataformas[i].y + plataformas[i].h, cor_borda, 1.0f);
    }

    if (!coletado){
        al_draw_filled_rectangle(coletavel.x, coletavel.y, coletavel.x + coletavel.w, coletavel.y + coletavel.h, cor_papel);
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

    float chao = disp_y - ALTURA_CHAO;

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
            if (key[ALLEGRO_KEY_E]){
                Cenario jogador = {spy.x, spy.y, spy.tam_x, spy.tam_y};
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

            if (spy.y + spy.tam_y >= disp_y || spy.y - spy.tam_y <= 0)
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