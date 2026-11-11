#include "pontuacao.h"

void reiniciar_partida(Partida* partida) { // zera tudo para uma partida nova
    partida->frames = 0;
    partida->guardas_fora = 0;
    partida->documentos = 0;
    partida->pontuacao = 0;
    partida->acertos = 0;
}

void calcular_pontuacao(Partida* partida) { // pontuação = inicial - tempo - documentos que faltaram - guardas
    int segundos = partida->frames / 30;
    int faltaram = TOTAL_DOCUMENTOS_FASE - partida->documentos;

    int pontos = PONTOS_INICIAIS;
    pontos -= segundos * PONTOS_POR_SEGUNDO;
    pontos -= faltaram * PONTOS_POR_DOCUMENTO;
    pontos -= partida->guardas_fora * PONTOS_POR_GUARDA;
    pontos += partida->acertos * PONTOS_POR_ACERTO;

    if (pontos < 0) {
        pontos = 0;
    }
    partida->pontuacao = pontos;
}

void copiar_nome(char* destino, const char* origem) { // copia o nome letra por letra
    int i = 0;

    while (origem[i] != '\0' && i < TAM_NOME - 1) {
        destino[i] = origem[i];
        i++;
    }
    destino[i] = '\0';
}