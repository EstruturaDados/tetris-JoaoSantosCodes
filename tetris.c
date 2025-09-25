/*
 * ╔══════════════════════════════════════════════════════════════════════════════╗
 * ║                            TETRIS STACK - NÍVEL NOVATO                      ║
 * ║                        Sistema de Controle de Peças                         ║
 * ╠══════════════════════════════════════════════════════════════════════════════╣
 * ║ Autor: ByteBros Educational Games                                            ║
 * ║ Versão: 1.0 - Nível Novato                                                  ║
 * ║ Data: 2025                                                                   ║
 * ║                                                                              ║
 * ║ Descrição:                                                                   ║
 * ║ Este programa implementa o Nível Novato do desafio Tetris Stack,            ║
 * ║ focando no controle de peças através de uma fila circular.                  ║
 * ║                                                                              ║
 * ║ Funcionalidades:                                                             ║
 * ║ • Fila circular com capacidade fixa de 5 peças                              ║
 * ║ • Geração automática de peças com tipos aleatórios                          ║
 * ║ • Sistema de IDs sequenciais únicos                                         ║
 * ║ • Interface interativa via menu                                             ║
 * ║ • Visualização gráfica do estado da fila                                    ║
 * ╚══════════════════════════════════════════════════════════════════════════════╝
 */

#include <stdio.h>   // Funções de entrada/saída (printf, scanf)
#include <stdlib.h>  // Funções utilitárias (rand, srand)
#include <time.h>    // Funções de tempo (time para seed aleatória)

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 *                           DEFINIÇÕES DE ESTRUTURAS
 * ═══════════════════════════════════════════════════════════════════════════════
 */

/**
 * @brief Estrutura que representa uma peça do Tetris
 * 
 * Esta estrutura armazena as informações básicas de cada peça:
 * - tipo: Caractere que identifica o formato da peça ('I', 'O', 'T', 'L')
 * - id: Número único e sequencial para identificação da peça
 */
typedef struct {
    char tipo;  // Tipo da peça: 'I' (linha), 'O' (quadrado), 'T' (T), 'L' (L)
    int id;     // Identificador único e sequencial da peça
} Peca;

/**
 * @brief Estrutura que implementa uma fila circular de peças
 * 
 * A fila circular permite reutilização eficiente do espaço do array,
 * mantendo sempre 5 peças ativas no sistema:
 * - pecas[5]: Array fixo que armazena as peças
 * - indiceFrente: Posição da primeira peça (próxima a ser jogada)
 * - indiceTras: Posição da última peça inserida
 * - quantidadePecas: Contador atual de peças na fila (0-5)
 */
typedef struct {
    Peca pecas[5];          // Array circular de 5 peças
    int indiceFrente;       // Índice da frente da fila (próxima peça a jogar)
    int indiceTras;         // Índice do final da fila (última peça inserida)
    int quantidadePecas;    // Quantidade atual de peças na fila (0-5)
} FilaCircular;

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 *                              VARIÁVEIS GLOBAIS
 * ═══════════════════════════════════════════════════════════════════════════════
 */

/**
 * @brief Contador global para gerar IDs únicos e sequenciais
 * 
 * Esta variável é incrementada a cada nova peça criada,
 * garantindo que cada peça tenha um identificador único.
 * Inicia em 1 para facilitar a contagem para o usuário.
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
 */
