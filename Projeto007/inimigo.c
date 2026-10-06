#include <stdlib.h>

typedef struct NPC {
    float pos[2];
    float tam[2];
    float cone[2];
    float origem[2];
    float destino[2];
    float vel[2];
    int tipo;
    int status;
} NPC;

/*
Tipos de inimigo
0-Guarda: patrulha os corredores cansadamente com sua não tão fiel lanterna
*/
/*
Tipos de status
0-Descanso: sem movimentos
1-Patrulha: se desloca a um destino arbitrário
9-Alerta: investiga atividade suspeita
*/

//comportamento idle
void ia_mudar_status(*NPC boneco) {
    srand(time(0));
    int novo_status = rand() % 2;
    boneco->status = novo_status;
}

//TODO_POC: lógica do movimento
void ia_movimento(*NPC boneco) {
    switch (boneco->status) {
        case 0:
            boneco->status = ia_mudar_status(boneco);
            break;
        case 1:
            if (abs(boneco->pos[0] - boneco[0]) < 50) {

            }

            break:
    }
    return;
}

//TODO: lógica quando um inimigo detecta o protagonista
void ia_alerta(*NPC boneco, float orig_x, float orig_y) {
    switch (boneco->tipo) {
        case 0:
            // investigar posição 
    }
}
