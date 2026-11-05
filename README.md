# Projeto007

# Espectro Vermelho

Jogo de plataforma 2D com mecânicas de espionagem, desenvolvido em C com a biblioteca Allegro 5 para o Projeto Integrador do Bacharelado em Ciência da Computação do Senac Santo Amaro.

O jogador controla um agente soviético que se infiltra nos Estados Unidos em busca de documentos que possam ajudar seu país no conflito ideológico da Guerra Fria. O objetivo é coletar as informações espalhadas pelo mapa sem ser detectado pelos guardas.

## Sumário

1. [Sobre o jogo](#sobre-o-jogo)
2. [Objetivo educacional](#objetivo-educacional)
3. [Equipe](#equipe)
4. [Estado do projeto](#estado-do-projeto)
5. [Controles](#controles)
6. [Estrutura do repositório](#estrutura-do-repositório)
7. [Arquitetura do código](#arquitetura-do-código)
8. [Cronograma](#cronograma)
9. [Requisitos do projeto](#requisitos-do-projeto)
10. [Fluxo de trabalho](#fluxo-de-trabalho)
11. [Créditos e licenças](#créditos-e-licenças)

## Sobre o jogo

Espectro Vermelho é um jogo de plataforma de gênero espionagem, com arte em pixel art e foco em mecânicas de furtividade. O jogador precisa evitar o contato com os inimigos enquanto se movimenta pelo mapa, usando distrações do ambiente e ferramentas escolhidas antes da partida.

Características planejadas:

* Estética com o personagem em tons avermelhados e cenário em tons frios e acinzentados, para dar destaque ao protagonista.
* Inimigos com campo de visão e inteligência de movimento própria.
* Escolha de três ferramentas ("bugigangas") antes da partida, como lockpicks e armas.
* Trilha sonora ambiente e efeitos sonoros.

## Objetivo educacional

O jogo é voltado para estudantes do ensino médio. O aprendizado acontece pela história e pelos objetivos: os documentos coletados em cada fase tratam de eventos reais da Guerra Fria, e a missão final do jogador envolve responder a perguntas sobre o que foi descoberto. As fontes históricas utilizadas devem ser confiáveis.

## Equipe

| Nome | GitHub |
| --- | --- |
| Davi Germano | [D8vyd](https://github.com/D8vyd) |
| Pedro Augusto | [pdlucena](https://github.com/pdlucena) |
| Pedro Yukio | [yukiopedro](https://github.com/yukiopedro) |


## Estado do projeto

| Área | Situação |
| --- | --- |
| Movimento do personagem (andar, pular, correr, andar devagar) | Implementado |
| Cenário com plataformas e colisão | Implementado |
| Documento coletável com contador | Implementado |
| Menu principal, tela de controles e tela de créditos | Implementado |
| Menu de pausa durante o jogo | Implementado |
| Interação por mouse e teclado nos menus | Implementado |
| Guarda com cone de visão e patrulha | Em desenvolvimento |
| Detecção do jogador e condição de derrota | Planejado |
| Tela de opções | Planejado |
| Efeitos sonoros e trilha sonora | Planejado |
| Ferramentas e habilidades do jogador | Planejado |
| Documentos históricos com texto explicativo | Planejado |
| Relatório final com perguntas | Planejado |
| Sistema de Pontuação | Planejado |


## Controles

| Tecla | Ação |
| --- | --- |
| A e D | Andar |
| W ou Espaço | Pular |
| Shift | Correr |
| Ctrl | Andar devagar |
| E | Interagir (coletar documento) |
| Esc | Pausar o jogo |

Controles planejados: Q para usar item ou habilidade, Tab para alternar entre item e habilidade, teclas numéricas para escolher itens na barra de acesso rápido e F para incapacitar um inimigo.

## Estrutura do repositório

```
Projeto007/
├── Projeto007.sln
├── README.md
├── .gitignore
├── .gitattributes
└── Projeto007/
    ├── Projeto007.vcxproj
    ├── Projeto007.vcxproj.filters
    ├── packages.config
    ├── main.c
    ├── cenario.c / cenario.h
    ├── personagem.c / personagem.h
    ├── menu.c / menu.h
    ├── telas.c / telas.h
    ├── mouse.c / mouse.h
    └── assets/
        └── PressStart2P-Regular.ttf
```

| Módulo | Responsabilidade |
| --- | --- |
| `main` | Inicialização da Allegro, laço principal do jogo, tratamento de eventos e desenho da fase. |
| `cenario` | Plataformas, colisão com o cenário, documento coletável e contador de documentos. |
| `personagem` | Estruturas do personagem e do NPC, criação de inimigos e movimento. |
| `menu` | Menu principal e menu de pausa, com seleção por teclado e mouse. |
| `telas` | Telas de texto (controles e créditos) e controle de qual tela está ativa. |
| `mouse` | Posição, movimento e clique do mouse, e detecção de um ponto dentro de um retângulo. |

## Arquitetura do código

O laço principal roda a 30 quadros por segundo, controlado por um temporizador da Allegro, e consome eventos de uma fila (teclado, mouse, tempo e janela).

Convenções adotadas:

* Estruturas (`Menu`, `Mouse`, `Personagem`, `NPC`, `TelaTexto`) são passadas por ponteiro, para que as funções alterem o dado original.
* Marcas de "aconteceu neste quadro" (`KEY_SEEN`, `clicou`, `mexeu`) são apagadas ao fim de cada quadro, de modo que cada aperto de tecla ou clique execute a ação uma única vez.
* Constantes e dimensões do cenário são definidas por `#define` nos arquivos de cabeçalho.

## Cronograma

| Fase | Entrega | Previsão |
| --- | --- | --- |
| 1. Descoberta | Levantamento de ideias e escopo | 06/09 |
| 2. Alpha | Prova de conceito (POC) | 07/10 |
| 2. Alpha | Produto mínimo viável (MVP) | 11/11 |
| 3. Beta | Versão beta | 30/11 |
| 4. Release | Entrega do produto final | 01/12 |

## Requisitos do projeto

Requisitos funcionais:

* Movimento do personagem (andar e pular).
* Ações do personagem (interagir com o ambiente, usar habilidades e itens).
* Escolha de três ferramentas antes da partida.
* Inimigos com campo de visão e inteligência de movimento.
* Conteúdo educativo sobre eventos históricos da Guerra Fria.
* Cenário com colisões e interatividade.
* Arte em pixel art e música.

Requisitos não funcionais:

* Desenvolvimento em C com a biblioteca Allegro.
* Uso de no máximo 100 MB de memória RAM.
* Taxa de quadros estável.
* Fontes históricas confiáveis.
* Paleta de cores fria no cenário, com o personagem em destaque.
* Música condizente com o ambiente e livre de restrições de direitos autorais.

## Fluxo de trabalho

* Cada funcionalidade é desenvolvida em uma branch própria, a partir da `master`.
* As alterações são integradas à `master` por pull request, com revisão de pelo menos um integrante da equipe.
* As mensagens de commit descrevem a alteração realizada.
* Arquivos gerados pela compilação (`x64`, `.vs`, `*.user`) não são versionados e já constam no `.gitignore`.

## Créditos e licenças

* [Allegro 5](https://liballeg.org/): biblioteca de jogos em C, distribuída sob a licença zlib.