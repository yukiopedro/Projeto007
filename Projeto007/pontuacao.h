#ifndef PONTUACAO_H
#define PONTUACAO_H

#define PONTOS_INICIAIS 1100
#define PONTOS_POR_SEGUNDO 2
#define PONTOS_POR_DOCUMENTO 200
#define PONTOS_POR_GUARDA 100
#define TOTAL_DOCUMENTOS_FASE 3
#define PONTOS_POR_ACERTO 300

typedef struct Partida {
    int frames;  
    int guardas_fora;
    int documentos;
    int pontuacao;
    int acertos;
} Partida;

void reiniciar_partida(Partida* partida);
void calcular_pontuacao(Partida* partida);

#endif // !pontuacao