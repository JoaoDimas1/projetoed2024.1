#include "dimas_veiculos.h"

Cliente clientes[MAX_CLIENTES];
Carro carros[MAX_CARROS];
CarroUsado carros_usados[MAX_CARROS_USADOS];
int num_clientes = 0;
int num_carros = 0;
int num_carros_usados = 0;
int cliente_logado = -1;
int admin_autenticado = 0;


void carregar_clientes() {
    FILE *file = fopen(CLIENTES_FILE, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }
    while (fscanf(file, "%80s %80s %d", clientes[num_clientes].email, clientes[num_clientes].senha, &clientes[num_clientes].id) == 3) {
        num_clientes++;
    }
    fclose(file);
}

void salvar_clientes() {
    FILE *file = fopen(CLIENTES_FILE, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo de clientes para escrita.\n");
        return;
    }
    for (int i = 0; i < num_clientes; ++i) {
        fprintf(file, "%s %s %d\n", clientes[i].email, clientes[i].senha, clientes[i].id);
    }
    fclose(file);
}

void carregar_carros() {
    FILE *file = fopen(CARROS_FILE, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de carros.\n");
        return;
    }
    while (fscanf(file, "%49s %49s %d %d %d %f", carros[num_carros].modelo, carros[num_carros].tipo, 
                  &carros[num_carros].ano, &carros[num_carros].alugado, &carros[num_carros].dias_aluguel, 
                  &carros[num_carros].valor) == 6) {
        num_carros++;
    }
    fclose(file);
}


void salvar_carros() {
    FILE *file = fopen(CARROS_FILE, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo de carros para escrita.\n");
        return;
    }
    for (int i = 0; i < num_carros; ++i) {
        fprintf(file, "%s %s %d %d %d %.2f\n", carros[i].modelo, carros[i].tipo, carros[i].ano, 
                carros[i].alugado, carros[i].dias_aluguel, carros[i].valor);
    }
    fclose(file);
}


void carregar_carros_usados() {
    FILE *file = fopen(CARROS_USADOS_FILE, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de carros usados.\n");
        return;
    }
    while (fscanf(file, "%49s %49s %d %d %f %f", carros_usados[num_carros_usados].modelo, 
                  carros_usados[num_carros_usados].tipo, &carros_usados[num_carros_usados].ano, 
                  &carros_usados[num_carros_usados].quilometragem, &carros_usados[num_carros_usados].valor_compra, 
                  &carros_usados[num_carros_usados].valor_aluguel) == 6) {
        num_carros_usados++;
    }
    fclose(file);
}

void salvar_carros_usados() {
    FILE *file = fopen(CARROS_USADOS_FILE, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo de carros usados para escrita.\n");
        return;
    }
    for (int i = 0; i < num_carros_usados; ++i) {
        fprintf(file, "%s %s %d %d %.2f %.2f\n", carros_usados[i].modelo, carros_usados[i].tipo, 
                carros_usados[i].ano, carros_usados[i].quilometragem, carros_usados[i].valor_compra, 
                carros_usados[i].valor_aluguel);
    }
    fclose(file);
}
void cadastrar_cliente() {
    if (num_clientes >= MAX_CLIENTES) {
        printf("Limite de clientes alcançado.\n");
        return;
    }
    Cliente c;
    printf("E-mail: ");
    fgets(c.email, MAX_EMAIL, stdin);
    c.email[strcspn(c.email, "\n")] = '\0'; 

    if (strchr(c.email, '@') == NULL) {
        printf("Erro: O e-mail deve conter o caractere '@'.\n");
        return;
    }

    if (strcmp(c.email, ADMIN_EMAIL) == 0) {
        printf("Erro: O e-mail do administrador não pode ser usado para outros clientes.\n");
        return;
    }

    for (int i = 0; i < num_clientes; ++i) {
        if (strcmp(clientes[i].email, c.email) == 0) {
            printf("Erro: O e-mail já está em uso.\n");
            return;
        }
    }

    printf("Senha: ");
    fgets(c.senha, MAX_SENHA, stdin);
    c.senha[strcspn(c.senha, "\n")] = '\0';

    if (strlen(c.email) > 80) {
        printf("Erro: O e-mail não pode exceder 80 caracteres.\n");
        return;
    }
    if (strlen(c.senha) > 80) {
        printf("Erro: A senha não pode exceder 80 caracteres.\n");
        return;
    }

    c.id = num_clientes + 1;
    clientes[num_clientes++] = c;
    printf("Cliente cadastrado com sucesso!\n");
}