Peca gerarNovaPeca() {
    Peca novaPeca;
    
    // Array com os tipos de peças disponíveis no Tetris clássico
    char tiposDisponiveis[] = {'I', 'O', 'T', 'L'};
    const int totalTipos = 4;
    
    // Seleciona tipo aleatório usando módulo para garantir índice válido
    int indiceAleatorio = rand() % totalTipos;
    novaPeca.tipo = tiposDisponiveis[indiceAleatorio];
    
    // Atribui ID sequencial único e incrementa contador
    novaPeca.id = contadorIdGlobal++;
    
    return novaPeca;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 *                         FUNÇÕES DE CONTROLE DA FILA
 * ═══════════════════════════════════════════════════════════════════════════════
 */

/**
 * @brief Inicializa a fila circular com 5 peças geradas automaticamente
 * 
 * Esta função prepara a fila para uso:
 * 1. Zera os índices de controle
 * 2. Gera 5 peças iniciais para preencher a fila
 * 3. Configura os ponteiros de frente e trás corretamente
 * 
 * A fila sempre inicia cheia (5/5 peças) para garantir que o jogador
 * tenha peças disponíveis desde o início do jogo.
 * 
 * @param filaPtr Ponteiro para a estrutura da fila a ser inicializada
 */
void inicializarFilaCircular(FilaCircular* filaPtr) {
    // Inicializa índices de controle
    filaPtr->indiceFrente = 0;           // Primeira posição do array
    filaPtr->indiceTras = -1;            // Será incrementado para 0 na primeira inserção
    filaPtr->quantidadePecas = 0;        // Fila inicia vazia
    
    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < 5; i++) {
        // Calcula próxima posição circular para inserção
        filaPtr->indiceTras = (filaPtr->indiceTras + 1) % 5;
        
        // Gera e insere nova peça na posição calculada
        filaPtr->pecas[filaPtr->indiceTras] = gerarNovaPeca();
        
        // Incrementa contador de peças
        filaPtr->quantidadePecas++;
    }
}

/**
 * @brief Verifica se a fila está completamente cheia
 * 
 * @param filaPtr Ponteiro para a fila a ser verificada
 * @return int 1 se a fila está cheia (5 peças), 0 caso contrário
 */
int verificarFilaCheia(FilaCircular* filaPtr) {
    return filaPtr->quantidadePecas == 5;
}

/**
 * @brief Verifica se a fila está completamente vazia
 * 
 * @param filaPtr Ponteiro para a fila a ser verificada
 * @return int 1 se a fila está vazia (0 peças), 0 caso contrário
 */
int verificarFilaVazia(FilaCircular* filaPtr) {
    return filaPtr->quantidadePecas == 0;
}

/**
 * @brief Adiciona uma peça no final da fila circular (operação enqueue)
 * 
 * Esta função implementa a inserção em fila circular:
 * 1. Verifica se há espaço disponível
 * 2. Calcula a próxima posição usando aritmética modular
 * 3. Insere a peça e atualiza os controles
 * 
 * @param filaPtr Ponteiro para a fila onde inserir
 * @param pecaParaInserir Peça a ser inserida na fila
 */
void inserirPecaNaFila(FilaCircular* filaPtr, Peca pecaParaInserir) {
    // Validação: verifica se há espaço na fila
    if (verificarFilaCheia(filaPtr)) {
        printf("⚠️  ERRO: Tentativa de inserir peça em fila cheia!\n");
        return;
    }
    
    // Calcula próxima posição circular (0-4)
    filaPtr->indiceTras = (filaPtr->indiceTras + 1) % 5;
    
    // Insere a peça na posição calculada
    filaPtr->pecas[filaPtr->indiceTras] = pecaParaInserir;
    
    // Atualiza contador de peças
    filaPtr->quantidadePecas++;
}

/**
 * @brief Remove e retorna a peça da frente da fila (operação dequeue)
 * 
 * Esta função implementa a remoção em fila circular:
 * 1. Verifica se há peças disponíveis
 * 2. Salva a peça a ser removida
 * 3. Atualiza o índice da frente usando aritmética modular
 * 4. Decrementa o contador e retorna a peça
 * 
 * @param filaPtr Ponteiro para a fila de onde remover
 * @return Peca Peça removida da frente da fila, ou peça vazia em caso de erro
 */
