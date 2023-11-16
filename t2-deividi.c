//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "tecla.h"
#include "tela.h"

#define MAX_WORDS 920
#define MAX_WORD_LENGTH 15
#define TEMPO 19

// Declaração das funções
void apresentacao();
void encerramento();
void jogo();
bool quer_jogar_de_novo();
void preenche_palavras(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int *num_palavras);
void imprime_tela(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int num_palavras, int palavra_selecionada, int tempo_restante, int decimais);
bool verifica_letra(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int num_palavras, char letra, int *palavra_selecionada);

int main()
{
    srand(time(0));
    tecla_ini();
    do
    {
        apresentacao();
        tela_ini();
        jogo();
    } while (quer_jogar_de_novo());
    encerramento();

    tela_fim();

    return 0;
}

void jogo()
{
    char palavras[MAX_WORDS][MAX_WORD_LENGTH];
    int num_palavras = 0;
    preenche_palavras(palavras, &num_palavras);

    int palavra_selecionada = -1;
    double t0 = tela_relogio();
    double agora, tempo_passado;



    while (num_palavras > 0)
    {
        agora = tela_relogio();
        tempo_passado = agora - t0;
        int resta = TEMPO - (int)tempo_passado;
        int decimais = 9 - (int)((tempo_passado - (int)tempo_passado) * 10);
        if (resta <= 0 && decimais <= 0)
        {
            printf("Tempo esgotado\n");
            break;
        }

        imprime_tela(palavras, num_palavras, palavra_selecionada, resta, decimais);

        char letra = tecla_le_char();
        printf("%c\n", letra);
        printf("Você tem %d.%d segundos\n", resta, decimais);

        if (verifica_letra(palavras, num_palavras, letra, &palavra_selecionada))
        {
            if (strlen(palavras[palavra_selecionada]) == 0)
            {
                for (int i = palavra_selecionada; i < num_palavras - 1; i++)
                {
                    strcpy(palavras[i], palavras[i + 1]);
                }
                num_palavras--;
                palavra_selecionada = -1;
            }
        }
    }
}


bool verifica_letra(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int num_palavras, char letra, int *palavra_selecionada)
{
    // Se a palavra já foi escolhida, verifica se a letra corresponde à próxima letra da palavra
    if (*palavra_selecionada != -1)
    {
        if (palavras[*palavra_selecionada][0] == letra)
        {
            for (int i = 0; palavras[*palavra_selecionada][i] != '\0'; i++)
            {
                palavras[*palavra_selecionada][i] = palavras[*palavra_selecionada][i + 1];
            }
            return true;
        }
        return false;
    }

    // Se a palavra ainda não foi escolhida, verifica se a letra corresponde à primeira letra de alguma palavra
    for (int i = 0; i < num_palavras; i++)
    {
        if (palavras[i][0] == letra)
        {
            *palavra_selecionada = i;
            printf("Palavra escolhida: %s\n", palavras[*palavra_selecionada]);
            return true;
        }
    }

    return false;
}

void apresentacao()
{
    printf("Você deve adivinhar as palavras digitando a primeira letra.\n");
    printf("Tecle qualquer letra para iniciar. \n");
    while (tecla_le_char() != '\n')
        ;
}

void encerramento()
{
    printf("Parabéns, você adivinhou todas as palavras!\n");
}

bool quer_jogar_de_novo()
{
    char c;
    printf("Digite 's' para jogar de novo ou qualquer outra tecla para sair: \n");
    c = tecla_le_char();
    while (c != '\n')
    {
        if (c == 's' || c == 'S')
        {
            printf("\n");
            return true;
        }
        c = tecla_le_char();
    }
    printf("\n");
    return false;
}

void preenche_palavras(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int *num_palavras)
{
    FILE *arquivo;
    arquivo = fopen("palavras.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de palavras.\n");
        exit(1);
    }

    // Lê todas as palavras do arquivo
    char todas_as_palavras[MAX_WORDS][MAX_WORD_LENGTH];
    int total_palavras = 0;

    while (total_palavras < MAX_WORDS && fscanf(arquivo, "%s", todas_as_palavras[total_palavras]) != -1)
    {
        total_palavras++;
    }

    fclose(arquivo);

    // Agora, selecione aleatoriamente 10 palavras dentre as disponíveis
    *num_palavras = 0;

    while (*num_palavras < 10 && total_palavras > 0)
    {
        int indice_aleatorio = rand() % total_palavras;

        // Copie a palavra selecionada para o vetor final
        strcpy(palavras[*num_palavras], todas_as_palavras[indice_aleatorio]);

        // Remova a palavra do vetor de todas as palavras
        for (int i = indice_aleatorio; i < total_palavras - 1; i++)
        {
            strcpy(todas_as_palavras[i], todas_as_palavras[i + 1]);
        }

        total_palavras--;
        (*num_palavras)++;
    }
}

void imprime_tela(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int num_palavras, int palavra_selecionada, int tempo_restante, int decimais)
{
    tela_limpa();

    printf("Tempo Restante: %d.%d\n", tempo_restante, decimais);

    int inicio = 0;
    int fim = num_palavras;

    // Ajusta os limites para mostrar apenas 10 palavras
    if (num_palavras > 10)
    {
        inicio = palavra_selecionada - 4;
        fim = palavra_selecionada + 5;

        // Garante que não ultrapasse os limites do array
        if (inicio < 0)
        {
            inicio = 0;
            fim = 10;
        }
        else if (fim > num_palavras)
        {
            fim = num_palavras;
            inicio = fim - 10;
        }
    }

    for (int i = inicio; i < fim; i++)
    {
        if (i == palavra_selecionada)
        {
            tela_cor_letra(255, 255, 0);
            printf("%s\n", palavras[i]);
            tela_cor_normal();
        }
        else
        {
            printf("%s\n", palavras[i]);
        }
    }

    tela_atualiza(); // Mova esta linha para garantir que a tela seja atualizada imediatamente após a impressão
}