void editar_cliente() {
    if (cliente_logado == -1) {
        printf("Nenhum cliente logado.\n");
        return;
    }
    Cliente *c = &clientes[cliente_logado];
    printf("Novo E-mail (deixe em branco para manter): ");
    char buffer[MAX_EMAIL];
    fgets(buffer, MAX_EMAIL, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strchr(buffer, '@') == NULL) {
            printf("Erro: O e-mail deve conter o caractere '@'.\n");
            return;
        }
        if (strcmp(buffer, ADMIN_EMAIL) == 0) {
            printf("Erro: O e-mail do administrador não pode ser usado para outros clientes.\n");
            return;
        }
        for (int i = 0; i < num_clientes; ++i) {
            if (strcmp(clientes[i].email, buffer) == 0) {
                printf("Erro: O e-mail já está em uso.\n");
                return;
            }
        }
        strcpy(c->email, buffer);
    }
    printf("Nova Senha (deixe em branco para manter): ");
    fgets(buffer, MAX_SENHA, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strcpy(c->senha, buffer);
    }
    printf("Cliente editado com sucesso!\n");
}
int comparar_clientes(const void *a, const void *b) {
    return strcmp(((Cliente *)a)->email, ((Cliente *)b)->email);
}

void ordenar_clientes() {
    quicksort(clientes, 0, num_clientes - 1);
}

