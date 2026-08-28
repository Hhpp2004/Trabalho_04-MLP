//
// Created by henrique on 28/08/2026.
//

#ifndef TRABALHO_04_MULTI_PERCEPTON_H
#define TRABALHO_04_MULTI_PERCEPTON_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

typedef struct mlp {
    int entradas;
    int ocultos;
    int saidas;

    double **peso_entrada_oculta;
    double **pesos_ocultos_saida;

    double *bias_oculta;
    double *bias_saida;

    double *saida_oculta;
    double *saida;

    double *delta_oculta;
    double *delta_saida;

    double taxa_aprendizado;
}mlp;

typedef struct dados {
    int quantidade;
    int entradas;
    int saidas;

    int **entrada;
    int **targets;
}dados;

//TODO Funções de Rede
mlp *criar_mlp(int entradas, int ocultos, int saidas);
void inicializar_pesos(mlp *mlp);
void liberar_mlp(mlp *mlp);

//TODO funções para dados
dados *criar_dados(int quantidade, int entrada, int saidas);
int ler_dados(const char *nome_arquivo, dados *dados, const char *nome_porta);
int ler_dados_letras(const char *nome_arquivo, dados *dados);
void liberar_dados(dados *dados);

//TODO Funções de processo
void forward(mlp *rede, int *entrada);
double erro(mlp *rede, int *target);
void calcular_delta_saida(mlp *rede, const int *target);
void calcular_delta_oculto(mlp *rede);
void atualizar_pesos_saida(mlp *rede);
void  atualizar_pesos_ocultos (mlp *rede, int *entrada);

//TODO Funções de treinamento
void treinar_mlp (mlp *rede, dados *dados, int epocas);
void testar_mlp(mlp *rede, dados *dados);

#endif //TRABALHO_04_MULTI_PERCEPTON_H
