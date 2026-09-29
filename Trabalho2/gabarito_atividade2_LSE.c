/*
 * Atividade Avaliada 2 - Números Aleatórios
 * Lista Simplesmente Encadeada (LSE) com alocação dinâmica
 *
 * Compilar: gcc gabarito_atividade2_LSE.c -o gabarito -lm
 * Executar: ./gabarito
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TOTAL 1000
#define LINHAS 50
#define COLUNAS 20

typedef struct No
{
    int valor;
    struct No *prox;
} No;

typedef struct
{
    No *primeiro;
    int qtd;
} Lista;

void iniciarLista(Lista *l)
{
    l->primeiro = NULL;
    l->qtd = 0;
}

static No *criarNo(int valor)
{
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro: falha na alocacao de memoria.\n");
        exit(EXIT_FAILURE);
    }
    novo->valor = valor;
    novo->prox = NULL;
    return novo;
}

void inserirInicio(Lista *l, int valor)
{
    No *novo = criarNo(valor);
    novo->prox = l->primeiro;
    l->primeiro = novo;
    l->qtd++;
}

void inserirFim(Lista *l, int valor)
{
    No *novo = criarNo(valor);
    if (l->primeiro == NULL)
    {
        l->primeiro = novo;
    }
    else
    {
        No *aux = l->primeiro;
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo;
    }
    l->qtd++;
}

void inserirOrdenado(Lista *l, int valor)
{

    if (l->primeiro == NULL || valor <= l->primeiro->valor)
    {
        inserirInicio(l, valor);
        return;
    }

    No *aux = l->primeiro;
    while (aux->prox != NULL && aux->prox->valor <= valor)
        aux = aux->prox;

    if (aux->prox == NULL)
    {
        inserirFim(l, valor);
        return;
    }

    No *novo = criarNo(valor);
    novo->prox = aux->prox;
    aux->prox = novo;
    l->qtd++;
}

int removerInicio(Lista *l)
{
    if (l->primeiro == NULL)
        return 0;
    No *rem = l->primeiro;
    l->primeiro = rem->prox;
    free(rem);
    l->qtd--;
    return 1;
}

int removerFim(Lista *l)
{
    if (l->primeiro == NULL)
        return 0;

    if (l->primeiro->prox == NULL)
    {
        free(l->primeiro);
        l->primeiro = NULL;
    }
    else
    {
        No *aux = l->primeiro;
        while (aux->prox->prox != NULL)
            aux = aux->prox;
        free(aux->prox);
        aux->prox = NULL;
    }
    l->qtd--;
    return 1;
}

int removerValor(Lista *l, int valor)
{
    if (l->primeiro == NULL)
        return 0;

    if (l->primeiro->valor == valor)
        return removerInicio(l);

    No *aux = l->primeiro;
    while (aux->prox != NULL && aux->prox->valor != valor)
        aux = aux->prox;

    if (aux->prox == NULL)
        return 0;

    No *rem = aux->prox;
    aux->prox = rem->prox;
    free(rem);
    l->qtd--;
    return 1;
}

void liberarLista(Lista *l)
{
    No *aux = l->primeiro;
    while (aux != NULL)
    {
        No *prox = aux->prox;
        free(aux);
        aux = prox;
    }
    l->primeiro = NULL;
    l->qtd = 0;
}

void analisar(const Lista *l)
{
    if (l->primeiro == NULL)
    {
        printf("Lista vazia.\n");
        return;
    }

    int menor = l->primeiro->valor;
    int maior = l->primeiro->valor;
    double soma = 0.0;
    int distintos = 0;
    int valoresRepetidos = 0;
    int duplicados = 0;

    /* 1ª passada: menor, maior e soma */
    for (const No *q = l->primeiro; q != NULL; q = q->prox)
    {
        if (q->valor < menor)
            menor = q->valor;
        if (q->valor > maior)
            maior = q->valor;
        soma += q->valor;
    }

    const No *p = l->primeiro;
    while (p != NULL)
    {
        int atual = p->valor;
        int cont = 0;
        while (p != NULL && p->valor == atual)
        {
            cont++;
            p = p->prox;
        }
        distintos++;
        if (cont > 1)
        {
            valoresRepetidos++;
            duplicados += cont - 1;
        }
    }

    double media = soma / l->qtd;

    double somaQuad = 0.0;
    for (const No *q = l->primeiro; q != NULL; q = q->prox)
        somaQuad += (q->valor - media) * (q->valor - media);
    double desvio = sqrt(somaQuad / l->qtd);

    printf("===== ANALISE ESTATISTICA =====\n");
    printf("Quantidade de elementos      : %d\n", l->qtd);
    printf("Menor valor                  : %d\n", menor);
    printf("Maior valor                  : %d\n", maior);
    printf("Media aritmetica             : %.4f\n", media);
    printf("Desvio padrao                : %.4f\n", desvio);
    printf("Valores distintos            : %d\n", distintos);
    printf("Valores que se repetem       : %d\n", valoresRepetidos);
    printf("Total de elementos duplicados: %d\n", duplicados);
    printf("\n");
}

void exibirMatriz(const Lista *l)
{
    printf("===== LISTA ORDENADA (%d x %d) =====\n", LINHAS, COLUNAS);
    const No *p = l->primeiro;
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (p != NULL)
            {
                printf("%5d", p->valor);
                p = p->prox;
            }
        }
        printf("\n");
    }
    printf("\n");
}

static void exibirResumo(const Lista *l, const char *titulo)
{
    printf("%s (qtd = %d): ", titulo, l->qtd);
    const No *p = l->primeiro;
    for (int i = 0; i < 5 && p != NULL; i++, p = p->prox)
        printf("%d ", p->valor);
    printf("... ");
    printf("\n");
}

int main(void)
{
    Lista lista;
    iniciarLista(&lista);

    srand((unsigned)time(NULL));

    for (int i = 0; i < TOTAL; i++)
    {
        int num = rand() % 1001;
        inserirOrdenado(&lista, num);
    }

    printf("===== DEMONSTRACAO DAS OPERACOES =====\n");
    exibirResumo(&lista, "Lista inicial          ");

    inserirInicio(&lista, -1);
    inserirFim(&lista, 5000);
    inserirOrdenado(&lista, 2000);
    exibirResumo(&lista, "Apos inserirInicio(-1), inserirFim(5000), inserirOrdenado(2000)");

    removerInicio(&lista);
    removerFim(&lista);
    removerValor(&lista, 2000);
    if (!removerValor(&lista, 99999))
        printf("removerValor(99999): valor nao encontrado (esperado)\n");
    exibirResumo(&lista, "Apos removerInicio, removerFim e removerValor");
    printf("\n");

    analisar(&lista);
    exibirMatriz(&lista);

    liberarLista(&lista);
    printf("Memoria liberada. Elementos restantes na lista: %d\n", lista.qtd);

    return 0;
}