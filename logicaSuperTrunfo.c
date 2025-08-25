#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

// Função para exibir o resultado da comparação de forma clara
// Parâmetros:
// - atributo: Nome do atributo sendo comparado (ex: "População")
// - valor1, valor2: Valores do atributo para as cartas 1 e 2
// - menor_vence: Flag (1 para sim, 0 para não) que inverte a regra (menor valor ganha)
// - c1, c2: As duas cartas sendo comparadas, para podermos exibir seus nomes
void comparar_cartas(const char* atributo, double valor1, double valor2, int menor_vence, Carta c1, Carta c2) {
    printf("\n--- RESULTADO DA COMPARAÇÃO ---\n");
    printf("Atributo Escolhido: %s\n", atributo);
    printf("----------------------------------\n");
    printf("Carta 1 (%s): %.2f\n", c1.NomeCidade, valor1);
    printf("Carta 2 (%s): %.2f\n", c2.NomeCidade, valor2);
    printf("----------------------------------\n");

    int carta1_venceu = 0;
    int empate = 0;

    if (menor_vence) {
        // Regra invertida: menor valor vence
        if (valor1 < valor2) {
            carta1_venceu = 1;
        } else if (valor1 == valor2) {
            empate = 1;
        }
    } else {
        // Regra padrão: maior valor vence
        if (valor1 > valor2) {
            carta1_venceu = 1;
        } else if (valor1 == valor2) {
            empate = 1;
        }
    }

    if (empate) {
        printf("Resultado: Empate!\n");
    } else if (carta1_venceu) {
        printf("Resultado: Carta 1 (%s) venceu!\n", c1.NomeCidade);
    } else {
        printf("Resultado: Carta 2 (%s) venceu!\n", c2.NomeCidade);
    }
    printf("--- FIM DA COMPARAÇÃO ---\n");
}