Peca removerPecaDaFila(FilaCircular* filaPtr) {
    // Peça vazia para retorno em caso de erro
    Peca pecaVazia = {' ', 0};
    
    // Validação: verifica se há peças na fila
    if (verificarFilaVazia(filaPtr)) {
        printf("⚠️  ERRO: Tentativa de remover peça de fila vazia!\n");
        return pecaVazia;
    }
    
    // Salva a peça que será removida
    Peca pecaRemovida = filaPtr->pecas[filaPtr->indiceFrente];
    
    // Atualiza índice da frente para próxima posição circular
    filaPtr->indiceFrente = (filaPtr->indiceFrente + 1) % 5;
    
    // Decrementa contador de peças
    filaPtr->quantidadePecas--;
    
    return pecaRemovida;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 *                         FUNÇÕES DE VISUALIZAÇÃO
 * ═══════════════════════════════════════════════════════════════════════════════
 */

/**
 * @brief Exibe o estado atual da fila de forma visual e organizada
 * 
 * Esta função apresenta informações completas sobre a fila:
 * 1. Cabeçalho identificando a seção
 * 2. Representação visual das peças em formato tabular
 * 3. Estatísticas da fila (quantidade, próxima peça)
 * 4. Tratamento especial para fila vazia
 * 
 * A visualização usa caracteres Unicode para criar bordas elegantes
 * e organiza as peças da frente para trás da fila.
 * 
 * @param filaPtr Ponteiro para a fila a ser exibida
 */
void exibirEstadoDaFila(FilaCircular* filaPtr) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                    ESTADO ATUAL DA FILA                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("📋 Peças na fila (frente → trás):\n\n");
    
    // Tratamento especial para fila vazia
    if (verificarFilaVazia(filaPtr)) {
        printf("❌ Fila completamente vazia!\n");
        printf("   Não há peças disponíveis para jogar.\n");
        return;
    }
    
    // Cabeçalho da tabela visual
    printf("┌─────┬─────┬─────┬─────┬─────┐\n");
    printf("│");
    
    // Exibe cada posição da fila (5 posições fixas)
    for (int posicao = 0; posicao < 5; posicao++) {
        // Calcula índice real no array circular
        int indiceReal = (filaPtr->indiceFrente + posicao) % 5;
        
        // Verifica se esta posição contém uma peça válida
        if (posicao < filaPtr->quantidadePecas) {
            // Formata: tipo + ID (ex: "I1", "O2")
            printf(" %c%-2d │", 
                   filaPtr->pecas[indiceReal].tipo, 
                   filaPtr->pecas[indiceReal].id);
        } else {
            // Posição vazia representada por traços
            printf("  -  │");
        }
    }
    
    // Rodapé da tabela
    printf("\n└─────┴─────┴─────┴─────┴─────┘\n");
    
    // Informações estatísticas da fila
    printf("📊 Estatísticas:\n");
    printf("   • Quantidade atual: %d/5 peças\n", filaPtr->quantidadePecas);
    
    // Mostra próxima peça apenas se fila não estiver vazia
    if (!verificarFilaVazia(filaPtr)) {
        printf("   • Próxima peça a jogar: %c%d\n", 
               filaPtr->pecas[filaPtr->indiceFrente].tipo, 
               filaPtr->pecas[filaPtr->indiceFrente].id);
    }
    
    printf("\n");
}

/**
 * @brief Exibe o menu principal do jogo com opções disponíveis
 * 
 * Esta função apresenta uma interface visual atrativa para o usuário:
 * 1. Título do jogo e nível atual
 * 2. Lista de opções numeradas
 * 3. Prompt para entrada do usuário
 * 
 * O design usa caracteres Unicode para criar bordas elegantes
 * e organizar as informações de forma clara.
 */
