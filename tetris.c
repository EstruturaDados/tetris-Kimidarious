#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

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

// Estrutura da pilha de reserva
typedef struct {
    Peca pecas[TAM_PILHA];  // Array de peças reservadas
    int topo;                // Índice do topo da pilha (-1 = vazia)
} PilhaReserva;

// Variável global para controlar o ID das peças
int proximoId = 0;

// ========================================
// FUNÇÕES DE GERAÇÃO DE PEÇAS
// ========================================

// Função para gerar uma peça aleatória
Peca gerarPeca() {
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
    if (filaCheia(fila)) {
        return 0;
    }
    
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;
    
    return 1;
}

// Função para remover uma peça do início da fila (dequeue)
int dequeue(FilaPecas *fila, Peca *pecaRemovida) {
    if (filaVazia(fila)) {
        return 0;
    }
    
    *pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    
    return 1;
}

// Função auxiliar para obter uma peça em determinada posição da fila
Peca obterPecaFila(FilaPecas *fila, int posicao) {
    int indice = (fila->frente + posicao) % TAM_FILA;
    return fila->pecas[indice];
}

// Função auxiliar para modificar uma peça em determinada posição da fila
void modificarPecaFila(FilaPecas *fila, int posicao, Peca novaPeca) {
    int indice = (fila->frente + posicao) % TAM_FILA;
    fila->pecas[indice] = novaPeca;
}

// ========================================
// FUNÇÕES DA PILHA
// ========================================

// Função para inicializar a pilha
void inicializarPilha(PilhaReserva *pilha) {
    pilha->topo = -1;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(PilhaReserva *pilha) {
    return (pilha->topo == -1);
}

// Função para verificar se a pilha está cheia
int pilhaCheia(PilhaReserva *pilha) {
    return (pilha->topo == TAM_PILHA - 1);
}

// Função para empilhar uma peça (push)
int push(PilhaReserva *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return 0;
    }
    
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    
    return 1;
}

