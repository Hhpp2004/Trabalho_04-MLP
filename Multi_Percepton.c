//
// Created by henrique on 28/08/2026.
//
#include "Multi_Percepton.h"


/*
 * TODO Aqui temos as funções para criar, inicializar e finalizar a rede
 */


//criando a estrutura
mlp *criar_mlp(int entrada, int ocultos, int saidas) {
    //alocando a rede na memoria
    mlp *mlp1 = malloc(sizeof(mlp));

    if (mlp1 != NULL) {
        //definindo entradas, saidas e camadas ocultas
        mlp1->entradas = entrada;
        mlp1->ocultos = ocultos;
        mlp1->saidas = saidas;
        mlp1->taxa_aprendizado = 0.1;

        //criando o vetor de bias da saida e da camada oculto
        mlp1->bias_oculta = malloc(ocultos * sizeof(double));
        mlp1->bias_saida = malloc(saidas * sizeof(double));

        //criando os vetores de delta
        mlp1->delta_oculta = malloc(ocultos * sizeof(double));
        mlp1->delta_saida = malloc(saidas * sizeof(double));

        //criando o vetor da saída da camada oculta e das saídas
        mlp1->saida = malloc(saidas * sizeof(double));
        mlp1->saida_oculta = malloc(ocultos * sizeof(double));

        //criando as matrizes para cruzamento de dados para a rede identificar posteriormente

        mlp1->peso_entrada_oculta = malloc(sizeof(double) * entrada);
        for (int i = 0; i < entrada;i++) {
            mlp1->peso_entrada_oculta[i] = malloc(ocultos * sizeof(double));
        }

        mlp1->pesos_ocultos_saida = malloc(sizeof(double) * ocultos);
        for (int i = 0; i < ocultos; i++) {
            mlp1->pesos_ocultos_saida[i] = malloc(saidas * sizeof(double));
        }

        return mlp1;
    } else {
        return NULL;
    }
}

//Definindo valores para pesos de bias para iniciar o processo
void inicializar_pesos(mlp *mlp) {
    for (int i = 0; i < mlp->entradas; i++) {
        for (int j = 0; j < mlp->ocultos; j++) {
            mlp->peso_entrada_oculta[i][j] = ((double) rand() / RAND_MAX) - 0.5;
        }
    }

    for (int i = 0; i < mlp->ocultos; i++) {
        for (int j = 0; j < mlp->saidas; j++) {
            mlp->pesos_ocultos_saida[i][j] = ((double) rand() / RAND_MAX) - 0.5;
        }
    }

    for (int i = 0; i < mlp->saidas; i++) {
        mlp->bias_saida[i] = ((double) rand() / RAND_MAX) - 0.5;
    }

    for (int i = 0; i < mlp->ocultos; i++) {
        mlp->bias_oculta[i] = ((double) rand() / RAND_MAX) - 0.5;
    }
}

//Apagando e liberando memoria
void liberar_mlp(mlp * mlp) {
    if (mlp != NULL) {
        for (int i = 0;i < mlp->entradas; i++) {
            free(mlp->peso_entrada_oculta[i]);
        }
        free(mlp->peso_entrada_oculta);

        for (int i = 0;i < mlp->ocultos; i++) {
            free(mlp->pesos_ocultos_saida[i]);
        }
        free(mlp->pesos_ocultos_saida);

        free(mlp->bias_oculta);
        free(mlp->bias_saida);

        free(mlp->delta_oculta);
        free(mlp->delta_saida);

        free(mlp->saida_oculta);
        free(mlp->saida);

        free(mlp);
    }
}

/*
 * TODO agora vamos trabalhar com funções
 * que faz reconhecimento de dados externos para treinamento futuro
 * vamos fazer alocação dinamica da estrutura de dados
*/

dados *criar_dados(int quantidade, int entrada, int saidas) {
    dados *dados1 = malloc(sizeof(dados));

    if (dados1 != NULL) {
        dados1->quantidade = quantidade;
        dados1->entradas = entrada;
        dados1->saidas = saidas;

        dados1->entrada = malloc(sizeof(int *) * quantidade);
        for (int i = 0; i < quantidade; i++) {
            dados1->entrada[i] = malloc(entrada * sizeof(int));
        }

        dados1->targets = malloc(sizeof(int *) * quantidade);
        for (int i = 0; i < quantidade; i++) {
            dados1->targets[i] = malloc(sizeof(int) * saidas);
        }
        return dados1;
    } else {
        return NULL;
    }
}

