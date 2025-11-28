#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_FILA];  // Array de peças
    int frente;             // Índice do início da fila
    int tras;               // Índice do final da fila
    int tamanho;            // Quantidade atual de elementos
} FilaPecas;

// Variável global para controlar o ID das peças
int proximoId = 0;

// Função para gerar uma peça aleatória
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Seleciona um tipo aleatório
    nova.nome = tipos[rand() % 4];
    nova.id = proximoId++;
    
    return nova;
}

// Função para inicializar a fila
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Função para verificar se a fila está vazia
int filaVazia(FilaPecas *fila) {
    return (fila->tamanho == 0);
}

// Função para verificar se a fila está cheia
int filaCheia(FilaPecas *fila) {
    return (fila->tamanho == TAM_FILA);
}

// Função para inserir uma peça no final da fila (enqueue)
int enqueue(FilaPecas *fila, Peca peca) {
    // Verifica se a fila está cheia
    if (filaCheia(fila)) {
        printf("\n[ERRO] A fila está cheia! Não é possível adicionar mais peças.\n");
        return 0;
    }
    
    // Atualiza o índice traseiro de forma circular
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;
    
    printf("\n[SUCESSO] Peça [%c %d] adicionada à fila!\n", peca.nome, peca.id);
    return 1;
}

// Função para remover uma peça do início da fila (dequeue)
int dequeue(FilaPecas *fila, Peca *pecaRemovida) {
    // Verifica se a fila está vazia
    if (filaVazia(fila)) {
        printf("\n[ERRO] A fila está vazia! Não há peças para jogar.\n");
        return 0;
    }
    
    // Remove a peça da frente
    *pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    
    printf("\n[SUCESSO] Peça [%c %d] foi jogada!\n", 
           pecaRemovida->nome, pecaRemovida->id);
    return 1;
}

// Função para exibir o estado atual da fila
void exibirFila(FilaPecas *fila) {
    printf("\n========================================\n");
    printf("FILA DE PEÇAS\n");
    printf("========================================\n");
    
    if (filaVazia(fila)) {
        printf("A fila está vazia.\n");
    } else {
        printf("Fila de peças: ");
        
        // Percorre a fila de forma circular
        int i;
        int indice = fila->frente;
        for (i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
            indice = (indice + 1) % TAM_FILA;
        }
        printf("\n");
    }
    
    printf("Tamanho atual: %d/%d\n", fila->tamanho, TAM_FILA);
    printf("========================================\n");
}

// Função para preencher a fila inicial com peças
void preencherFilaInicial(FilaPecas *fila) {
    int i;
    for (i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca();
        enqueue(fila, nova);
    }
}

// Função para exibir o menu de opções
void exibirMenu() {
    printf("\n========================================\n");
    printf("OPÇÕES DE AÇÃO\n");
    printf("========================================\n");
    printf("1 - Jogar peça (dequeue)\n");
    printf("2 - Inserir nova peça (enqueue)\n");
    printf("0 - Sair\n");
    printf("========================================\n");
    printf("Digite sua opção: ");
}

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    FilaPecas fila;
    int opcao;
    Peca pecaRemovida;
    
    // Inicializa a fila
    inicializarFila(&fila);
    
    // Mensagem de boas-vindas
    printf("========================================\n");
    printf("   BEM-VINDO AO TETRIS STACK!\n");
    printf("========================================\n");
    printf("\nInicializando fila de peças...\n");
    
    // Preenche a fila inicial
    preencherFilaInicial(&fila);
    
    // Loop principal do programa
    do {
        // Exibe o estado atual da fila
        exibirFila(&fila);
        
        // Exibe o menu e lê a opção
        exibirMenu();
        scanf("%d", &opcao);
        
        // Processa a opção escolhida
        switch(opcao) {
            case 1:
                // Jogar peça (dequeue)
                if (dequeue(&fila, &pecaRemovida)) {
                    printf("Você jogou a peça [%c %d]!\n", 
                           pecaRemovida.nome, pecaRemovida.id);
                }
                break;
                
            case 2:
                // Inserir nova peça (enqueue)
                if (!filaCheia(&fila)) {
                    Peca nova = gerarPeca();
                    enqueue(&fila, nova);
                } else {
                    printf("\n[ERRO] A fila está cheia! "
                           "Jogue uma peça antes de adicionar outra.\n");
                }
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
        
    } while(opcao != 0);
    
    return 0;
}