void exibirMenuPrincipal() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                        TETRIS STACK                         ║\n");
    printf("║                      🎮 Nível Novato 🎮                      ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║  1️⃣  - Jogar peça (remover da frente da fila)               ║\n");
    printf("║  0️⃣  - Sair do jogo                                         ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("🎯 Escolha uma opção: ");
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
 * @brief Exibe mensagem de despedida e estatísticas finais
 * 
 * @param totalPecasJogadas Número total de peças jogadas durante a sessão
 */
void exibirDespedida(int totalPecasJogadas) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║               👋 OBRIGADO POR JOGAR! 👋                      ║\n");
    printf("║                    Tetris Stack                              ║\n");
    printf("║                                                              ║\n");
    printf("║  📊 Estatísticas da sessão:                                 ║\n");
    printf("║     • Total de peças jogadas: %-3d                          ║\n", totalPecasJogadas);
    printf("║     • Peças geradas: %-3d                                   ║\n", contadorIdGlobal - 1);
    printf("║                                                              ║\n");
    printf("║  Volte sempre para mais desafios! 🎮                        ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 *                              FUNÇÃO PRINCIPAL
 * ═══════════════════════════════════════════════════════════════════════════════
 */

/**
 * @brief Função principal que controla o fluxo do jogo
 * 
 * Esta função implementa o loop principal do jogo:
 * 1. Inicialização do sistema (seed aleatória, fila)
 * 2. Apresentação das boas-vindas
 * 3. Loop principal de interação com usuário
 * 4. Processamento das opções do menu
 * 5. Finalização e estatísticas
 * 
 * O programa mantém controle de estatísticas e oferece
 * uma experiência de usuário fluida com pausas apropriadas.
 * 
 * @return int Código de saída (0 = sucesso)
 */
int main() {
    // ═══════════════════════════════════════════════════════════════════════════
    //                              INICIALIZAÇÃO
    // ═══════════════════════════════════════════════════════════════════════════
    
    // Inicializa gerador de números aleatórios com seed baseada no tempo atual
    srand(time(NULL));
    
    // Declaração das variáveis principais
    FilaCircular filaDeJogo;        // Estrutura principal da fila
    int opcaoEscolhida;             // Opção selecionada pelo usuário
    int contadorPecasJogadas = 0;   // Estatística de peças jogadas
    
    // Inicializa a fila com 5 peças geradas automaticamente
    inicializarFilaCircular(&filaDeJogo);
    
    // Exibe mensagem de boas-vindas
    exibirBoasVindas();
    
    // Mostra estado inicial da fila
    exibirEstadoDaFila(&filaDeJogo);
    
    // ═══════════════════════════════════════════════════════════════════════════
    //                              LOOP PRINCIPAL
    // ═══════════════════════════════════════════════════════════════════════════
    
    do {
        // Exibe menu e captura opção do usuário
        exibirMenuPrincipal();
        scanf("%d", &opcaoEscolhida);
        
        // Processa a opção escolhida
        switch (opcaoEscolhida) {
            case 1: {
                // ═══════════════════════════════════════════════════════════════
                //                        JOGAR PEÇA
                // ═══════════════════════════════════════════════════════════════
                
                if (!verificarFilaVazia(&filaDeJogo)) {
                    // Remove peça da frente da fila
                    Peca pecaJogada = removerPecaDaFila(&filaDeJogo);
                    contadorPecasJogadas++;
                    
                    printf("\n🎯 PEÇA JOGADA COM SUCESSO!\n");
                    printf("   Tipo: %c | ID: %d\n", pecaJogada.tipo, pecaJogada.id);
                    
                    // Gera e adiciona nova peça automaticamente
                    Peca novaPecaGerada = gerarNovaPeca();
                    inserirPecaNaFila(&filaDeJogo, novaPecaGerada);
                    
                    printf("✨ NOVA PEÇA ADICIONADA:\n");
                    printf("   Tipo: %c | ID: %d\n", novaPecaGerada.tipo, novaPecaGerada.id);
                    
                    // Exibe estado atualizado da fila
                    exibirEstadoDaFila(&filaDeJogo);
                } else {
                    printf("\n❌ ERRO: Não há peças disponíveis para jogar!\n");
                    printf("   A fila está vazia. Isso não deveria acontecer no Nível Novato.\n");
                }
                break;
            }
            
            case 0:
                // ═══════════════════════════════════════════════════════════════
                //                          SAIR DO JOGO
                // ═══════════════════════════════════════════════════════════════
                exibirDespedida(contadorPecasJogadas);
                break;
                
            default:
                // ═══════════════════════════════════════════════════════════════
                //                        OPÇÃO INVÁLIDA
                // ═══════════════════════════════════════════════════════════════
                printf("\n❌ OPÇÃO INVÁLIDA!\n");
                printf("   Por favor, escolha uma opção válida do menu.\n");
                printf("   Opções disponíveis: 1 (Jogar) ou 0 (Sair)\n");
                break;
        }
        
        // Pausa para melhor experiência do usuário (exceto ao sair)
        if (opcaoEscolhida != 0) {
            printf("\n⏸️  Pressione Enter para continuar...");
            getchar(); // Limpa buffer do scanf
            getchar(); // Aguarda Enter do usuário
        }
        
    } while (opcaoEscolhida != 0);
    
    // ═══════════════════════════════════════════════════════════════════════════
    //                              FINALIZAÇÃO
    // ═══════════════════════════════════════════════════════════════════════════
    
    return 0; // Indica execução bem-sucedida
}