int main() {
    int QuantidadeCartas;
    char buffer[100];

    printf("Iniciando Cadastro Super Trunfo\n\n");

    printf("> Vai ser solicitado o cadastro de 2(duas) cartas! \n");
    QuantidadeCartas = 2;

    Carta *Baralho = malloc(QuantidadeCartas * sizeof(Carta));
    if (!Baralho) {
        printf(">> Erro ao alocar memória.\n");
        return 1;
    }
    printf(">> Memória alocada com sucesso.\n");

    int used[8][4] = {0};
    srand((unsigned)time(NULL));

    printf("> Iniciando cadastro das cartas.\n");

    for (int i = 0; i < QuantidadeCartas; i++) {
        printf("\n>> Cadastro da carta [%d]\n", i + 1);

        while (1) {
            printf(">>> Informe o Estado (A-H): ");
            fgets(buffer, sizeof(buffer), stdin);
            if (strlen(buffer) < 2) continue; // Garante que algo além de '\n' foi digitado
            Baralho[i].Estado = (char)toupper((unsigned char)buffer[0]);
            if (Baralho[i].Estado >= 'A' && Baralho[i].Estado <= 'H') break;
            printf(">> Estado inválido! Use A..H.\n");
        }

        int e = Baralho[i].Estado - 'A';
        int livres = 0;
        for (int k = 0; k < 4; k++) if (!used[e][k]) livres++;
        if (livres == 0) {
            printf(">> Para o estado %c ja existem 4 cartas (01..04). Escolha outro estado.\n", Baralho[i].Estado);
            i--;
            continue;
        }

        int num;
        do { num = (rand() % 4) + 1; } while (used[e][num - 1]);
        used[e][num - 1] = 1;

        snprintf(Baralho[i].CodigoCarta, sizeof(Baralho[i].CodigoCarta), "%c%02d", Baralho[i].Estado, num);

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

        printf(">>> Calculando densidade populacional!\n");
        if (Baralho[i].Area > 0.0f) {
            Baralho[i].DensidadePopulacional = (float)Baralho[i].Populacao / Baralho[i].Area;
        } else {
            Baralho[i].DensidadePopulacional = 0.0f;
            printf(">>> Atenção: área zero. Densidade definida como 0.0.\n");
        }

        printf(">>> Calculando PIB per capita!\n");
        if (Baralho[i].Populacao > 0) {
            Baralho[i].PIBPerCapita = (float)((Baralho[i].PIB * 1e9) / (double)Baralho[i].Populacao);
        } else {
            Baralho[i].PIBPerCapita = 0.0f;
            printf(">>> Atenção: população zero. PIB per capita definido como 0.0.\n");
        }

        // O cálculo do Super Poder não é usado na comparação, mas mantemos por consistência
        double termo_inverso_densidade = 0.0;
        if (Baralho[i].DensidadePopulacional > 0.0f) {
            termo_inverso_densidade = 1.0 / (double)Baralho[i].DensidadePopulacional;
        }
        Baralho[i].SuperPoder = (double)Baralho[i].Populacao + (double)Baralho[i].Area + (double)Baralho[i].PIB +
                              (double)Baralho[i].NumeroPontosTuristicos + (double)Baralho[i].PIBPerCapita +
                              termo_inverso_densidade;
    }

    printf("\n> Finalizado cadastro das cartas.\n");
    printf("> Exibindo cadastro das cartas.\n\n");

    for (int i = 0; i < QuantidadeCartas; i++) {
        printf(">> ---------------------------------------------------------------------------------\n");
        printf(">> CARTA                       : %d (%s)\n", i + 1, Baralho[i].NomeCidade);
        printf(">> CÓDIGO DA CARTA             : %s\n", Baralho[i].CodigoCarta);
        printf(">> POPULAÇÃO                   : %lu\n", Baralho[i].Populacao);          
        printf(">> ÁREA                        : %.2f km²\n", Baralho[i].Area);
        printf(">> PIB                         : %.2f bilhões de reais\n", Baralho[i].PIB);
        printf(">> NÚMERO DE PONTOS TURISTICOS : %d\n", Baralho[i].NumeroPontosTuristicos);
        printf(">> DENSIDADE POPULACIONAL      : %.2f hab/km²\n", Baralho[i].DensidadePopulacional);
        printf(">> PIB PER CAPITA              : %.2f reais\n", Baralho[i].PIBPerCapita);
        printf(">> ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
    }

    // --- INÍCIO DA SEÇÃO DE COMPARAÇÃO INTERATIVA ---
    
    printf("\n\n====================== HORA DO DUELO! ======================\n");
    printf("Escolha o atributo para comparar as cartas %s e %s:\n", Baralho[0].NomeCidade, Baralho[1].NomeCidade);
    printf("1. População\n");
    printf("2. Área\n");
    printf("3. PIB\n");
    printf("4. Número de Pontos Turísticos\n");
    printf("5. Densidade Populacional (MENOR vence)\n");
    printf("6. PIB per Capita\n");
    printf("============================================================\n");
    printf("Sua escolha: ");

    int escolha;
    fgets(buffer, sizeof(buffer), stdin);
    escolha = atoi(buffer);

    switch (escolha) {
        case 1:
            comparar_cartas("População", (double)Baralho[0].Populacao, (double)Baralho[1].Populacao, 0, Baralho[0], Baralho[1]);
            break;
        case 2:
            comparar_cartas("Área", Baralho[0].Area, Baralho[1].Area, 0, Baralho[0], Baralho[1]);
            break;
        case 3:
            comparar_cartas("PIB", Baralho[0].PIB, Baralho[1].PIB, 0, Baralho[0], Baralho[1]);
            break;
        case 4:
            comparar_cartas("Número de Pontos Turísticos", (double)Baralho[0].NumeroPontosTuristicos, (double)Baralho[1].NumeroPontosTuristicos, 0, Baralho[0], Baralho[1]);
            break;
        case 5:
            // Regra especial: menor vence
            comparar_cartas("Densidade Populacional", Baralho[0].DensidadePopulacional, Baralho[1].DensidadePopulacional, 1, Baralho[0], Baralho[1]);
            break;
        case 6:
            comparar_cartas("PIB per Capita", Baralho[0].PIBPerCapita, Baralho[1].PIBPerCapita, 0, Baralho[0], Baralho[1]);
            break;
        default:
            printf("\n>> Opção inválida! Nenhuma comparação foi realizada.\n");
            break;
    }

    printf("\nFinalizando o programa.\n");

    free(Baralho);
    return 0;
}