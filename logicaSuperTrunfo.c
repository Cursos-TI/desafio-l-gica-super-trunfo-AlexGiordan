#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h> // Para usar o tipo bool

// Estrutura para representar uma carta do jogo
typedef struct {
    char Estado;
    char CodigoCarta[4];
    char NomeCidade[30];
    unsigned long int Populacao;
    float Area;
    float PIB;
    int NumeroPontosTuristicos;
    float DensidadePopulacional;
    float PIBPerCapita;
    double SuperPoder;
} Carta;

// Função para limpar o caractere de nova linha de uma string
void limpar_nova_linha(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

// Função auxiliar para obter o valor de um atributo com base na escolha do menu
// Isso evita um bloco switch gigante e repetitivo no main
double get_valor_atributo(Carta c, int escolha) {
    switch (escolha) {
        case 1: return (double)c.Populacao;
        case 2: return c.Area;
        case 3: return c.PIB;
        case 4: return (double)c.NumeroPontosTuristicos;
        case 5: return c.DensidadePopulacional;
        case 6: return c.PIBPerCapita;
        default: return 0.0; // Valor padrão em caso de erro
    }
}

int main() {
    // --- SEÇÃO DE CADASTRO DAS CARTAS (sem alterações do código anterior) ---
    char buffer[100];
    printf("Iniciando Cadastro Super Trunfo\n\n");
    printf("> Vai ser solicitado o cadastro de 2(duas) cartas! \n");
    const int QuantidadeCartas = 2;

    Carta *Baralho = malloc(QuantidadeCartas * sizeof(Carta));
    if (!Baralho) {
        printf(">> Erro ao alocar memória.\n");
        return 1;
    }
    printf(">> Memória alocada com sucesso.\n");
    srand((unsigned)time(NULL));

    for (int i = 0; i < QuantidadeCartas; i++) {
        printf("\n>> Cadastro da carta [%d]\n", i + 1);
        printf(">>> Informe o nome da cidade: ");
        fgets(Baralho[i].NomeCidade, sizeof(Baralho[i].NomeCidade), stdin);
        limpar_nova_linha(Baralho[i].NomeCidade);

        printf(">>> Informe a população em quantidade nominal: ");
        fgets(buffer, sizeof(buffer), stdin);
        Baralho[i].Populacao = strtoul(buffer, NULL, 10);

        printf(">>> Informe a área total em km²: ");
        fgets(buffer, sizeof(buffer), stdin);
        Baralho[i].Area = (float)atof(buffer);

        printf(">>> Informe o PIB em bilhôes de reais: ");
        fgets(buffer, sizeof(buffer), stdin);
        Baralho[i].PIB = (float)atof(buffer);

        printf(">>> Informe o número de pontos turisticos: ");
        fgets(buffer, sizeof(buffer), stdin);
        Baralho[i].NumeroPontosTuristicos = atoi(buffer);

        if (Baralho[i].Area > 0.0f) {
            Baralho[i].DensidadePopulacional = (float)Baralho[i].Populacao / Baralho[i].Area;
        } else {
            Baralho[i].DensidadePopulacional = 0.0f;
        }
        if (Baralho[i].Populacao > 0) {
            Baralho[i].PIBPerCapita = (float)((Baralho[i].PIB * 1e9) / (double)Baralho[i].Populacao);
        } else {
            Baralho[i].PIBPerCapita = 0.0f;
        }
    }
    printf("\n> Finalizado cadastro das cartas.\n\n");

    // --- FIM DA SEÇÃO DE CADASTRO ---


    // --- INÍCIO DA SEÇÃO DE COMPARAÇÃO AVANÇADA ---
    
    printf("\n\n====================== HORA DO DUELO FINAL! ======================\n");
    printf("Você deve escolher dois atributos para a comparação.\n");
    printf("A carta com a MAIOR SOMA dos valores dos atributos vence!\n");
    printf("====================================================================\n\n");

    const char *nomes_atributos[] = {
        "População", "Área", "PIB", 
        "Número de Pontos Turísticos", "Densidade Populacional", "PIB per Capita"
    };
    int escolha1 = 0, escolha2 = 0;

    // --- ESCOLHA DO PRIMEIRO ATRIBUTO ---
    printf("Escolha o PRIMEIRO atributo para comparar:\n");
    for (int i = 0; i < 6; i++) {
        printf("%d. %s %s\n", i + 1, nomes_atributos[i], (i + 1 == 5) ? "(MENOR vence)" : "");
    }
    printf("Sua escolha: ");
    fgets(buffer, sizeof(buffer), stdin);
    escolha1 = atoi(buffer);

    // Validação da primeira escolha
    if (escolha1 < 1 || escolha1 > 6) {
        printf("\n>> Escolha inválida! Programa encerrado.\n");
        free(Baralho);
        return 1;
    }

    // --- ESCOLHA DO SEGUNDO ATRIBUTO (MENU DINÂMICO) ---
    printf("\nEscolha o SEGUNDO atributo (diferente de '%s'):\n", nomes_atributos[escolha1 - 1]);
    for (int i = 0; i < 6; i++) {
        // Apenas exibe as opções que NÃO foram a primeira escolha
        if ((i + 1) != escolha1) {
             printf("%d. %s %s\n", i + 1, nomes_atributos[i], (i + 1 == 5) ? "(MENOR vence)" : "");
        }
    }
    printf("Sua escolha: ");
    fgets(buffer, sizeof(buffer), stdin);
    escolha2 = atoi(buffer);

    // Validação da segunda escolha
    if (escolha2 < 1 || escolha2 > 6 || escolha2 == escolha1) {
        printf("\n>> Escolha inválida ou repetida! Programa encerrado.\n");
        free(Baralho);
        return 1;
    }

    // --- OBTENDO VALORES E NOMES DOS ATRIBUTOS ESCOLHIDOS ---
    const char *nome_attr1 = nomes_atributos[escolha1 - 1];
    const char *nome_attr2 = nomes_atributos[escolha2 - 1];

    double valor_attr1_c1 = get_valor_atributo(Baralho[0], escolha1);
    double valor_attr1_c2 = get_valor_atributo(Baralho[1], escolha1);
    
    double valor_attr2_c1 = get_valor_atributo(Baralho[0], escolha2);
    double valor_attr2_c2 = get_valor_atributo(Baralho[1], escolha2);

    // --- CÁLCULO DAS SOMAS ---
    double soma_carta1 = valor_attr1_c1 + valor_attr2_c1;
    double soma_carta2 = valor_attr1_c2 + valor_attr2_c2;

    // --- EXIBIÇÃO DETALHADA DO RESULTADO ---
    printf("\n\n--- ANÁLISE COMPLETA DA RODADA ---\n");
    printf("Atributos escolhidos: %s e %s\n", nome_attr1, nome_attr2);
    printf("------------------------------------\n");

    // Exibição dos valores individuais para cada carta
    printf("CARTA 1: %s\n", Baralho[0].NomeCidade);
    printf("  > %-28s: %.2f\n", nome_attr1, valor_attr1_c1);
    printf("  > %-28s: %.2f\n", nome_attr2, valor_attr2_c1);
    printf("  > SOMA TOTAL                  : %.2f\n\n", soma_carta1);

    printf("CARTA 2: %s\n", Baralho[1].NomeCidade);
    printf("  > %-28s: %.2f\n", nome_attr1, valor_attr1_c2);
    printf("  > %-28s: %.2f\n", nome_attr2, valor_attr2_c2);
    printf("  > SOMA TOTAL                  : %.2f\n", soma_carta2);
    printf("------------------------------------\n");

    // Lógica para determinar vencedor individual de cada atributo (usando operador ternário)
    // Apenas para fins de exibição, não afeta o resultado final.
    bool c1_vence_attr1;
    if (escolha1 == 5) c1_vence_attr1 = valor_attr1_c1 < valor_attr1_c2; // Densidade (menor vence)
    else c1_vence_attr1 = valor_attr1_c1 > valor_attr1_c2; // Padrão (maior vence)
    
    bool c1_vence_attr2;
    if (escolha2 == 5) c1_vence_attr2 = valor_attr2_c1 < valor_attr2_c2; // Densidade (menor vence)
    else c1_vence_attr2 = valor_attr2_c1 > valor_attr2_c2; // Padrão (maior vence)

    printf("Análise individual:\n");
    printf("  > %s: %s\n", nome_attr1, (valor_attr1_c1 == valor_attr1_c2) ? "Empate" : (c1_vence_attr1 ? Baralho[0].NomeCidade : Baralho[1].NomeCidade));
    printf("  > %s: %s\n", nome_attr2, (valor_attr2_c1 == valor_attr2_c2) ? "Empate" : (c1_vence_attr2 ? Baralho[0].NomeCidade : Baralho[1].NomeCidade));
    printf("------------------------------------\n");

    // --- RESULTADO FINAL BASEADO NA SOMA ---
    printf("RESULTADO FINAL (baseado na soma):\n");
    if (soma_carta1 > soma_carta2) {
        printf("VENCEDOR: Carta 1 - %s!\n", Baralho[0].NomeCidade);
    } else if (soma_carta2 > soma_carta1) {
        printf("VENCEDOR: Carta 2 - %s!\n", Baralho[1].NomeCidade);
    } else {
        printf("RESULTADO: Empate!\n");
    }
    printf("====================================\n");

    printf("\nFinalizando o programa.\n");
    free(Baralho);
    return 0;
}