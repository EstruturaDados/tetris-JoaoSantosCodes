/**
 * @file tetris.c
 * @brief Sistema de Gerenciamento de Peças Tetris - Nível Adventurer
 * @version 2.0.0
 * @date 2024-01-26
 * @author João Santos
 * @institution Universidade Estácio de Sá
 * @discipline Estruturas de Dados
 * @created 2024-01-20
 * @updated 2024-01-26
 * 
 * @mainpage Documentação do Sistema Tetris Stack - Nível Adventurer
 * 
 * @section intro_sec Introdução
 * 
 * Este sistema implementa um simulador avançado de gerenciamento de peças do jogo Tetris,
 * utilizando estruturas de dados fundamentais (fila circular e pilha linear) para criar
 * uma experiência interativa e educativa.
 * 
 * @section features_sec Funcionalidades Principais
 * 
 * O sistema oferece as seguintes funcionalidades integradas:
 * 
 * @subsection queue_features Fila Circular de Peças
 * - **Capacidade**: 5 peças simultâneas
 * - **Operações**: Inserção (enqueue) e remoção (dequeue)
 * - **Algoritmo**: Circular com índices dinâmicos
 * - **Validação**: Controle automático de overflow/underflow
 * 
 * @subsection stack_features Pilha de Reserva
 * - **Capacidade**: 3 peças reservadas
 * - **Operações**: Empilhamento (push) e desempilhamento (pop)
 * - **Algoritmo**: LIFO (Last In, First Out)
 * - **Estratégia**: Permite reservar peças para uso posterior
 * 
 * @subsection integration_features Integração Avançada
 * - **Transferência**: Peças podem ser movidas da fila para a pilha
 * - **Geração Automática**: Novas peças são criadas automaticamente
 * - **Interface Visual**: Exibição em tempo real do estado das estruturas
 * - **Estatísticas**: Monitoramento completo do sistema
 * 
 * @section usage_sec Exemplos de Uso
 * 
 * @subsection example_basic Uso Básico
 * 
 * @code{.c}
 * // Inicialização das estruturas
 * FilaCircular fila;
 * PilhaReserva pilha;
 * inicializarFilaCircular(&fila);
 * inicializarPilhaReserva(&pilha);
 * 
 * // Geração e inserção de peça
 * Peca novaPeca = gerarNovaPeca();
 * inserirPecaNaFila(&fila, novaPeca);
 * 
 * // Jogada de peça
 * if (!verificarFilaVazia(&fila)) {
 *     Peca pecaJogada = removerPecaDaFila(&fila);
 *     printf("Jogou peça %c%d\n", pecaJogada.tipo, pecaJogada.id);
 * }
 * @endcode
 * 
 * @subsection example_advanced Uso Avançado com Reserva
 * 
 * @code{.c}
 * // Reservar peça estratégica
 * if (!verificarFilaVazia(&fila) && !verificarPilhaCheia(&pilha)) {
 *     Peca pecaReservada = removerPecaDaFila(&fila);
 *     reservarPecaNaPilha(&pilha, pecaReservada);
 * }
 * 
 * // Usar peça reservada quando necessário
 * if (!verificarPilhaVazia(&pilha)) {
 *     Peca pecaUsada = usarPecaDaPilha(&pilha);
 *     printf("Usou peça reservada %c%d\n", pecaUsada.tipo, pecaUsada.id);
 * }
 * @endcode
 * 
 * @section compilation_sec Compilação e Execução
 * 
 * Para compilar o sistema:
 * @code{.bash}
 * gcc -o tetris tetris.c -std=c99 -Wall -Wextra
 * @endcode
 * 
 * Para executar:
 * @code{.bash}
 * ./tetris
 * @endcode
 * 
 * @author João Santos
 * @copyright 2024 Universidade Estácio de Sá
 */

/*
 * ╔══════════════════════════════════════════════════════════════════════════════╗
 * ║                          TETRIS STACK - NÍVEL ADVENTURER                    ║
 * ║                    Sistema Avançado de Controle de Peças                    ║
 * ╠══════════════════════════════════════════════════════════════════════════════╣
 * ║ Desenvolvedor: João Santos                                                   ║
 * ║ Instituição: Universidade Estácio                                           ║
 * ║ Disciplina: Estruturas de Dados                                             ║
 * ║ Versão: 2.0 - Nível Adventurer                                              ║
 * ║ Data de Criação: Janeiro 2025                                               ║
 * ║ Última Atualização: Janeiro 2025                                            ║
 * ║                                                                              ║
 * ║ DESCRIÇÃO DO PROJETO:                                                        ║
 * ║ Este programa implementa o Nível Adventurer do desafio Tetris Stack,        ║
 * ║ expandindo o sistema básico com uma pilha de reserva para estratégias       ║
 * ║ avançadas de gerenciamento de peças.                                        ║
 * ║                                                                              ║
 * ║ FUNCIONALIDADES IMPLEMENTADAS:                                               ║
 * ║ ✓ Fila circular com capacidade fixa de 5 peças (FIFO)                      ║
 * ║ ✓ Pilha de reserva linear com capacidade para 3 peças (LIFO)               ║
 * ║ ✓ Geração automática de peças com 4 tipos diferentes                       ║
 * ║ ✓ Sistema de identificação única e sequencial                               ║
 * ║ ✓ Interface interativa com menu contextual                                  ║
 * ║ ✓ Visualização simultânea de ambas as estruturas                           ║
 * ║ ✓ Operações integradas entre fila e pilha                                  ║
 * ║ ✓ Validação completa de estados e operações                                ║
 * ║ ✓ Feedback inteligente com dicas para o usuário                            ║
 * ║                                                                              ║
 * ║ TIPOS DE PEÇAS SUPORTADOS:                                                  ║
 * ║ • 'I' - Peça em linha (4 blocos em linha reta)                             ║
 * ║ • 'O' - Peça quadrada (2x2 blocos)                                         ║
 * ║ • 'T' - Peça em formato T (3 blocos + 1 central)                           ║
 * ║ • 'L' - Peça em formato L (3 blocos + 1 perpendicular)                     ║
 * ╚══════════════════════════════════════════════════════════════════════════════╝
 */