int ler_dados(const char *nome_arquivo, dados *dados, const char *nome_porta)
{
    FILE *file = fopen(nome_arquivo, "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char linha[100];
    bool isEncontrou = false;

    // Procurar a porta
    while (fgets(linha, sizeof(linha), file) != NULL) {

        linha[strcspn(linha, "\r\n")] = '\0';

        if (strcmp(linha, nome_porta) == 0) {
            isEncontrou = true;
            break;
        }
    }

    if (!isEncontrou) {
        printf("Porta %s nao encontrada.\n", nome_porta);
        return 0;
    }

    // Ler cada amostra
    for (int i = 0; i < dados->quantidade; i++) {

        if (fgets(linha, sizeof(linha), file) == NULL) {
            printf("Fim inesperado do arquivo.\n");
            return 0;
        }

        int valores[10];
        int quantidade_valores = 0;

        char *token = strtok(linha, " \t\r\n");

        while (token != NULL) {

            valores[quantidade_valores] = atoi(token);
            quantidade_valores++;

            token = strtok(NULL, " \t\r\n");
        }

        // Verificar quantidade de valores
        int esperado = dados->entradas + dados->saidas;

        if (quantidade_valores != esperado) {
            printf("Quantidade de valores incorreta na linha %d.\n", i);
            printf("Esperado: %d | Encontrado: %d\n",
                   esperado, quantidade_valores);

            fclose(file);
            return 0;
        }

        // Separar entradas
        for (int j = 0; j < dados->entradas; j++) {
            dados->entrada[i][j] = valores[j];
        }

        // Separar targets
        for (int j = 0; j < dados->saidas; j++) {
            dados->targets[i][j] =
                valores[dados->entradas + j];
        }
    }

    fclose(file);

    printf("Arquivo lido.\n");
    printf("Porta %s encontrada.\n", nome_porta);

    return 1;
}

int ler_dados_letras(const char *nome_arquivo, dados *dados) {

    FILE *file = fopen(nome_arquivo, "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo de letras.\n");
        return 0;
    }

    printf("Arquivo de letras lido.\n");

    for (int i = 0; i < dados->quantidade; i++) {


        for (int j = 0; j < dados->entradas; j++) {

            if (fscanf(file, "%d", &dados->entrada[i][j]) != 1) {

                printf("Erro ao ler os dados da letra.\n");
                printf("Amostra: %d\n", i);
                printf("Entrada: %d\n", j);

                fclose(file);
                return 0;
            }

            if (dados->entrada[i][j] != -1 &&
                dados->entrada[i][j] != 1) {

                printf("Valor invalido: %d\n",
                       dados->entrada[i][j]);

                fclose(file);
                return 0;
                }
        }

        for (int j = 0; j < dados->saidas; j++) {
            dados->targets[i][j] = -1;
        }

        dados->targets[i][i] = 1;
    }

    fclose(file);

    printf("Todas as 26 letras foram carregadas.\n");

    return 1;
}


void liberar_dados(dados *dados) {
    if (dados != NULL) {
        for (int i = 0; i < dados->quantidade; i++) {
            free(dados->entrada[i]);
        }
        free(dados->entrada);

        for (int i = 0; i < dados->quantidade; i++) {
            free(dados->targets[i]);
        }
        free(dados->targets);
        free(dados);
    }
}

/*
 * TODO Agora vamos desenvolver as
 * funções de processo e cruzamento
 * de dados das matrizes.
 */

double erro(mlp *rede, int *target) {
    double erro = 0.0;
    for (int i  = 0; i < rede->saidas; i++) {
        double diferenca = target[i] - rede->saida[i];
        erro += 0.5 * (diferenca * diferenca);
    }

    return erro;
}



