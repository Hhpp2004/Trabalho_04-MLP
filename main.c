#include <ctype.h>

#include "Multi_Percepton.h"

int main(void) {

int opcao;

do {
    printf("\nMLP - MENU PRINCIPAL\n");
    printf("1 - Portas Logicas\n");
    printf("2 - Reconhecimento de Letras\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {

        int opcao_porta;
        const char *nome_porta = NULL;

        printf("PORTAS LOGICAS\n");
        printf("1 - AND\n");
        printf("2 - NOT\n");
        printf("3 - OR\n");
        printf("4 - XOR\n");
        printf("5 - XNOR\n");
        printf("6 - NAND\n");
        printf("7 - NOR\n");
        printf("0 - Voltar\n");
        printf("Escolha uma porta: ");
        scanf("%d", &opcao_porta);

        switch (opcao_porta) {

            case 1:
                nome_porta = "AND";
                break;

            case 2:
                nome_porta = "NOT";
                break;

            case 3:
                nome_porta = "OR";
                break;

            case 4:
                nome_porta = "XOR";
                break;

            case 5:
                nome_porta = "XNOR";
                break;

            case 6:
                nome_porta = "NAND";
                break;

            case 7:
                nome_porta = "NOR";
                break;

            case 0:
                continue;

            default:
                printf("\nOpcao invalida!\n");
                continue;
        }

        // NOT possui somente uma entrada
        int entradas;

        if (opcao_porta == 2) {
            entradas = 1;
        } else {
            entradas = 2;
        }

        int amostras;

        if (opcao_porta == 2) {
            amostras = 2;
        } else {
            amostras = 4;
        }

        int saidas = 1;
        int ocultos = 2;

        printf("PORTA ESCOLHIDA\n");
        printf("Porta: %s\n", nome_porta);
        printf("Entradas: %d\n", entradas);
        printf("Amostras: %d\n", amostras);

        dados *dados1 = criar_dados(
            amostras,
            entradas,
            saidas
        );

        if (dados1 == NULL) {
            printf("Erro ao criar os dados.\n");
            continue;
        }

        if (!ler_dados("portas.txt", dados1, nome_porta)) {

            printf("Erro ao ler os dados da porta.\n");

            liberar_dados(dados1);

            continue;
        }

        printf("Porta %s encontrada.\n", nome_porta);


        mlp *rede = criar_mlp(
            entradas,
            ocultos,
            saidas
        );

        if (rede == NULL) {

            printf("Erro ao criar a rede.\n");

            liberar_dados(dados1);

            continue;
        }

        inicializar_pesos(rede);

        printf("REDE CRIADA\n");
        printf("Entradas: %d\n", rede->entradas);
        printf("Ocultos: %d\n", rede->ocultos);
        printf("Saidas: %d\n", rede->saidas);
        printf("TREINAMENTO\n");

        treinar_mlp(
            rede,
            dados1,
            10000
        );
        printf("TESTE\n");

        testar_mlp(
            rede,
            dados1
        );

        liberar_mlp(rede);
        liberar_dados(dados1);
    }

    else if (opcao == 2) {

        char letra;

        printf("RECONHECIMENTO DE LETRAS\n");
        printf("Digite uma letra (A-Z): ");
        scanf(" %c", &letra);

        // Converte minuscula para maiuscula
        letra = (char) toupper(letra);

        // Verifica se e uma letra valida
        if (letra < 'A' || letra > 'Z') {

            printf("\nLetra invalida!\n");

            continue;
        }

        printf("\nLetra escolhida: %c\n", letra);


        int entradas = 64;
        int ocultos = 45;
        int saidas = 26;
        int amostras = 26;

        printf("CONFIGURACAO\n");
        printf("Letra escolhida: %c\n", letra);
        printf("Entradas: %d\n", entradas);
        printf("Ocultos: %d\n", ocultos);
        printf("Saidas: %d\n", saidas);
        printf("Amostras: %d\n", amostras);

        dados *dados1 = criar_dados(
            amostras,
            entradas,
            saidas
        );

        if (dados1 == NULL) {

            printf("Erro ao criar os dados.\n");

            continue;
        }

        if (!ler_dados_letras("letras.txt", dados1)) {

            printf("Erro ao ler os dados das letras.\n");

            liberar_dados(dados1);

            continue;
        }

        printf("Dados das letras carregados.\n");

        mlp *rede = criar_mlp(
            entradas,
            ocultos,
            saidas
        );

        if (rede == NULL) {

            printf("Erro ao criar a rede.\n");

            liberar_dados(dados1);

            continue;
        }

        inicializar_pesos(rede);

        printf("REDE CRIADA\n");
        printf("Entradas: %d\n", rede->entradas);
        printf("Ocultos: %d\n", rede->ocultos);
        printf("Saidas: %d\n", rede->saidas);
        printf("TREINAMENTO\n");

        treinar_mlp(
            rede,
            dados1,
            10000
        );
        printf("TESTE\n");

        testar_mlp(
            rede,
            dados1
        );

        liberar_mlp(rede);
        liberar_dados(dados1);
    }

    else if (opcao != 0) {

        printf("\nOpcao invalida!\n");
    }

} while (opcao != 0);
printf("PROGRAMA ENCERRADO\n");

return 0;
}