int buscar_cliente_binario(const char *email) {
    int esquerda = 0;
    int direita = num_clientes - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int cmp = strcmp(clientes[meio].email, email);

        if (cmp == 0) {
            return meio; 
        } else if (cmp < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return -1; 
}

void fazer_login() {
    if (num_clientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    ordenar_clientes();

    char email[MAX_EMAIL];
    char senha[MAX_SENHA];
    printf("E-mail: ");
    fgets(email, MAX_EMAIL, stdin);
    email[strcspn(email, "\n")] = '\0';
    printf("Senha: ");
    fgets(senha, MAX_SENHA, stdin);
    senha[strcspn(senha, "\n")] = '\0';

    if (strchr(email, '@') == NULL) {
        printf("Erro: O e-mail deve conter o caractere '@'.\n");
        return;
    }
    if (strlen(email) > 80) {
        printf("Erro: O e-mail não pode exceder 80 caracteres.\n");
        return;
    }
    if (strlen(senha) > 80) {
        printf("Erro: A senha não pode exceder 80 caracteres.\n");
        return;
    }

    int index = buscar_cliente_binario(email);

    if (index != -1 && strcmp(clientes[index].senha, senha) == 0) {
        cliente_logado = index;
        printf("Login bem-sucedido!\n");
    } else {
        printf("E-mail ou senha incorretos.\n");
    }
}


void excluir_cliente() {
    if (cliente_logado == -1) {
        printf("Nenhum cliente logado.\n");
        return;
    }
    for (int i = cliente_logado; i < num_clientes - 1; ++i) {
        clientes[i] = clientes[i + 1];
    }
    num_clientes--;
    cliente_logado = -1;
    printf("Cliente excluído com sucesso!\n");
}

void mostrar_carros() {
    for (int i = 0; i < num_carros; ++i) {
        printf("Modelo: %s, Tipo: %s, Ano: %d, Alugado: %d, Dias de aluguel: %d\n",
               carros[i].modelo, carros[i].tipo, carros[i].ano, carros[i].alugado, carros[i].dias_aluguel);
    }
}

void mostrar_carros_disponiveis() {
    printf("Carros novos disponíveis:\n");
    for (int i = 0; i < num_carros; ++i) {
        if (!carros[i].alugado) {
            printf("Modelo: %s, Tipo: %s, Ano: %d, Valor de Compra: R$ %.2f\n", 
                   carros[i].modelo, carros[i].tipo, carros[i].ano, carros[i].valor);
        }
    }

    printf("Carros usados disponíveis:\n");
    for (int i = 0; i < num_carros_usados; ++i) {
        printf("Modelo: %s, Tipo: %s, Ano: %d, Quilometragem: %d, Valor de Compra: R$ %.2f, Valor de Aluguel: R$ %.2f\n", 
               carros_usados[i].modelo, carros_usados[i].tipo, carros_usados[i].ano, 
               carros_usados[i].quilometragem, carros_usados[i].valor_compra, 
               carros_usados[i].valor_aluguel);
    }
}


void listar_clientes() {
    for (int i = 0; i < num_clientes; ++i) {
        printf("ID: %d, E-mail: %s\n", clientes[i].id, clientes[i].email);
    }
}

void mostrar_carros_usados() {
    for (int i = 0; i < num_carros_usados; ++i) {
        printf("Modelo: %s, Tipo: %s, Ano: %d, Quilometragem: %d, Valor de Compra: R$ %.2f, Valor de Aluguel: R$ %.2f\n",
               carros_usados[i].modelo, carros_usados[i].tipo, carros_usados[i].ano, carros_usados[i].quilometragem,
               carros_usados[i].valor_compra, carros_usados[i].valor_aluguel);
    }
}


void excluir_carro_usado() {
    char modelo[MAX_STRING];
    printf("Modelo do carro usado a ser excluído: ");
    fgets(modelo, MAX_STRING, stdin);
    modelo[strcspn(modelo, "\n")] = '\0';

    for (int i = 0; i < num_carros_usados; ++i) {
        if (strcmp(carros_usados[i].modelo, modelo) == 0) {
            for (int j = i; j < num_carros_usados - 1; ++j) {
                carros_usados[j] = carros_usados[j + 1];
            }
            num_carros_usados--;
            printf("Carro usado excluído com sucesso!\n");
            return;
        }
    }
    printf("Carro usado não encontrado.\n");
}

void cadastrar_carro_novo() {
    if (num_carros >= MAX_CARROS) {
        printf("Limite de carros novos alcançado.\n");
        return;
    }
    Carro c;
    printf("Modelo: ");
    fgets(c.modelo, MAX_STRING, stdin);
    c.modelo[strcspn(c.modelo, "\n")] = '\0';
    printf("Tipo: ");
    fgets(c.tipo, MAX_STRING, stdin);
    c.tipo[strcspn(c.tipo, "\n")] = '\0';
    printf("Ano: ");
    scanf("%d", &c.ano);
    printf("Valor: ");
    scanf("%f", &c.valor); 
    getchar(); 

    c.alugado = 0;
    c.dias_aluguel = 0;
    carros[num_carros++] = c;
    printf("Carro novo cadastrado com sucesso!\n");

}

void cadastrar_carro_usado() {
    if (num_carros_usados >= MAX_CARROS_USADOS) {
        printf("Limite de carros usados alcançado.\n");
        return;
    }
    CarroUsado c;
    printf("Modelo: ");
    fgets(c.modelo, MAX_STRING, stdin);
    c.modelo[strcspn(c.modelo, "\n")] = '\0';
    printf("Tipo: ");
    fgets(c.tipo, MAX_STRING, stdin);
    c.tipo[strcspn(c.tipo, "\n")] = '\0';
    printf("Ano: ");
    scanf("%d", &c.ano);
    printf("Quilometragem: ");
    scanf("%d", &c.quilometragem);
    printf("Valor de Compra: ");
    scanf("%f", &c.valor_compra); 
    printf("Valor de Aluguel: ");
    scanf("%f", &c.valor_aluguel); 
    getchar(); 

    carros_usados[num_carros_usados++] = c;
    printf("Carro usado cadastrado com sucesso!\n");
}



void alugar_carro() {
    if (cliente_logado == -1) {
        printf("Você precisa estar logado para alugar um carro.\n");
        return;
    }

    printf("Carros usados disponíveis para aluguel:\n");
    mostrar_carros_usados();

    char modelo[MAX_STRING];
    printf("Modelo do carro a ser alugado: ");
    fgets(modelo, MAX_STRING, stdin);
    modelo[strcspn(modelo, "\n")] = '\0';

    int dias_aluguel;
    printf("Quantos dias você vai ficar com o carro? ");
    scanf("%d", &dias_aluguel);
    getchar(); 

    for (int i = 0; i < num_carros_usados; ++i) {
        if (strcmp(carros_usados[i].modelo, modelo) == 0) {

            float valor_total = carros_usados[i].valor_aluguel * dias_aluguel;
            printf("Você está alugando o carro usado %s.\n", modelo);
            printf("Valor diário do carro: R$ %.2f\n", carros_usados[i].valor_aluguel);
            printf("Número de dias: %d\n", dias_aluguel);
            printf("Valor total do aluguel: R$ %.2f\n", valor_total);


            for (int j = i; j < num_carros_usados - 1; ++j) {
                carros_usados[j] = carros_usados[j + 1];
            }
            num_carros_usados--;
            printf("Carro usado alugado com sucesso por %d dias!\n", dias_aluguel);
            return;
        }
    }
    printf("Carro usado não disponível ou não encontrado.\n");
}


void comprar_carro() {
    if (cliente_logado == -1) {
        printf("Você precisa estar logado para comprar um carro.\n");
        return;
    }

    printf("Carros novos disponíveis:\n");
    for (int i = 0; i < num_carros; ++i) {
        printf("Modelo: %s, Tipo: %s, Ano: %d, Valor: R$ %.2f\n", carros[i].modelo, carros[i].tipo, carros[i].ano, carros[i].valor);
    }

    printf("Carros usados disponíveis para compra:\n");
    mostrar_carros_usados();

    char modelo[MAX_STRING];
    char tipo[MAX_STRING];
    printf("Modelo do carro a ser comprado: ");
    fgets(modelo, MAX_STRING, stdin);
    modelo[strcspn(modelo, "\n")] = '\0';

    printf("Tipo do carro a ser comprado (novo/usado): ");
    fgets(tipo, MAX_STRING, stdin);
    tipo[strcspn(tipo, "\n")] = '\0';

    int encontrado = 0;

    if (strcmp(tipo, "novo") == 0) {
        for (int i = 0; i < num_carros; ++i) {
            if (strcmp(carros[i].modelo, modelo) == 0) {
                printf("Você está comprando o carro novo %s.\n", modelo);
                printf("Valor do carro: R$ %.2f\n", carros[i].valor);

                for (int j = i; j < num_carros - 1; ++j) {
                    carros[j] = carros[j + 1];
                }
                num_carros--;
                encontrado = 1;
                printf("Carro novo comprado com sucesso!\n");
                break;
            }
        }
    } else if (strcmp(tipo, "usado") == 0) {
        for (int i = 0; i < num_carros_usados; ++i) {
            if (strcmp(carros_usados[i].modelo, modelo) == 0) {
                printf("Você está comprando o carro usado %s.\n", modelo);
                printf("Valor do carro: R$ %.2f\n", carros_usados[i].valor_compra);

                for (int j = i; j < num_carros_usados - 1; ++j) {
                    carros_usados[j] = carros_usados[j + 1];
                }
                num_carros_usados--;
                encontrado = 1;
                printf("Carro usado comprado com sucesso!\n");
                break;
            }
        }
    } else {
        printf("Tipo de carro inválido. Por favor, escolha 'novo' ou 'usado'.\n");
    }

    if (!encontrado) {
        printf("Carro não encontrado.\n");
    }
}


void menu_carros() {
    int opcao;
    do {
        printf("Menu Carros:\n");
        printf("1. Mostrar Carros Disponíveis\n");
        printf("0. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                mostrar_carros_disponiveis();
                break;
            case 0:
                return;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}


void menu_carros_usados() {
    int opcao;
    do {
        printf("1. Mostrar todos os carros usados\n");
        printf("2. Cadastrar carro usado\n");
        printf("3. Excluir carro usado\n");
        printf("4. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                mostrar_carros_usados();
                break;
            case 2:
                cadastrar_carro_usado();
                break;
            case 3:
                excluir_carro_usado();
                break;
            case 4:

                break;
            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 4);
}

void menu_admin() {
    int opcao;
    do {
        printf("1. Cadastrar carro novo\n");
        printf("2. Menu Carros Usados\n");
        printf("3. Listar clientes\n");
        printf("4. Ordenar clientes\n");
        printf("5. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                cadastrar_carro_novo();
                break;
            case 2:
                menu_carros_usados();
                break;
            case 3:
                listar_clientes();
                break;
            case 4:
                ordenar_clientes();
                printf("Clientes ordenados com sucesso!\n");
                break;
            case 5:
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 5);
}
void menu_suporte() {
    int opcao;
    do {
        printf("Menu Suporte:\n");
        printf("1. Solicitar Conserto de Carro\n");
        printf("2. Informações de Manutenção\n");
        printf("3. Suporte Geral\n");
        printf("4. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                printf("Solicitação de conserto registrada com sucesso!\n");

                break;
            case 2:
                printf("Informações de manutenção:\n");
                printf("1. Troca de óleo: R$ 150,00\n");
                printf("2. Revisão geral: R$ 500,00\n");
                printf("3. Balanceamento e alinhamento: R$ 200,00\n");

                break;
            case 3:
                printf("Suporte Geral:\n");
                printf("1. Atendimento ao cliente: (11) 1234-5678\n");
                printf("2. E-mail de suporte: suporte@dimasveiculos.com\n");

                break;
            case 4:
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 4);
}