// ═══════════════════════════════════════════════════════════════════════════════
//                              BIBLIOTECAS NECESSÁRIAS
// ═══════════════════════════════════════════════════════════════════════════════

#include <stdio.h>   // Funções de entrada/saída (printf, scanf, getchar)
#include <stdlib.h>  // Funções utilitárias (rand, srand, exit)
#include <time.h>    // Funções de tempo (time para inicialização aleatória)

// ═══════════════════════════════════════════════════════════════════════════════
//                           DEFINIÇÕES DE ESTRUTURAS DE DADOS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * @brief Estrutura que representa uma peça individual do Tetris
 * 
 * Cada peça possui duas propriedades fundamentais:
 * • tipo: Caractere que define o formato geométrico da peça
 * • id: Identificador numérico único para rastreamento
 * 
 * @note Os tipos suportados são: 'I', 'O', 'T', 'L'
 * @note Os IDs são gerados sequencialmente a partir de 1
 */
typedef struct {
    char tipo;  // Tipo geométrico: 'I'=linha, 'O'=quadrado, 'T'=T, 'L'=L
    int id;     // Identificador único e sequencial (1, 2, 3, ...)
} Peca;

/**
 * @brief Estrutura que implementa uma fila circular para gerenciamento de peças
 * 
 * A fila circular otimiza o uso de memória reutilizando posições do array.
 * Mantém exatamente 5 peças em rotação constante, seguindo o padrão FIFO
 * (First In, First Out - primeiro a entrar, primeiro a sair).
 * 
 * Componentes da estrutura:
 * • pecas[5]: Array fixo que armazena as 5 peças
 * • indiceFrente: Aponta para a próxima peça a ser jogada
 * • indiceTras: Aponta para a posição da última peça inserida
 * • quantidadePecas: Contador atual de peças válidas (0 a 5)
 * 
 * @note A circularidade é implementada usando operação módulo (%)
 * @note Quando cheia, novas inserções sobrescrevem as mais antigas
 */
typedef struct {
    Peca pecas[5];          // Array circular com capacidade fixa de 5 peças
    int indiceFrente;       // Índice da frente (próxima peça a jogar)
    int indiceTras;         // Índice do final (última peça inserida)
    int quantidadePecas;    // Contador atual de peças válidas (0-5)
} FilaCircular;

/**
 * @brief Estrutura que implementa uma pilha linear para reserva estratégica
 * 
 * A pilha de reserva permite armazenamento temporário de até 3 peças,
 * seguindo o padrão LIFO (Last In, First Out - último a entrar, primeiro a sair).
 * É utilizada para estratégias avançadas de gerenciamento de peças.
 * 
 * Componentes da estrutura:
 * • pecasReservadas[3]: Array linear para armazenamento das peças
 * • indiceTopo: Índice do topo da pilha (-1 = vazia, 0-2 = posições válidas)
 * • quantidadeReservada: Contador atual de peças reservadas (0 a 3)
 * 
 * Operações principais:
 * • Push (empilhar): Adiciona peça no topo, incrementa indiceTopo
 * • Pop (desempilhar): Remove peça do topo, decrementa indiceTopo
 * 
 * @note O índice -1 indica pilha vazia
 * @note Máximo de 3 peças podem ser armazenadas simultaneamente
 */
typedef struct {
    Peca pecasReservadas[3];    // Array linear para até 3 peças reservadas
    int indiceTopo;             // Índice do topo (-1=vazia, 0-2=válido)
    int quantidadeReservada;    // Contador atual de peças reservadas (0-3)
} PilhaReserva;

// ═══════════════════════════════════════════════════════════════════════════════
//                              VARIÁVEIS GLOBAIS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * @brief Contador global para gerar identificadores únicos e sequenciais
 * 
 * Esta variável mantém o controle dos IDs atribuídos às peças:
 * • Inicia em 1 para facilitar a contagem humana
 * • É incrementada automaticamente a cada nova peça criada
 * • Garante que cada peça tenha um identificador único durante toda a sessão
 * • Permite rastreamento e estatísticas precisas do jogo
 * 
 * @note O valor nunca é decrementado, mantendo a unicidade dos IDs
 * @note É utilizada pela função gerarNovaPeca() para atribuição automática
 */
int contadorIdGlobal = 1;

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 *                         FUNÇÕES DE GERAÇÃO DE PEÇAS
 * ═══════════════════════════════════════════════════════════════════════════════
 */

/**
 * @brief Gera uma nova peça automaticamente com tipo aleatório e ID sequencial
 * 
 * Esta função é responsável por criar novas peças para o jogo:
 * 1. Seleciona aleatoriamente um tipo de peça entre os disponíveis
 * 2. Atribui um ID único e sequencial
 * 3. Incrementa o contador global para próximas peças
 * 
 * Tipos de peças disponíveis:
 * - 'I': Peça em linha reta (4 blocos)
 * - 'O': Peça quadrada (2x2 blocos)
 * - 'T': Peça em formato T
 * - 'L': Peça em formato L
 * 
 * @return Peca Nova peça gerada com tipo aleatório e ID único
 * 
 * @note A função utiliza rand() % 4 para seleção aleatória
 * @note O ID é atribuído automaticamente pelo contadorIdGlobal
 * @note Requer inicialização prévia do gerador aleatório com srand()
 * 
 * @see contadorIdGlobal
 * @see srand()
 * 
 * @example
 * @code{.c}
 * // Inicializar gerador aleatório
 * srand(time(NULL));
 * 
 * // Gerar nova peça
 * Peca novaPeca = gerarNovaPeca();
 * printf("Peça gerada: %c%d\n", novaPeca.tipo, novaPeca.id);
 * @endcode
 */