void forward(mlp *rede, int *entrada) {

    //Calculo da camada oculta
    for (int i = 0; i < rede->ocultos; i++) {
        double soma = rede->bias_oculta[i];

        for (int j = 0; j < rede->entradas; j++) {
            soma += entrada[j] * rede->peso_entrada_oculta[j][i];
        }

        rede->saida_oculta[i] = tanh(soma);
    }

    //calculo da camada de saida
    for (int i = 0; i < rede->saidas; i++) {
        double soma = rede->bias_saida[i];
        for (int j = 0; j < rede->ocultos; j++) {
            soma += rede->saida_oculta[j] * rede->pesos_ocultos_saida[j][i];
        }
        rede->saida[i] = tanh(soma);
    }
}

void calcular_delta_saida(mlp *rede, const int *target) {
    for (int i = 0; i < rede->saidas; i++) {
        double saida = rede->saida[i];
        rede->delta_saida[i] = (target[i] - saida) * (1.0 - saida * saida);
    }
}

void calcular_delta_oculto(mlp *rede) {
    for (int i = 0; i < rede->ocultos; i++) {
        double soma = 0.0;
        for (int j = 0; j < rede->saidas; j++) {
            soma += rede->delta_saida[j] * rede->pesos_ocultos_saida[i][j];
        }
        rede->delta_oculta[i] =
            (1.0 - rede->saida_oculta[i] * rede->saida_oculta[i]) * soma;
    }
}

void atualizar_pesos_saida(mlp *rede) {
    for (int i = 0; i < rede->ocultos; i++) {
        for (int j = 0; j < rede->saidas; j++) {
            rede->pesos_ocultos_saida[i][j] +=
                rede->taxa_aprendizado *
                    rede->delta_saida[j] *
                        rede->saida_oculta[i];
        }
    }

    for (int i = 0; i < rede->saidas; i++) {
        rede->bias_saida[i] +=
            rede->taxa_aprendizado * rede->delta_saida[i];
    }
}

void atualizar_pesos_ocultos (mlp *rede, int *entrada) {
    for (int i = 0; i < rede->entradas; i++) {
        for (int j = 0; j < rede->ocultos; j++) {
            rede->peso_entrada_oculta[i][j] +=
                rede->taxa_aprendizado * rede->delta_oculta[j] * entrada[i];
        }
    }

    for (int j = 0; j < rede->ocultos; j++) {
        rede->bias_oculta[j] += rede->taxa_aprendizado * rede->delta_oculta[j];
    }
}

void treinar_mlp (mlp *rede, dados *dados, int epocas) {
    for (int epoca = 0; epoca < epocas; epoca++) {
        double erro_total = 0.0;
        for (int i = 0; i < dados->quantidade; i++) {
            forward(rede, dados->entrada[i]);
            erro_total += erro(rede, dados->targets[i]);
            calcular_delta_saida(rede, dados->targets[i]);
            calcular_delta_oculto(rede);
            atualizar_pesos_saida(rede);
            atualizar_pesos_ocultos(rede, dados->entrada[i]);
        }
        if (erro_total < 0.001) {
            printf("Treinamento concluido!\n");
            break;
        }
    }
}

void testar_mlp(mlp *rede, dados *dados) {
    int acerto = 0;
    for (int i = 0; i < dados->quantidade; i++) {
        forward(rede, dados->entrada[i]);
        printf("\nEntrada:\n");
        for (int j = 0; j < dados->entradas; j++) {
            printf("%i", dados->entrada[i][j]);
        }

        printf("\nSaida:\n");
        for (int j = 0; j < dados->saidas; j++) {
            printf("%.6lf\n", rede->saida[j]);
        }

        printf("\nTargets ");
        for (int j = 0; j < dados->saidas; j++) {
            printf("%i\n",dados->targets[i][j]);
        }
        bool isAcerto = true;
        for (int j = 0; j < dados->saidas; j++) {
            int resultado;
            if (rede->saida[j] >= 0) {
                resultado = 1;
            } else {
                resultado = -1;
            }
            if (resultado != dados->targets[i][j]) {
                isAcerto = false;
            }
        }

        if (isAcerto) {
            printf("Resultado: ACERTO\n");
            acerto++;
        } else {
            printf("Resultado: ERRO\n");
        }

        printf("\n");
        printf("Acertos: %d/%d\n", acerto, dados->quantidade);

        const double taxa = (double) acerto / dados->quantidade * 100.0;
        printf("Taxa de acerto: %.2f%%",
               taxa);
    }
}