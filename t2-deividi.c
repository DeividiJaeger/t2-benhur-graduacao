#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "tecla.h"
#include "tela.h"

#define MAX_WORDS 10
#define MAX_WORD_LENGTH 10
#define TEMPO 19

// Declaração das funções
void apresentacao();
void encerramento();
void jogo();
bool quer_jogar_de_novo();
void preenche_palavras(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int *num_palavras);
void mostra_palavras(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int num_palavras, int palavra_selecionada);

int main()
{
    srand(time(0));
    tecla_ini();

    do
    {
        apresentacao();
        jogo();
    } while (quer_jogar_de_novo());
    encerramento();
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
        int decimais = 9 - (int)((tempo_passado - (int)tempo_passado) * 10); // Dígitos após a vírgula (decremento)

        if (resta <= 0)
        {
            printf("Tempo esgotado\n");
            break;
        }

        mostra_palavras(palavras, num_palavras, palavra_selecionada);

        char letra = tecla_le_char();
        printf("%c\n", letra);
        printf("Você tem %d.%d segundos\n", resta, decimais);

        if (palavra_selecionada == -1)
        {
            palavra_selecionada = rand() % num_palavras;
        }
        else
        {
            if (palavras[palavra_selecionada][0] == letra)
            {
                for (int i = 0; palavras[palavra_selecionada][i] != '\0'; i++)
                {
                    palavras[palavra_selecionada][i] = palavras[palavra_selecionada][i + 1];
                }
                if (palavras[palavra_selecionada][0] == '\0')
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
}

void apresentacao()
{
    printf("Você deve adivinhar as palavras digitando a primeira letra.\n");
    printf("Tecle qualquer letra para iniciar. \n");
    tecla_le_char();
    while (tecla_le_char() != '\n');
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

    *num_palavras = 0;
    while (*num_palavras < MAX_WORDS && fscanf(arquivo, "%s", palavras[*num_palavras]) != EOF)
    {
        // Converte as palavras para letras minúsculas
        for (int i = 0; palavras[*num_palavras][i] != '\0'; i++)
        {
            palavras[*num_palavras][i] = tolower(palavras[*num_palavras][i]);
        }
        (*num_palavras)++;
    }

    fclose(arquivo);
}

void mostra_palavras(char palavras[MAX_WORDS][MAX_WORD_LENGTH], int num_palavras, int palavra_selecionada)
{
    for (int i = 0; i < num_palavras; i++)
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
}
