#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numeroConta;
    char nome[50];
    float saldo;
    int ativo;
} Cliente;

void exibirMenu() {
    printf("\n--- Sistema de Manutencao de Contas ---\n");
    printf("1. Cadastrar um novo cliente\n");
    printf("2. Consultar um cliente pelo numero da conta\n");
    printf("3. Atualizar o saldo de um cliente (Deposito/Saque)\n");
    printf("4. Encerrar conta (remover cliente)\n");
    printf("5. Listar todos os clientes\n");
    printf("6. Restaurar leitura com rewind() e listar novamente\n");
    printf("7. Encerrar\n");
    printf("Escolha uma opcao: ");
}

int main() {
    FILE *arquivo;
    Cliente cliente;
    int opcao, numeroConta, opcaoSaldo;
    float valor;

    arquivo = fopen("contas.dat", "rb+");
    
    if (arquivo == NULL) {
        arquivo = fopen("contas.dat", "wb+");
        if (arquivo == NULL) {
            printf("Erro ao criar o arquivo.\n");
            return 1;
        }
    }

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite o numero da conta (1 a 100): ");
                scanf("%d", &numeroConta);
                
                if (numeroConta < 1) {
                    printf("Numero de conta invalido.\n");
                    break;
                }

                fseek(arquivo, (numeroConta - 1) * sizeof(Cliente), SEEK_SET);
                fread(&cliente, sizeof(Cliente), 1, arquivo);

                if (cliente.ativo == 1 && cliente.numeroConta == numeroConta) {
                    printf("Ja existe uma conta cadastrada neste numero.\n");
                } else {
                    cliente.numeroConta = numeroConta;
                    printf("Digite o nome do cliente: ");
                    getchar();
                    fgets(cliente.nome, 50, stdin);
                    cliente.nome[strcspn(cliente.nome, "\n")] = 0;
                    printf("Digite o saldo inicial: ");
                    scanf("%f", &cliente.saldo);
                    cliente.ativo = 1;

                    fseek(arquivo, (numeroConta - 1) * sizeof(Cliente), SEEK_SET);
                    fwrite(&cliente, sizeof(Cliente), 1, arquivo);
                    printf("Cliente cadastrado com sucesso!\n");
                }
                break;

            case 2:
                printf("Digite o numero da conta a consultar: ");
                scanf("%d", &numeroConta);

                fseek(arquivo, (numeroConta - 1) * sizeof(Cliente), SEEK_SET);
                fread(&cliente, sizeof(Cliente), 1, arquivo);

                if (cliente.ativo == 1 && cliente.numeroConta == numeroConta) {
                    printf("\nConta: %d\nNome: %s\nSaldo: R$ %.2f\n", cliente.numeroConta, cliente.nome, cliente.saldo);
                } else {
                    printf("\nConta inexistente ou encerrada.\n");
                }
                break;

            case 3:
                printf("Digite o numero da conta para atualizar: ");
                scanf("%d", &numeroConta);

                fseek(arquivo, (numeroConta - 1) * sizeof(Cliente), SEEK_SET);
                fread(&cliente, sizeof(Cliente), 1, arquivo);

                if (cliente.ativo == 1 && cliente.numeroConta == numeroConta) {
                    printf("\nCliente: %s | Saldo atual: R$ %.2f\n", cliente.nome, cliente.saldo);
                    printf("1. Depositar dinheiro\n");
                    printf("2. Sacar (remover) dinheiro\n");
                    printf("Escolha a operacao: ");
                    scanf("%d", &opcaoSaldo);

                    if (opcaoSaldo == 1) {
                        printf("Digite o valor do deposito: ");
                        scanf("%f", &valor);
                        if (valor > 0) {
                            cliente.saldo += valor;
                            
                            fseek(arquivo, (numeroConta - 1) * sizeof(Cliente), SEEK_SET);
                            fwrite(&cliente, sizeof(Cliente), 1, arquivo);
                            printf("Deposito realizado! Novo saldo: R$ %.2f\n", cliente.saldo);
                        } else {
                            printf("Valor invalido para deposito.\n");
                        }
                    } 
                    else if (opcaoSaldo == 2) {
                        printf("Digite o valor do saque: ");
                        scanf("%f", &valor);
                        if (valor > 0) {
                            if (valor <= cliente.saldo) {
                                cliente.saldo -= valor;
                                
                                fseek(arquivo, (numeroConta - 1) * sizeof(Cliente), SEEK_SET);
                                fwrite(&cliente, sizeof(Cliente), 1, arquivo);
                                printf("Saque realizado com sucesso! Novo saldo: R$ %.2f\n", cliente.saldo);
                            } else {
                                printf("Erro: Saldo insuficiente para realizar este saque.\n");
                            }
                        } else {
                            printf("Valor invalido para saque.\n");
                        }
                    } 
                    else {
                        printf("Opcao de operacao invalida.\n");
                    }
                } else {
                    printf("Conta inexistente ou encerrada.\n");
                }
                break;

            case 4:
                printf("Digite o numero da conta a ser encerrada: ");
                scanf("%d", &numeroConta);

                fseek(arquivo, (numeroConta - 1) * sizeof(Cliente), SEEK_SET);
                fread(&cliente, sizeof(Cliente), 1, arquivo);

                if (cliente.ativo == 1 && cliente.numeroConta == numeroConta) {
                    cliente.ativo = 0;
                    cliente.saldo = 0.0;
                    
                    fseek(arquivo, (numeroConta - 1) * sizeof(Cliente), SEEK_SET);
                    fwrite(&cliente, sizeof(Cliente), 1, arquivo);
                    printf("Conta encerrada com sucesso.\n");
                } else {
                    printf("Conta inexistente.\n");
                }
                break;

            case 5:
                printf("\n--- Lista de Clientes ---\n");
                fseek(arquivo, 0, SEEK_SET);
                
                while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
                    if (cliente.ativo == 1) {
                        printf("Conta: %d | Nome: %-20s | Saldo: R$ %.2f\n", cliente.numeroConta, cliente.nome, cliente.saldo);
                    }
                }
                break;

            case 6:
                printf("\n--- Restaurando ponteiro com rewind() ---\n");
                rewind(arquivo);
                printf("Lendo novamente do inicio:\n");
                
                while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
                    if (cliente.ativo == 1) {
                        printf("Conta: %d | Nome: %-20s | Saldo: R$ %.2f\n", cliente.numeroConta, cliente.nome, cliente.saldo);
                    }
                }
                break;

            case 7:
                printf("Encerrando o sistema...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 7);

    fclose(arquivo);
    return 0;
}