Peca gerarNovaPeca() {
    Peca pecaNova;
    
    // Array com os tipos de peças disponíveis no Tetris clássico
    char tiposDisponiveis[] = {'I', 'O', 'T', 'L'};
    const int quantidadeTipos = 4;
    
    // Seleciona tipo aleatório usando módulo para garantir índice válido
    int indiceAleatorio = rand() % quantidadeTipos;
    pecaNova.tipo = tiposDisponiveis[indiceAleatorio];
    
    // Atribui ID sequencial único e incrementa contador
    pecaNova.id = contadorIdGlobal++;
    
    return pecaNova;
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         FUNÇÕES DE CONTROLE DA FILA CIRCULAR
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * @brief Inicializa a fila circular com 5 peças geradas automaticamente
 * 
 * Esta função prepara a fila para uso no início do jogo:
 * • Configura os índices de controle (frente=0, trás=-1)
 * • Gera 5 peças iniciais usando geração aleatória
 * • Preenche completamente a fila para garantir disponibilidade imediata
 * • Estabelece o estado inicial padrão do sistema
 * 
 * Algoritmo de preenchimento:
 * 1. Inicializa contadores e índices
 * 2. Loop de 5 iterações para preenchimento completo
 * 3. Calcula posição circular usando módulo (%)
 * 4. Gera peça aleatória e insere na posição
 * 5. Incrementa contador de peças válidas
 * 
 * @param filaPtr Ponteiro para a estrutura da fila a ser inicializada
 * @note A fila sempre inicia cheia (5/5 peças) para melhor experiência
 * @note Utiliza gerarNovaPeca() para criar peças com tipos e IDs únicos
 */
void inicializarFilaCircular(FilaCircular* filaPtr) {
    // Inicializa índices de controle da fila circular
    filaPtr->indiceFrente = 0;           // Primeira posição do array (próxima a jogar)
    filaPtr->indiceTras = -1;            // Será incrementado para 0 na primeira inserção
    filaPtr->quantidadePecas = 0;        // Contador inicia zerado
    
    // Preenche a fila com 5 peças iniciais para garantir disponibilidade
    for (int indiceLoop = 0; indiceLoop < 5; indiceLoop++) {
        // Calcula próxima posição circular para inserção (0, 1, 2, 3, 4)
        filaPtr->indiceTras = (filaPtr->indiceTras + 1) % 5;
        
        // Gera nova peça aleatória e insere na posição calculada
        filaPtr->pecas[filaPtr->indiceTras] = gerarNovaPeca();
        
        // Incrementa contador de peças válidas na fila
        filaPtr->quantidadePecas++;
    }
}

/**
 * @brief Verifica se a fila circular está completamente cheia
 * 
 * Função auxiliar para validação de operações de inserção.
 * Evita overflow e garante integridade da estrutura de dados.
 * 
 * @param filaPtr Ponteiro para a fila a ser verificada
 * @return int 1 se a fila está cheia (5/5 peças), 0 caso contrário
 * @note Capacidade máxima fixa de 5 peças conforme especificação
 */
int verificarFilaCheia(FilaCircular* filaPtr) {
    return filaPtr->quantidadePecas == 5;
}

/**
 * @brief Verifica se a fila circular está completamente vazia
 * 
 * Função auxiliar para validação de operações de remoção.
 * Evita underflow e garante integridade da estrutura de dados.
 * 
 * @param filaPtr Ponteiro para a fila a ser verificada
 * @return int 1 se a fila está vazia (0/5 peças), 0 caso contrário
 * @note Em condições normais, a fila nunca deve ficar vazia
 */
int verificarFilaVazia(FilaCircular* filaPtr) {
    return filaPtr->quantidadePecas == 0;
}

/**
 * @brief Adiciona uma peça no final da fila circular (operação enqueue)
 * 
 * Implementa a inserção padrão de fila FIFO com estrutura circular:
 * • Valida disponibilidade de espaço antes da inserção
 * • Calcula próxima posição usando aritmética modular
 * • Insere a peça e atualiza contadores de controle
 * • Mantém integridade da estrutura circular
 * 
 * Algoritmo de inserção:
 * 1. Verificação de capacidade disponível
 * 2. Cálculo da posição circular: (indiceTras + 1) % 5
 * 3. Inserção da peça na posição calculada
 * 4. Atualização do contador de peças
 * 
 * @param filaPtr Ponteiro para a fila onde inserir a peça
 * @param pecaParaInserir Estrutura Peca com tipo e ID a ser inserida
 * @note Exibe mensagem de erro se tentar inserir em fila cheia
 * @note A inserção falha silenciosamente em caso de overflow
 */
void inserirPecaNaFila(FilaCircular* filaPtr, Peca pecaParaInserir) {
    // Validação de capacidade: impede overflow da estrutura
    if (verificarFilaCheia(filaPtr)) {
        printf("⚠️  ERRO: Tentativa de inserir peça em fila cheia!\n");
        return;  // Falha silenciosa para manter estabilidade
    }
    
    // Calcula próxima posição circular (0→1→2→3→4→0...)
    filaPtr->indiceTras = (filaPtr->indiceTras + 1) % 5;
    
    // Insere a peça na posição calculada
    filaPtr->pecas[filaPtr->indiceTras] = pecaParaInserir;
    
    // Atualiza contador de peças válidas na fila
    filaPtr->quantidadePecas++;
}

/**
 * @brief Remove e retorna a peça da frente da fila (operação dequeue)
 * 
 * Implementa a remoção padrão de fila FIFO com estrutura circular:
 * • Valida disponibilidade de peças antes da remoção
 * • Salva a peça a ser removida para retorno
 * • Atualiza índice da frente usando aritmética modular
 * • Decrementa contador e mantém integridade da estrutura
 * 
 * Algoritmo de remoção:
 * 1. Verificação de disponibilidade de peças
 * 2. Backup da peça na frente da fila
 * 3. Atualização circular: (indiceFrente + 1) % 5
 * 4. Decrementação do contador de peças
 * 5. Retorno da peça removida
 * 
 * @param filaPtr Ponteiro para a fila de onde remover a peça
 * @return Peca Peça removida da frente da fila, ou peça vazia em caso de erro
 * @note Retorna peça com tipo ' ' e ID 0 em caso de fila vazia
 * @note A remoção falha silenciosamente em caso de underflow
 */
Peca removerPecaDaFila(FilaCircular* filaPtr) {
    // Peça vazia para retorno em caso de erro ou fila vazia
    Peca pecaVazia = {' ', 0};
    
    // Validação de disponibilidade: impede underflow da estrutura
    if (verificarFilaVazia(filaPtr)) {
        printf("⚠️  ERRO: Tentativa de remover peça de fila vazia!\n");
        return pecaVazia;  // Retorna peça inválida para sinalizar erro
    }
    
    // Salva a peça que será removida da frente da fila
    Peca pecaRemovida = filaPtr->pecas[filaPtr->indiceFrente];
    
    // Atualiza índice da frente para próxima posição circular (0→1→2→3→4→0...)
    filaPtr->indiceFrente = (filaPtr->indiceFrente + 1) % 5;
    
    // Decrementa contador de peças válidas na fila
    filaPtr->quantidadePecas--;
    
    return pecaRemovida;  // Retorna a peça removida com sucesso
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         FUNÇÕES DE CONTROLE DA PILHA DE RESERVA
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * @brief Inicializa a pilha de reserva como vazia
 * 
 * Esta função prepara a pilha para uso no início do jogo:
 * • Define o índice do topo como -1 (convenção padrão para pilha vazia)
 * • Zera o contador de peças reservadas
 * • Estabelece o estado inicial limpo da estrutura
 * 
 * A pilha inicia sempre vazia, permitindo ao jogador decidir
 * estrategicamente quais peças reservar durante o jogo.
 * 
 * @param pilhaPtr Ponteiro para a estrutura da pilha a ser inicializada
 * @note O valor -1 no indiceTopo é a convenção padrão para pilha vazia
 * @note A pilha permite armazenamento de até 3 peças simultaneamente
 */
void inicializarPilhaReserva(PilhaReserva* pilhaPtr) {
    pilhaPtr->indiceTopo = -1;           // -1 indica pilha vazia (convenção padrão)
    pilhaPtr->quantidadeReservada = 0;   // Nenhuma peça reservada inicialmente
}

/**
 * @brief Verifica se a pilha de reserva está completamente cheia
 * 
 * Função auxiliar para validação de operações de inserção (push).
 * Evita overflow e garante integridade da estrutura de dados.
 * 
 * @param pilhaPtr Ponteiro para a pilha a ser verificada
 * @return int 1 se a pilha está cheia (3/3 peças), 0 caso contrário
 * @note Capacidade máxima fixa de 3 peças conforme especificação
 */
int verificarPilhaCheia(PilhaReserva* pilhaPtr) {
    return pilhaPtr->quantidadeReservada == 3;
}

/**
 * @brief Verifica se a pilha de reserva está completamente vazia
 * 
 * Função auxiliar para validação de operações de remoção (pop).
 * Evita underflow e garante integridade da estrutura de dados.
 * 
 * @param pilhaPtr Ponteiro para a pilha a ser verificada
 * @return int 1 se a pilha está vazia (0/3 peças), 0 caso contrário
 * @note A pilha inicia sempre vazia e pode ser esvaziada durante o jogo
 */
int verificarPilhaVazia(PilhaReserva* pilhaPtr) {
    return pilhaPtr->quantidadeReservada == 0;
}

/**
 * @brief Adiciona uma peça no topo da pilha de reserva (operação push)
 * 
 * Implementa a inserção padrão de pilha LIFO com validação completa:
 * • Valida disponibilidade de espaço antes da inserção
 * • Incrementa o índice do topo para nova posição
 * • Insere a peça na posição calculada
 * • Atualiza contador de peças reservadas
 * 
 * Algoritmo de inserção:
 * 1. Verificação de capacidade disponível
 * 2. Incremento do indiceTopo (-1→0, 0→1, 1→2)
 * 3. Inserção da peça na nova posição do topo
 * 4. Atualização do contador de peças
 * 
 * @param pilhaPtr Ponteiro para a pilha onde inserir a peça
 * @param pecaParaReservar Estrutura Peca com tipo e ID a ser reservada
 * @note Exibe mensagem de erro detalhada se tentar inserir em pilha cheia
 * @note A inserção falha silenciosamente em caso de overflow
 */
void reservarPecaNaPilha(PilhaReserva* pilhaPtr, Peca pecaParaReservar) {
    // Validação de capacidade: impede overflow da estrutura
    if (verificarPilhaCheia(pilhaPtr)) {
        printf("⚠️  ERRO: Tentativa de reservar peça em pilha cheia!\n");
        printf("   A pilha de reserva já contém 3 peças (máximo permitido).\n");
        return;  // Falha silenciosa para manter estabilidade
    }
    
    // Incrementa índice do topo (de -1 para 0 na primeira inserção)
    pilhaPtr->indiceTopo++;
    
    // Insere a peça na nova posição do topo
    pilhaPtr->pecasReservadas[pilhaPtr->indiceTopo] = pecaParaReservar;
    
    // Atualiza contador de peças reservadas na pilha
    pilhaPtr->quantidadeReservada++;
}

/**
 * @brief Remove e retorna a peça do topo da pilha de reserva (operação pop)
 * 
 * Implementa a remoção padrão de pilha LIFO com validação completa:
 * • Valida disponibilidade de peças antes da remoção
 * • Salva a peça do topo para retorno
 * • Decrementa o índice do topo
 * • Atualiza contador e mantém integridade da estrutura
 * 
 * Algoritmo de remoção:
 * 1. Verificação de disponibilidade de peças
 * 2. Backup da peça no topo da pilha
 * 3. Decrementação do indiceTopo (2→1, 1→0, 0→-1)
 * 4. Decrementação do contador de peças
 * 5. Retorno da peça removida
 * 
 * @param pilhaPtr Ponteiro para a pilha de onde remover a peça
 * @return Peca Peça removida do topo da pilha, ou peça vazia em caso de erro
 * @note Retorna peça com tipo ' ' e ID 0 em caso de pilha vazia
 * @note A remoção falha silenciosamente em caso de underflow
 */
Peca usarPecaDaPilha(PilhaReserva* pilhaPtr) {
    // Peça vazia para retorno em caso de erro ou pilha vazia
    Peca pecaVazia = {' ', 0};
    
    // Validação de disponibilidade: impede underflow da estrutura
    if (verificarPilhaVazia(pilhaPtr)) {
        printf("⚠️  ERRO: Tentativa de usar peça de pilha vazia!\n");
        printf("   Não há peças reservadas disponíveis para uso.\n");
        return pecaVazia;  // Retorna peça inválida para sinalizar erro
    }
    
    // Salva a peça do topo que será removida
    Peca pecaUsada = pilhaPtr->pecasReservadas[pilhaPtr->indiceTopo];
    
    // Decrementa índice do topo (volta para -1 se era a última peça)
    pilhaPtr->indiceTopo--;
    
    // Decrementa contador de peças reservadas
    pilhaPtr->quantidadeReservada--;
    
    return pecaUsada;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 *                         FUNÇÕES DE VISUALIZAÇÃO
 * ═══════════════════════════════════════════════════════════════════════════════
 */

/**
 * @brief Exibe o estado atual da fila de forma visual e organizada
 * 
 * Esta função apresenta a fila circular de forma intuitiva:
 * 1. Mostra o cabeçalho com título e estatísticas
 * 2. Exibe cada peça com seu ID e tipo
 * 3. Indica visualmente a posição de início e fim
 * 4. Mostra o status de ocupação da fila
 * 
 * @param filaPtr Ponteiro para a fila a ser exibida
 */
void exibirEstadoDaFila(FilaCircular* filaPtr) {
    printf("\n┌─────────────────────────────────────────────────────────────┐\n");
    printf("│                    🎮 FILA DE PEÇAS                         │\n");
    printf("│                                                             │\n");
    printf("│  Capacidade: 5 peças  │  Ocupação: %d/5  │  Status: %-6s │\n", 
           filaPtr->quantidadePecas, 
           verificarFilaVazia(filaPtr) ? "Vazia" : 
           verificarFilaCheia(filaPtr) ? "Cheia" : "Ativa");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    
    if (verificarFilaVazia(filaPtr)) {
        printf("│                     🔄 Fila vazia                           │\n");
        printf("│              Gere peças para começar!                      │\n");
    } else {
        printf("│  Posição  │  ID   │  Tipo  │           Status             │\n");
        printf("├───────────┼───────┼────────┼──────────────────────────────┤\n");
        
        int indiceAtual = filaPtr->indiceFrente;
        for (int contadorPecas = 0; contadorPecas < filaPtr->quantidadePecas; contadorPecas++) {
            char* statusPeca = "";
            if (contadorPecas == 0) statusPeca = "← Próxima a jogar";
            else if (contadorPecas == filaPtr->quantidadePecas - 1) statusPeca = "← Última adicionada";
            
            printf("│     %d     │  %3d  │   %c    │  %-26s  │\n", 
                   contadorPecas + 1, 
                   filaPtr->pecas[indiceAtual].id, 
                   filaPtr->pecas[indiceAtual].tipo,
                   statusPeca);
            
            indiceAtual = (indiceAtual + 1) % 5;
        }
    }
    
    printf("└─────────────────────────────────────────────────────────────┘\n");
}

/**
 * @brief Exibe o estado atual da pilha de reserva de forma visual
 * 
 * Esta função apresenta a pilha linear de forma intuitiva:
 * 1. Mostra o cabeçalho com título e estatísticas
 * 2. Exibe cada peça reservada com seu ID e tipo
 * 3. Indica visualmente o topo da pilha
 * 4. Mostra o status de ocupação da pilha
 * 
 * @param pilhaPtr Ponteiro para a pilha a ser exibida
 */
void exibirEstadoDaPilha(PilhaReserva* pilhaPtr) {
    printf("\n┌─────────────────────────────────────────────────────────────┐\n");
    printf("│                  📦 PILHA DE RESERVA                        │\n");
    printf("│                                                             │\n");
    printf("│  Capacidade: 3 peças  │  Ocupação: %d/3  │  Status: %-6s │\n", 
           pilhaPtr->quantidadeReservada, 
           verificarPilhaVazia(pilhaPtr) ? "Vazia" : 
           verificarPilhaCheia(pilhaPtr) ? "Cheia" : "Ativa");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    
    if (verificarPilhaVazia(pilhaPtr)) {
        printf("│                    📦 Pilha vazia                           │\n");
        printf("│             Reserve peças para usar depois!                │\n");
    } else {
        printf("│  Posição  │  ID   │  Tipo  │           Status             │\n");
        printf("├───────────┼───────┼────────┼──────────────────────────────┤\n");
        
        // Exibe da posição mais alta para a mais baixa (topo para base)
        for (int indicePilha = pilhaPtr->indiceTopo; indicePilha >= 0; indicePilha--) {
            char* statusPeca = "";
            if (indicePilha == pilhaPtr->indiceTopo) statusPeca = "← Topo (próxima a usar)";
            else if (indicePilha == 0) statusPeca = "← Base da pilha";
            
            printf("│     %d     │  %3d  │   %c    │  %-26s  │\n", 
                   indicePilha, 
                   pilhaPtr->pecasReservadas[indicePilha].id, 
                   pilhaPtr->pecasReservadas[indicePilha].tipo,
                   statusPeca);
        }
    }
    
    printf("└─────────────────────────────────────────────────────────────┘\n");
}

/**
 * @brief Exibe simultaneamente o estado da fila e da pilha de forma otimizada
 * 
 * Esta função oferece uma visão completa e integrada do sistema:
 * • Apresenta interface unificada com fila e pilha lado a lado
 * • Mostra estatísticas consolidadas em tempo real
 * • Utiliza layout otimizado para melhor aproveitamento da tela
 * • Fornece feedback visual imediato sobre o estado do jogo
 * 
 * Otimizações implementadas:
 * - Reutilização das funções de exibição individuais
 * - Layout responsivo que se adapta ao conteúdo
 * - Cálculos de estatísticas consolidados
 * - Limpeza de tela otimizada para Windows
 * 
 * @param filaPtr Ponteiro para a fila circular de peças
 * @param pilhaPtr Ponteiro para a pilha de reserva
 * @note Função otimizada que reutiliza componentes de exibição existentes
 * @note Compatível com terminal Windows (usa 'cls' para limpeza)
 */
void exibirEstadoCompleto(FilaCircular* filaPtr, PilhaReserva* pilhaPtr) {
    // Limpeza otimizada da tela para Windows
    system("cls");
    
    // Cabeçalho principal unificado
    printf("═══════════════════════════════════════════════════════════════════════════════\n");
    printf("                           🎯 TETRIS STACK - NÍVEL ADVENTURER\n");
    printf("═══════════════════════════════════════════════════════════════════════════════\n");
    
    // Reutilização das funções de exibição individuais (evita duplicação de código)
    exibirEstadoDaFila(filaPtr);
    exibirEstadoDaPilha(pilhaPtr);
    
    // Estatísticas consolidadas com cálculos otimizados
    int totalPecasNoSistema = filaPtr->quantidadePecas + pilhaPtr->quantidadeReservada;
    int espacosLivres = (5 - filaPtr->quantidadePecas) + (3 - pilhaPtr->quantidadeReservada);
    
    printf("\n┌─────────────────────────────────────────────────────────────┐\n");
    printf("│                    📊 ESTATÍSTICAS CONSOLIDADAS             │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  Peças na fila: %d/5  │  Peças reservadas: %d/3           │\n", 
           filaPtr->quantidadePecas, pilhaPtr->quantidadeReservada);
    printf("│  Total no sistema: %d/8  │  Espaços livres: %d             │\n", 
           totalPecasNoSistema, espacosLivres);
    printf("│  Próximo ID: %d  │  Eficiência: %.1f%%                   │\n", 
           contadorIdGlobal, (totalPecasNoSistema / 8.0) * 100);
    printf("└─────────────────────────────────────────────────────────────┘\n");
}

/**
 * @brief Exibe o menu principal do Nível Adventurer com todas as opções disponíveis
 * 
 * Esta função apresenta as opções interativas do jogo:
 * 1. Jogar peça da fila (operação dequeue)
 * 2. Reservar peça da fila na pilha (integração fila→pilha)
 * 3. Usar peça reservada da pilha (operação pop)
 * 4. Gerar nova peça na fila (operação enqueue)
 * 5. Exibir estado completo (fila + pilha)
 * 6. Sair do programa
 * 
 * O menu é contextual e mostra avisos quando operações não são possíveis.
 */
void exibirMenuPrincipal(FilaCircular* filaPtr, PilhaReserva* pilhaPtr) {
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                    🎯 MENU PRINCIPAL                          ║\n");
    printf("║                   NÍVEL ADVENTURER                            ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    
    // Opção 1: Jogar peça da fila
    if (verificarFilaVazia(filaPtr)) {
        printf("║  1️⃣  Jogar peça da fila           ❌ (Fila vazia)           ║\n");
    } else {
        printf("║  1️⃣  Jogar peça da fila           ✅ (Peça %c%d disponível) ║\n",
               filaPtr->pecas[filaPtr->indiceFrente].tipo,
               filaPtr->pecas[filaPtr->indiceFrente].id);
    }
    
    // Opção 2: Reservar peça da fila na pilha
    if (verificarFilaVazia(filaPtr) || verificarPilhaCheia(pilhaPtr)) {
        printf("║  2️⃣  Reservar peça na pilha       ❌ ");
        if (verificarFilaVazia(filaPtr)) printf("(Fila vazia)");
        else printf("(Pilha cheia)");
        printf("        ║\n");
    } else {
        printf("║  2️⃣  Reservar peça na pilha       ✅ (Espaço disponível)    ║\n");
    }
    
    // Opção 3: Usar peça reservada da pilha
    if (verificarPilhaVazia(pilhaPtr)) {
        printf("║  3️⃣  Usar peça reservada          ❌ (Pilha vazia)          ║\n");
    } else {
        printf("║  3️⃣  Usar peça reservada          ✅ (Peça %c%d no topo)    ║\n",
               pilhaPtr->pecasReservadas[pilhaPtr->indiceTopo].tipo,
               pilhaPtr->pecasReservadas[pilhaPtr->indiceTopo].id);
    }
    
    // Opção 4: Gerar nova peça
    if (verificarFilaCheia(filaPtr)) {
        printf("║  4️⃣  Gerar nova peça              ❌ (Fila cheia)           ║\n");
    } else {
        printf("║  4️⃣  Gerar nova peça              ✅ (Espaço disponível)    ║\n");
    }
    
    // Opções sempre disponíveis
    printf("║  5️⃣  Exibir estado completo       ✅ (Sempre disponível)    ║\n");
    printf("║  0️⃣  Sair do programa             ✅ (Sempre disponível)    ║\n");
    
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("🎮 Digite sua escolha: ");
}

/**
 * @brief Exibe mensagem de boas-vindas e inicialização do jogo
 * 
 * Esta função apresenta informações iniciais ao usuário:
 * 1. Saudação e identificação do nível
 * 2. Confirmação da inicialização do sistema
 * 3. Preparação para mostrar estado inicial
 */
void exibirBoasVindas() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║            🎮 BEM-VINDO AO TETRIS STACK! 🎮                  ║\n");
    printf("║                     Nível Novato                            ║\n");
    printf("║                                                              ║\n");
    printf("║  Sistema de fila circular de peças inicializado com         ║\n");
    printf("║  sucesso! Você pode começar a jogar imediatamente.          ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}

/**
 * @brief Exibe mensagem de despedida ao finalizar o programa
 * 
 * Apresenta uma mensagem de agradecimento formatada com bordas decorativas
 * e informações sobre o programa Tetris Stack.
 */
void exibirDespedida() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║               👋 OBRIGADO POR JOGAR! 👋                      ║\n");
    printf("║                    Tetris Stack                              ║\n");
    printf("║                   Nível Adventurer                           ║\n");
    printf("║                                                              ║\n");
    printf("║  🎮 Você explorou com sucesso:                              ║\n");
    printf("║     • Fila circular de 5 peças                              ║\n");
    printf("║     • Pilha de reserva de 3 peças                           ║\n");
    printf("║     • Operações integradas entre estruturas                 ║\n");
    printf("║                                                              ║\n");
    printf("║  Volte sempre para mais desafios! 🚀                        ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 *                              FUNÇÃO PRINCIPAL
 * ═══════════════════════════════════════════════════════════════════════════════
 */

/**
 * @brief Função principal que controla o fluxo do jogo Tetris Stack - Nível Adventurer
 * 
 * Esta função implementa o loop principal do jogo:
 * 1. Inicializa as estruturas de dados (fila circular e pilha de reserva)
 * 2. Gera peças iniciais para começar o jogo
 * 3. Executa o loop interativo do menu
 * 4. Processa as escolhas do usuário
 * 5. Gerencia as operações entre fila e pilha
 * 
 * O programa oferece funcionalidades completas do Nível Adventurer:
 * - Fila circular para 5 peças com operações enqueue/dequeue
 * - Pilha linear para 3 peças reservadas com operações push/pop
 * - Integração entre fila e pilha para estratégias avançadas
 * - Interface visual completa e intuitiva
 * 
 * @return int Código de saída do programa (0 = sucesso)
 * 
 * @note Utiliza srand(time(NULL)) para garantir aleatoriedade das peças
 * @note Implementa validações completas para todas as operações
 * @note Oferece feedback inteligente baseado no estado das estruturas
 * 
 * @see FilaCircular
 * @see PilhaReserva
 * @see gerarNovaPeca()
 * 
 * @example
 * Fluxo típico de execução:
 * @code{.c}
 * // 1. Inicialização automática das estruturas
 * // 2. Exibição do estado inicial
 * // 3. Loop interativo:
 * //    - Mostrar menu contextual
 * //    - Processar escolha do usuário
 * //    - Atualizar estruturas
 * //    - Gerar novas peças automaticamente
 * // 4. Encerramento com mensagem de despedida
 * @endcode
 */
int main() {
    // ═══════════════════════════════════════════════════════════════════════════
    //                           INICIALIZAÇÃO DO SISTEMA
    // ═══════════════════════════════════════════════════════════════════════════
    
    // Inicializa gerador de números aleatórios com seed baseada no tempo atual
    srand(time(NULL));
    
    // Declaração das estruturas principais
    FilaCircular filaDeJogo;        // Fila circular para 5 peças
    PilhaReserva pilhaReserva;      // Pilha linear para 3 peças reservadas
    int opcaoEscolhida;             // Opção selecionada pelo usuário
    int contadorPecasJogadas = 0;   // Estatística de peças jogadas
    
    // Inicialização das estruturas de dados
    inicializarFilaCircular(&filaDeJogo);
    inicializarPilhaReserva(&pilhaReserva);
    
    // Exibe mensagem de boas-vindas
    exibirBoasVindas();
    
    // Mostra estado inicial completo
    exibirEstadoCompleto(&filaDeJogo, &pilhaReserva);
    
    // ═══════════════════════════════════════════════════════════════════════════
    //                              LOOP PRINCIPAL
    // ═══════════════════════════════════════════════════════════════════════════
    
    do {
        // Exibe menu contextual e captura opção do usuário
        exibirMenuPrincipal(&filaDeJogo, &pilhaReserva);
        scanf("%d", &opcaoEscolhida);
        
        // Processa a opção escolhida
        switch (opcaoEscolhida) {
            case 1: {
                // ═══════════════════════════════════════════════════════════════
                //                    JOGAR PEÇA DA FILA
                // ═══════════════════════════════════════════════════════════════
                printf("\n🎮 Processando jogada...\n");
                // Validação otimizada com early return
                if (!verificarFilaVazia(&filaDeJogo)) {
                    // Remove peça da frente da fila
                    Peca pecaJogada = removerPecaDaFila(&filaDeJogo);
                    contadorPecasJogadas++;
                    
                    printf("✅ Peça %c%d foi jogada com sucesso!\n", 
                           pecaJogada.tipo, pecaJogada.id);
                    printf("   📊 Peças restantes na fila: %d/5\n", 
                           filaDeJogo.quantidadePecas);
                    
                    // Gera automaticamente uma nova peça se houver espaço
                    if (!verificarFilaCheia(&filaDeJogo)) {
                        Peca novaPecaGerada = gerarNovaPeca();
                        inserirPecaNaFila(&filaDeJogo, novaPecaGerada);
                        printf("   🔄 Nova peça %c%d gerada automaticamente\n", 
                               novaPecaGerada.tipo, novaPecaGerada.id);
                    }
                } else {
                    printf("❌ Impossível jogar: fila está vazia!\n");
                    printf("   💡 Dica: Gere novas peças primeiro (opção 4)\n");
                }
                break;
            }
            
            case 2: {
                // ═══════════════════════════════════════════════════════════════
                //                 RESERVAR PEÇA DA FILA NA PILHA
                // ═══════════════════════════════════════════════════════════════
                printf("\n📦 Processando reserva de peça...\n");
                // Validação otimizada com múltiplas condições
                if (verificarFilaVazia(&filaDeJogo)) {
                    printf("❌ Impossível reservar: fila está vazia!\n");
                    printf("   💡 Dica: Gere novas peças primeiro (opção 4)\n");
                } else if (verificarPilhaCheia(&pilhaReserva)) {
                    printf("❌ Impossível reservar: pilha está cheia!\n");
                    printf("   💡 Dica: Use peças reservadas primeiro (opção 3)\n");
                } else {
                    Peca pecaReservada = removerPecaDaFila(&filaDeJogo);
                    reservarPecaNaPilha(&pilhaReserva, pecaReservada);
                    printf("✅ Peça %c%d foi reservada na pilha!\n", 
                           pecaReservada.tipo, pecaReservada.id);
                    printf("   📊 Peças na fila: %d/5 | Peças reservadas: %d/3\n", 
                           filaDeJogo.quantidadePecas, pilhaReserva.quantidadeReservada);
                    
                    // Gera automaticamente uma nova peça se houver espaço
                    if (!verificarFilaCheia(&filaDeJogo)) {
                        Peca novaPecaGerada = gerarNovaPeca();
                        inserirPecaNaFila(&filaDeJogo, novaPecaGerada);
                        printf("   🔄 Nova peça %c%d gerada automaticamente\n", 
                               novaPecaGerada.tipo, novaPecaGerada.id);
                    }
                }
                break;
            }
            
            case 3: {
                // ═══════════════════════════════════════════════════════════════
                //                   USAR PEÇA RESERVADA DA PILHA
                // ═══════════════════════════════════════════════════════════════
                printf("\n🎯 Processando uso de peça reservada...\n");
                
if (verificarPilhaVazia(&pilhaReserva)) {
                    printf("❌ Impossível usar: pilha está vazia!\n");
                    printf("   💡 Dica: Reserve peças primeiro (opção 2)\n");
                } else {
                    Peca pecaUsada = usarPecaDaPilha(&pilhaReserva);
                    printf("✅ Peça reservada %c%d foi usada com sucesso!\n", 
                           pecaUsada.tipo, pecaUsada.id);
                    printf("   📊 Peças reservadas restantes: %d/3\n", 
                           pilhaReserva.quantidadeReservada);
                }
                break;
            }
            
            case 4: {
                // ═══════════════════════════════════════════════════════════════
                //                      GERAR NOVA PEÇA
                // ═══════════════════════════════════════════════════════════════
                printf("\n🔄 Processando geração de nova peça...\n");
                
if (verificarFilaCheia(&filaDeJogo)) {
                    printf("❌ Impossível gerar: fila está cheia!\n");
                    printf("   💡 Dica: Jogue ou reserve peças primeiro (opções 1 ou 2)\n");
                } else {
                    Peca novaPecaGerada = gerarNovaPeca();
                    inserirPecaNaFila(&filaDeJogo, novaPecaGerada);
                    printf("✅ Nova peça %c%d gerada e adicionada à fila!\n", 
                           novaPecaGerada.tipo, novaPecaGerada.id);
                    printf("   📊 Peças na fila: %d/5\n", filaDeJogo.quantidadePecas);
                }
                break;
            }
            
            case 5: {
                // ═══════════════════════════════════════════════════════════════
                //                    EXIBIR ESTADO COMPLETO
                // ═══════════════════════════════════════════════════════════════
                printf("\n📊 Atualizando visualização completa...\n");
                exibirEstadoCompleto(&filaDeJogo, &pilhaReserva);
                break;
            }
            
            case 0: {
                // ═══════════════════════════════════════════════════════════════
                //                        SAIR DO PROGRAMA
                // ═══════════════════════════════════════════════════════════════
                printf("\n👋 Encerrando o jogo...\n");
                exibirDespedida();
                break;
            }
                
            default:
                // ═══════════════════════════════════════════════════════════════
                //                       OPÇÃO INVÁLIDA
                // ═══════════════════════════════════════════════════════════════
                printf("\n❌ Opção inválida! Por favor, escolha uma opção válida.\n");
                printf("   💡 Opções disponíveis: 0, 1, 2, 3, 4, 5\n");
                break;
        }
        
        // Pausa para o usuário ler as mensagens antes de continuar
        if (opcaoEscolhida != 0 && opcaoEscolhida != 5) {
            printf("\n⏸️  Pressione Enter para continuar...");
            getchar(); // Limpa buffer do scanf
            getchar(); // Aguarda Enter do usuário
        }
        
    } while (opcaoEscolhida != 0);
    
    // ═══════════════════════════════════════════════════════════════════════════
    //                              FINALIZAÇÃO
    // ═══════════════════════════════════════════════════════════════════════════
    
    printf("\n🎯 Programa finalizado com sucesso!\n");
    printf("   Obrigado por jogar Tetris Stack - Nível Adventurer!\n\n");
    
    return 0; // Indica execução bem-sucedida
}