// Função para desempilhar uma peça (pop)
int pop(PilhaReserva *pilha, Peca *pecaRemovida) {
    if (pilhaVazia(pilha)) {
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
void exibirEstado(FilaPecas *fila, PilhaReserva *pilha) {
    int i, indice;
    
    printf("\n========================================\n");
    printf("ESTADO ATUAL\n");
    printf("========================================\n");
    
    // Exibe a fila
    printf("Fila de peças: ");
    if (filaVazia(fila)) {
        printf("(vazia)");
    } else {
        indice = fila->frente;
        for (i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
            indice = (indice + 1) % TAM_FILA;
        }
    }
    printf("\n");
    
    // Exibe a pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("(vazia)");
    } else {
        for (i = pilha->topo; i >= 0; i--) {
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
void exibirMenu() {
    printf("\n========================================\n");
    printf("OPÇÕES DISPONÍVEIS\n");
    printf("========================================\n");
    printf("1 - Jogar peça da frente da fila\n");
    printf("2 - Enviar peça da fila para a pilha de reserva\n");
    printf("3 - Usar peça da pilha de reserva\n");
    printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
    printf("0 - Sair\n");
    printf("========================================\n");
    printf("Opção escolhida: ");
}

// ========================================
// FUNÇÕES DE AÇÃO DO JOGO
// ========================================

// Função para jogar uma peça (remove da fila)
void jogarPeca(FilaPecas *fila) {
    Peca pecaJogada;
    
    if (dequeue(fila, &pecaJogada)) {
        printf("\n[AÇÃO] Peça [%c %d] foi jogada!\n", 
               pecaJogada.nome, pecaJogada.id);
        
        // Adiciona nova peça automaticamente
        Peca nova = gerarPeca();
        enqueue(fila, nova);
        printf("[AUTO] Nova peça [%c %d] adicionada à fila.\n", 
               nova.nome, nova.id);
    } else {
        printf("\n[ERRO] A fila está vazia!\n");
    }
}

// Função para reservar uma peça (move da fila para pilha)
void reservarPeca(FilaPecas *fila, PilhaReserva *pilha) {
    Peca pecaReservada;
    
    // Verifica se a pilha está cheia
    if (pilhaCheia(pilha)) {
        printf("\n[ERRO] A pilha de reserva está cheia! "
               "Use uma peça reservada primeiro.\n");
        return;
    }
    
    // Verifica se a fila está vazia
    if (filaVazia(fila)) {
        printf("\n[ERRO] A fila está vazia!\n");
        return;
    }
    
    // Remove da fila
    dequeue(fila, &pecaReservada);
    
    // Adiciona na pilha
    push(pilha, pecaReservada);
    
    printf("\n[AÇÃO] Peça [%c %d] foi enviada para a pilha de reserva!\n", 
           pecaReservada.nome, pecaReservada.id);
    
    // Adiciona nova peça automaticamente
    Peca nova = gerarPeca();
    enqueue(fila, nova);
    printf("[AUTO] Nova peça [%c %d] adicionada à fila.\n", 
           nova.nome, nova.id);
}

// Função para usar uma peça reservada (remove da pilha)
void usarPecaReservada(PilhaReserva *pilha) {
    Peca pecaUsada;
    
    if (pop(pilha, &pecaUsada)) {
        printf("\n[AÇÃO] Peça reservada [%c %d] foi usada!\n", 
               pecaUsada.nome, pecaUsada.id);
    } else {
        printf("\n[ERRO] A pilha de reserva está vazia!\n");
    }
}

// Função para trocar a peça da frente da fila com o topo da pilha
void trocarPecaAtual(FilaPecas *fila, PilhaReserva *pilha) {
    Peca pecaFila, pecaPilha;
    
    // Verifica se há peças para trocar
    if (filaVazia(fila)) {
        printf("\n[ERRO] A fila está vazia!\n");
        return;
    }
    
    if (pilhaVazia(pilha)) {
        printf("\n[ERRO] A pilha está vazia!\n");
        return;
    }
    
    // Obtém as peças
    pecaFila = obterPecaFila(fila, 0);
    pecaPilha = pilha->pecas[pilha->topo];
    
    printf("\n[AÇÃO] Trocando [%c %d] (fila) com [%c %d] (pilha)...\n",
           pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
    
    // Realiza a troca
    modificarPecaFila(fila, 0, pecaPilha);
    pilha->pecas[pilha->topo] = pecaFila;
    
    printf("[SUCESSO] Troca realizada!\n");
}

// Função para trocar os 3 primeiros da fila com as 3 peças da pilha
void trocaMultipla(FilaPecas *fila, PilhaReserva *pilha) {
    Peca tempFila[3], tempPilha[3];
    int i;
    
    // Verifica se há peças suficientes
    if (fila->tamanho < 3) {
        printf("\n[ERRO] A fila precisa ter pelo menos 3 peças! (atual: %d)\n", 
               fila->tamanho);
        return;
    }
    
    if (pilha->topo + 1 < 3) {
        printf("\n[ERRO] A pilha precisa ter pelo menos 3 peças! (atual: %d)\n", 
               pilha->topo + 1);
        return;
    }
    
    printf("\n[AÇÃO] Realizando troca múltipla...\n");
    
    // Salva as 3 primeiras peças da fila
    printf("Peças da fila a trocar: ");
    for (i = 0; i < 3; i++) {
        tempFila[i] = obterPecaFila(fila, i);
        printf("[%c %d] ", tempFila[i].nome, tempFila[i].id);
    }
    printf("\n");
    
    // Salva as 3 peças da pilha (do topo para a base)
    printf("Peças da pilha a trocar: ");
    for (i = 0; i < 3; i++) {
        tempPilha[i] = pilha->pecas[pilha->topo - i];
        printf("[%c %d] ", tempPilha[i].nome, tempPilha[i].id);
    }
    printf("\n");
    
    // Coloca as peças da pilha na fila
    // As peças da pilha vão para a fila mantendo a ordem do topo
    for (i = 0; i < 3; i++) {
        modificarPecaFila(fila, i, tempPilha[i]);
    }
    
    // Coloca as peças da fila na pilha
    // As peças da fila vão para a pilha em ordem inversa
    for (i = 0; i < 3; i++) {
        pilha->pecas[pilha->topo - i] = tempFila[i];
    }
    
    printf("[SUCESSO] Troca múltipla realizada!\n");
}

// Função para preencher a fila inicial
void preencherFilaInicial(FilaPecas *fila) {
    int i;
    for (i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca();
        enqueue(fila, nova);
    }
}

// ========================================
// FUNÇÃO PRINCIPAL
// ========================================

int main() {
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
    printf("         NÍVEL MESTRE\n");
    printf("========================================\n");
    printf("\nInicializando jogo...\n");
    
    // Preenche a fila inicial
    preencherFilaInicial(&fila);
    printf("Fila de peças inicializada com %d peças.\n", TAM_FILA);
    printf("Pilha de reserva criada (capacidade: %d peças).\n", TAM_PILHA);
    printf("\nDica: Use a opção 2 para enviar peças para a pilha\n");
    printf("      antes de usar as opções de troca (4 e 5).\n");
    
    // Loop principal do programa
    do {
        // Exibe o estado atual
        exibirEstado(&fila, &pilha);
        
        // Exibe o menu e lê a opção
        exibirMenu();
        scanf("%d", &opcao);
        
        // Processa a opção escolhida
        switch(opcao) {
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
                
            case 4:
                // Trocar peça atual (frente da fila com topo da pilha)
                trocarPecaAtual(&fila, &pilha);
                break;
                
            case 5:
                // Troca múltipla (3 da fila com 3 da pilha)
                trocaMultipla(&fila, &pilha);
                break;
                
            case 0:
                // Sair do programa
                printf("\n========================================\n");
                printf("Obrigado por jogar Tetris Stack!\n");
                printf("Estatísticas finais:\n");
                printf("- Total de peças geradas: %d\n", proximoId);
                printf("- Peças na fila: %d\n", fila.tamanho);
                printf("- Peças na pilha: %d\n", pilha.topo + 1);
                printf("\nAté a próxima!\n");
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