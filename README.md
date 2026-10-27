# Trabalho 04 — MLP em C

Implementação acadêmica de uma rede neural **Multilayer Perceptron (MLP)** em C11. O programa possui uma interface de terminal com duas demonstrações:

1. treinamento para portas lógicas;
2. reconhecimento de letras representadas por matrizes de `8 x 8` pixels.

O projeto usa apenas a biblioteca padrão do C e a biblioteca matemática (`libm`), sem frameworks ou dependências externas.

## Funcionalidades

### Portas lógicas

O menu permite selecionar:

- AND
- NOT
- OR
- XOR
- XNOR
- NAND
- NOR

Para cada porta, o programa:

1. cria uma estrutura de dados com as amostras;
2. localiza a porta no arquivo `portas.txt`;
3. cria uma MLP com uma camada oculta;
4. inicializa pesos e vieses aleatoriamente;
5. treina por até `10.000` épocas;
6. testa todas as amostras e exibe as saídas, os alvos, acertos e taxa de acerto.

A porta NOT usa uma entrada e duas amostras. As demais usam duas entradas e quatro amostras.

### Reconhecimento de letras

O usuário informa uma letra de `A` a `Z`. A entrada é convertida para maiúscula e validada. A rede é configurada com:

- **64 entradas**, correspondentes a uma matriz `8 x 8`;
- **45 neurônios ocultos**;
- **26 saídas**, uma para cada letra;
- **26 amostras**, uma por letra.

O arquivo `letras.txt` contém os padrões das letras usando os valores `-1` e `1`. Os alvos são criados internamente como vetores one-hot: o índice da amostra recebe `1` e os demais recebem `-1`.

> Observação: a letra informada é exibida na configuração, mas o código atual carrega e treina as 26 letras completas. Não há uma etapa de classificação exclusiva da letra digitada.

## Como funciona a MLP

A rede possui uma camada de entrada, uma camada oculta e uma camada de saída. As estruturas principais estão declaradas em [`Multi_Percepton.h`](./Multi_Percepton.h):

- `mlp`: dimensões da rede, pesos, vieses, saídas intermediárias, deltas e taxa de aprendizagem;
- `dados`: quantidade de amostras, dimensões das entradas e saídas, entradas e alvos.

### Fluxo de processamento

1. **Inicialização** — pesos e vieses são gerados com `rand()` no intervalo aproximado de `[-0.5, 0.5]`.
2. **Forward pass** — cada neurônio calcula a soma ponderada e aplica `tanh`.
3. **Cálculo do erro** — é usado erro quadrático com fator `1/2`:

   ```text
   erro = 0,5 * (target - saída)^2
   ```

4. **Backpropagation** — os deltas da saída e da camada oculta são calculados usando a derivada de `tanh`.
5. **Atualização** — pesos e vieses são atualizados com taxa de aprendizagem `0.1`.
6. **Parada antecipada** — o treinamento termina antes das `10.000` épocas quando o erro total da época fica abaixo de `0.001`.
7. **Teste** — saídas maiores ou iguais a zero são classificadas como `1`; saídas negativas são classificadas como `-1`.

O treinamento é online: os pesos são atualizados após cada amostra, e não somente ao fim de uma época.

## Estrutura do projeto

```text
.
├── CMakeLists.txt             # Configuração do build
├── main.c                     # Menu e fluxo das duas demonstrações
├── Multi_Percepton.h          # Tipos e protótipos públicos da MLP
├── Multi_Percepton.c          # Alocação, leitura, forward e treinamento
├── portas.txt                 # Tabelas-verdade das portas lógicas
├── letras.txt                 # 26 padrões de letras com 64 valores cada
├── Multilayerperceptron.pdf   # Material de referência do trabalho
├── .idea/                     # Metadados do projeto no CLion
└── cmake-build-debug/         # Artefatos de build locais
```

### `main.c`

Implementa o menu principal e coordena as operações:

- leitura das opções do usuário;
- definição da topologia da rede;
- carregamento dos dados;
- criação e inicialização da MLP;
- treinamento e teste;
- liberação da memória.

### `Multi_Percepton.c`

Implementa as funções declaradas no cabeçalho:

| Grupo | Funções |
|---|---|
| Rede | `criar_mlp`, `inicializar_pesos`, `liberar_mlp` |
| Dados | `criar_dados`, `ler_dados`, `ler_dados_letras`, `liberar_dados` |
| Processamento | `forward`, `erro`, `calcular_delta_saida`, `calcular_delta_oculto` |
| Atualização | `atualizar_pesos_saida`, `atualizar_pesos_ocultos` |
| Treinamento | `treinar_mlp`, `testar_mlp` |

## Formatos dos dados

### `portas.txt`

O arquivo é organizado por blocos. Cada bloco começa com o nome da porta, seguido pelas linhas com entradas e alvo:

```text
AND
-1 -1 -1
-1 1 -1
1 -1 -1
1 1 1
```

Para portas com duas entradas, cada amostra possui três valores: duas entradas e um alvo. Para NOT, cada amostra possui uma entrada e um alvo:

```text
NOT
-1 1
1 -1
```

Os valores `-1` e `1` representam os estados lógicos utilizados pelo treinamento.

### `letras.txt`

Contém `26` padrões consecutivos. Cada padrão possui `64` inteiros, correspondentes aos pixels de uma letra em uma grade de `8 x 8`. Os valores aceitos são somente `-1` e `1`.

O leitor não usa nomes de letras no arquivo; a posição do padrão determina a classe:

- amostra `0` → saída `A`;
- amostra `1` → saída `B`;
- ...
- amostra `25` → saída `Z`.

## Requisitos

- GCC ou outro compilador compatível com C11;
- CMake `4.3` ou superior, conforme `cmake_minimum_required` no [`CMakeLists.txt`](./CMakeLists.txt);
- biblioteca matemática do sistema (`libm`), vinculada com `-lm`.

## Compilação

Na raiz do projeto:

```bash
cmake -S . -B build
cmake --build build
```

O executável será criado como:

```text
build/Trabalho_04
```

Também existe um diretório de build gerado localmente em `cmake-build-debug/`, normalmente utilizado pelo CLion.

## Execução

Execute o programa a partir da raiz do repositório, pois os arquivos `portas.txt` e `letras.txt` são abertos com caminhos relativos:

```bash
./build/Trabalho_04
```

ou, usando o artefato já presente:

```bash
./cmake-build-debug/Trabalho_04
```

O menu é:

```text
MLP - MENU PRINCIPAL
1 - Portas Logicas
2 - Reconhecimento de Letras
0 - Sair
```

Para encerrar, escolha `0`.

## Gerenciamento de memória

As matrizes de pesos, vetores de vieses, deltas, saídas, entradas e alvos são alocados dinamicamente. Ao fim de cada operação, o programa chama:

- `liberar_mlp(rede)`;
- `liberar_dados(dados1)`.

Isso permite reutilizar o menu sem manter as redes anteriores na memória.

## Estado atual e limitações conhecidas

Esta documentação descreve o comportamento encontrado no código atual. Alguns pontos importantes para evolução do projeto:

- a semente de `rand()` não é definida com `srand()`, portanto a sequência aleatória depende do ambiente e da implementação;
- as chamadas de `scanf()` não verificam se a entrada recebida é numérica ou se ocorreu erro de leitura;
- a função `criar_mlp()` não desfaz alocações parciais caso uma alocação intermediária falhe;
- `criar_dados()` possui a mesma limitação para falhas durante a alocação das matrizes;
- o leitor de portas usa um vetor local de tamanho fixo para cada linha;
- o reconhecimento de letras treina todos os padrões independentemente da letra digitada;
- não há testes automatizados, persistência de pesos, validação separada ou métricas além da taxa de acerto exibida no teste;
- o treinamento e os resultados podem variar entre execuções por causa da inicialização aleatória dos pesos.

## Referência de arquivos

- [CMakeLists.txt](./CMakeLists.txt) — build e vínculo com `libm`.
- [main.c](./main.c) — entrada do programa e menu.
- [Multi_Percepton.h](./Multi_Percepton.h) — API interna e estruturas.
- [Multi_Percepton.c](./Multi_Percepton.c) — implementação da rede e leitura dos dados.
- [portas.txt](./portas.txt) — dados das portas lógicas.
- [letras.txt](./letras.txt) — padrões das letras.
- [Multilayerperceptron.pdf](./Multilayerperceptron.pdf) — material de apoio incluído no projeto.
