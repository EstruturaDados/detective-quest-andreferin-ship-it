//fiz uma versao adaptada perseguicao batman-coringa
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //permite uilizar strncpy 

//declarando struct sala
struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
};

//fazendo programa modular conforme ensinado anteriormente
void liberarMemoria(struct Sala* raiz);
void limparBuffer(void);
struct Sala* criarSala(const char* nome, struct Sala* esq, struct Sala* dir);
struct Sala* criarMansao(void);
void pressionarEnter(void);
void mapearFuga(struct Sala* caminho[], int topo, struct Sala* atual);//quiz fazer com a possibilidade de retorno ao comodo anterior

/* --- Função principal --- */
int main(void) {
    printf("O Coringa te persegue, e você foge desesperadamente em busca de abrigo....\n");
    printf("Para a sua sorte você se depara com uma mansão a qual possui uma imponente placa com uma letra \"W\" impressa.\n");
    printf("Macacos me mordam! É a mansão Wayne.\n");
    printf("A imensa porta está apenas encostada, você entra e acessa ao Hall,\n");
    printf("no entanto, o Coringa é destemido e continua te perseguindo.\n");
    printf("Procure Bruce pelos cômodos da mansão, busque salvação!\n\n");

    struct Sala* mansao = criarMansao();
    if (mansao == NULL) {
        printf("Erro ao criar a mansão.\n");
        return 1;
    }

    struct Sala* atual = mansao;
    struct Sala* caminho[50];  // Pilha para guardar o caminho percorrido, quiz fazer com possibilidade de retorno ao comodo anterior - opcao3
    int topo = -1;
    int opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);
        printf("Escolha uma opção:\n");
        printf("1 - Acessar porta à direita\n");
        printf("2 - Acessar porta à esquerda\n");
        printf("3 - Voltar ao cômodo anterior\n");
        printf("0 - Sair do jogo\n");
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            pressionarEnter();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: // Direita
                if (atual->direita != NULL) {
                    caminho[++topo] = atual;
                    atual = atual->direita;
                    printf("O Coringa se aproxima, continue fugindo!\n");
                    pressionarEnter();
                } else {
                    printf("\nVocê foi alcançado, o Batmóvel quebrou e Batman não chega a tempo.\n");
                    printf("Boa sorte com o Coringa! HAHAHAHAHAHAHAHA.\n");
                    mapearFuga(caminho, topo, atual);
                    liberarMemoria(mansao);
                    return 0;
                }
                break;

            case 2: // Esquerda
                if (atual->esquerda != NULL) {
                    caminho[++topo] = atual;
                    atual = atual->esquerda;
                    printf("O Coringa se aproxima, continue fugindo!\n");
                    pressionarEnter();
                } else {
                    printf("\nVocê foi alcançado, o Batmóvel quebrou e Batman não chega a tempo.\n");
                    printf("Boa sorte com o Coringa! HAHAHAHAHAHAHAHA.\n");
                    mapearFuga(caminho, topo, atual);
                    liberarMemoria(mansao);
                    return 0;
                }
                break;

            case 3: // Voltar
                if (topo >= 0) {
                    atual = caminho[topo--];
                    printf("Você retorna ao cômodo anterior.\n");
                } else {
                    printf("Você já está no Hall, se sair o Coringa te pega!\n");
                }
                pressionarEnter();
                break;

            case 0: // Sair
                printf("Quanto medo do Coringa.....\n");
                liberarMemoria(mansao);
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
                pressionarEnter();
                break;
        }

        // Verifica se o jogador chegou a uma folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê foi alcançado, o Batmóvel quebrou e Batman não chega a tempo.\n");
            printf("Boa sorte com o Coringa! HAHAHAHAHAHAHAHA.\n");
            mapearFuga(caminho, topo, atual);
            liberarMemoria(mansao);
            return 0;
        }
    }

    liberarMemoria(mansao);
    return 0;
}

/* --- Implementações auxiliares --- */
//estrutura que chamarao as salas para compor mansao

struct Sala* criarSala(const char* nome, struct Sala* esq, struct Sala* dir) {
    struct Sala* nova = (struct Sala*) malloc(sizeof(struct Sala));
    if (nova == NULL) {
        fprintf(stderr, "Erro: falha na alocação de memória para nova sala.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(nova->nome, nome, sizeof(nova->nome) - 1);
    nova->nome[sizeof(nova->nome) - 1] = '\0';
    nova->esquerda = esq;
    nova->direita = dir;
    return nova;
}
//mansao já vem pre definida
/* Estrutura completa da Mansão Wayne */
struct Sala* criarMansao(void) {
    struct Sala* banheiro1 = criarSala("Banheiro", NULL, NULL);
    struct Sala* sacada = criarSala("Sacada", NULL, NULL);
    struct Sala* dispensa = criarSala("Dispensa", NULL, NULL);
    struct Sala* banheiro2 = criarSala("Banheiro", NULL, NULL);
    struct Sala* salaMusica = criarSala("Sala de Musica", NULL, NULL);
    struct Sala* quarto2 = criarSala("Quarto2", NULL, NULL);
    struct Sala* jardim = criarSala("Jardim", NULL, NULL);
    struct Sala* parque = criarSala("Parque", NULL, NULL);

    struct Sala* quarto1 = criarSala("Quarto1", banheiro1, sacada);//nivel 3 da arvore mansao
    struct Sala* salaLareira = criarSala("Sala Lareira", dispensa, banheiro2);
    struct Sala* varanda = criarSala("Varanda", jardim, parque);
    struct Sala* cozinha = criarSala("Cozinha", varanda, salaMusica);

    struct Sala* biblioteca = criarSala("Biblioteca", quarto1, salaLareira);//nivel 2 da arvore mansao
    struct Sala* salaEstar = criarSala("Sala de Estar", cozinha, quarto2);

    struct Sala* hall = criarSala("Hall", biblioteca, salaEstar);//nivel 1 da arvore mansao
    return hall;//nivel 0 da arvore mansao
}

/* Libera memória da árvore (pós-ordem) */
//funcao recursiva para liberacao de memoria

void liberarMemoria(struct Sala* raiz) {
    if (raiz == NULL) return;
    liberarMemoria(raiz->esquerda);
    liberarMemoria(raiz->direita);
    free(raiz);
}

/* Limpa buffer do teclado */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Espera o jogador pressionar Enter */
void pressionarEnter(void) {
    printf("Pressione Enter para continuar.....");
    getchar();
}

/* --- mostra o caminho percorrido durante a fuga --- */
void mapearFuga(struct Sala* caminho[], int topo, struct Sala* atual) {
    printf("Você fugiu acessando - ");
    for (int i = 0; i <= topo; i++) {
        printf("%s, ", caminho[i]->nome);
    }
    printf("e foi capturado no %s!\n", atual->nome);
}

