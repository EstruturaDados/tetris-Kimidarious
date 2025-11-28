#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça do Tetris
typedef struct
{
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct
{
    Peca pecas[TAM_FILA]; // Array de peças
    int frente;           // Índice do início da fila
    int tras;             // Índice do final da fila
    int tamanho;          // Quantidade atual de elementos
} FilaPecas;

// Estrutura da pilha de reserva
typedef struct
{
    Peca pecas[TAM_PILHA]; // Array de peças reservadas
    int topo;              // Índice do topo da pilha (-1 = vazia)
} PilhaReserva;

// Variável global para controlar o ID das peças
int proximoId = 0;

// ========================================
// FUNÇÕES DE GERAÇÃO DE PEÇAS
// ========================================

// Função para gerar uma peça aleatória
Peca gerarPeca()
{
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};

    // Seleciona um tipo aleatório
    nova.nome = tipos[rand() % 4];
    nova.id = proximoId++;

    return nova;
}

// ========================================
// FUNÇÕES DA FILA
// ========================================

// Função para inicializar a fila
void inicializarFila(FilaPecas *fila)
{
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Função para verificar se a fila está vazia
int filaVazia(FilaPecas *fila)
{
    return (fila->tamanho == 0);
}

// Função para verificar se a fila está cheia
int filaCheia(FilaPecas *fila)
{
    return (fila->tamanho == TAM_FILA);
}

// Função para inserir uma peça no final da fila (enqueue)
int enqueue(FilaPecas *fila, Peca peca)
{
    if (filaCheia(fila))
    {
        return 0;
    }

    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;

    return 1;
}

// Função para remover uma peça do início da fila (dequeue)
int dequeue(FilaPecas *fila, Peca *pecaRemovida)
{
    if (filaVazia(fila))
    {
        return 0;
    }

    *pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;

    return 1;
}

// ========================================
// FUNÇÕES DA PILHA
// ========================================

// Função para inicializar a pilha
void inicializarPilha(PilhaReserva *pilha)
{
    pilha->topo = -1;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(PilhaReserva *pilha)
{
    return (pilha->topo == -1);
}

// Função para verificar se a pilha está cheia
int pilhaCheia(PilhaReserva *pilha)
{
    return (pilha->topo == TAM_PILHA - 1);
}

// Função para empilhar uma peça (push)
int push(PilhaReserva *pilha, Peca peca)
{
    if (pilhaCheia(pilha))
    {
        return 0;
    }

    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;

    return 1;
}

// Função para desempilhar uma peça (pop)
int pop(PilhaReserva *pilha, Peca *pecaRemovida)
{
    if (pilhaVazia(pilha))
    {
        return 0;
    }

    *pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;

    return 1;
}

// ========================================
// FUNÇÕES DE EXIBIÇÃO
// ========================================

// Função para exibir o estado atual da fila e pilha
void exibirEstado(FilaPecas *fila, PilhaReserva *pilha)
{
    int i, indice;

    printf("\n========================================\n");
    printf("ESTADO ATUAL\n");
    printf("========================================\n");

    // Exibe a fila
    printf("Fila de peças: ");
    if (filaVazia(fila))
    {
        printf("(vazia)");
    }
    else
    {
        indice = fila->frente;
        for (i = 0; i < fila->tamanho; i++)
        {
            printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
            indice = (indice + 1) % TAM_FILA;
        }
    }
    printf("\n");

    // Exibe a pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha))
    {
        printf("(vazia)");
    }
    else
    {
        for (i = pilha->topo; i >= 0; i--)
        {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n");

    // Exibe informações de capacidade
    printf("\nCapacidade - Fila: %d/%d | Pilha: %d/%d\n",
           fila->tamanho, TAM_FILA, pilha->topo + 1, TAM_PILHA);
    printf("========================================\n");
}

// Função para exibir o menu de opções
void exibirMenu()
{
    printf("\n========================================\n");
    printf("OPÇÕES DE AÇÃO\n");
    printf("========================================\n");
    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("0 - Sair\n");
    printf("========================================\n");
    printf("Opção: ");
}

// ========================================
// FUNÇÕES DE AÇÃO DO JOGO
// ========================================

// Função para jogar uma peça (remove da fila)
void jogarPeca(FilaPecas *fila)
{
    Peca pecaJogada;

    if (dequeue(fila, &pecaJogada))
    {
        printf("\n[AÇÃO] Peça [%c %d] foi jogada!\n",
               pecaJogada.nome, pecaJogada.id);

        // Adiciona nova peça automaticamente
        Peca nova = gerarPeca();
        enqueue(fila, nova);
        printf("[AUTO] Nova peça [%c %d] adicionada à fila.\n",
               nova.nome, nova.id);
    }
    else
    {
        printf("\n[ERRO] A fila está vazia!\n");
    }
}

// Função para reservar uma peça (move da fila para pilha)
void reservarPeca(FilaPecas *fila, PilhaReserva *pilha)
{
    Peca pecaReservada;

    // Verifica se a pilha está cheia
    if (pilhaCheia(pilha))
    {
        printf("\n[ERRO] A pilha de reserva está cheia! "
               "Use uma peça reservada primeiro.\n");
        return;
    }

    // Verifica se a fila está vazia
    if (filaVazia(fila))
    {
        printf("\n[ERRO] A fila está vazia!\n");
        return;
    }

    // Remove da fila
    dequeue(fila, &pecaReservada);

    // Adiciona na pilha
    push(pilha, pecaReservada);

    printf("\n[AÇÃO] Peça [%c %d] foi reservada!\n",
           pecaReservada.nome, pecaReservada.id);

    // Adiciona nova peça automaticamente
    Peca nova = gerarPeca();
    enqueue(fila, nova);
    printf("[AUTO] Nova peça [%c %d] adicionada à fila.\n",
           nova.nome, nova.id);
}

// Função para usar uma peça reservada (remove da pilha)
void usarPecaReservada(PilhaReserva *pilha)
{
    Peca pecaUsada;

    if (pop(pilha, &pecaUsada))
    {
        printf("\n[AÇÃO] Peça reservada [%c %d] foi usada!\n",
               pecaUsada.nome, pecaUsada.id);
    }
    else
    {
        printf("\n[ERRO] A pilha de reserva está vazia!\n");
    }
}

// Função para preencher a fila inicial
void preencherFilaInicial(FilaPecas *fila)
{
    int i;
    for (i = 0; i < TAM_FILA; i++)
    {
        Peca nova = gerarPeca();
        enqueue(fila, nova);
    }
}

// ========================================
// FUNÇÃO PRINCIPAL
// ========================================

int main()
{
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    FilaPecas fila;
    PilhaReserva pilha;
    int opcao;

    // Inicializa as estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Mensagem de boas-vindas
    printf("========================================\n");
    printf("   BEM-VINDO AO TETRIS STACK!\n");
    printf("      MODO AVENTUREIRO\n");
    printf("========================================\n");
    printf("\nInicializando jogo...\n");

    // Preenche a fila inicial
    preencherFilaInicial(&fila);
    printf("Fila de peças inicializada com %d peças.\n", TAM_FILA);
    printf("Pilha de reserva criada (capacidade: %d peças).\n", TAM_PILHA);

    // Loop principal do programa
    do
    {
        // Exibe o estado atual
        exibirEstado(&fila, &pilha);

        // Exibe o menu e lê a opção
        exibirMenu();
        scanf("%d", &opcao);

        // Processa a opção escolhida
        switch (opcao)
        {
        case 1:
            // Jogar peça
            jogarPeca(&fila);
            break;

        case 2:
            // Reservar peça
            reservarPeca(&fila, &pilha);
            break;

        case 3:
            // Usar peça reservada
            usarPecaReservada(&pilha);
            break;

        case 0:
            // Sair do programa
            printf("\n========================================\n");
            printf("Obrigado por jogar Tetris Stack!\n");
            printf("Até a próxima!\n");
            printf("========================================\n");
            break;

        default:
            // Opção inválida
            printf("\n[ERRO] Opção inválida! Tente novamente.\n");
            break;
        }

    } while (opcao != 0);

    return 0;
}