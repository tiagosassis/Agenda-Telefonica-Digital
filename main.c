#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// ** Estruruas ** -------------------------------------------------------------------------------------------------
typedef struct{ // Para manipular data e hora, podendo assim calcular a idade dos contatos cadastrados
    int segundo, minuto, hora;
    int dia, mes, ano;
}tp_data;

typedef struct{ // Estruta que compoe o endereço de cada indivíduo
    char pais[25], estado[30], cidade[30], bairro[50], cep[20], numero[8], lagradouro[40];
}tp_endereco;

typedef struct{ // Estrutura que compoe o telefone de cada indivíduo
    char tel_fixo[15], tel_celular[15];
}tp_telefone;

typedef struct{ // Estrutura dos dados de cada contato
    char nome[100], cpf[20], sexo;
    int cod_de_registro;
    tp_endereco endereco;
    tp_telefone telefone;
    int dia_nasc, mes_nasc, ano_nasc, idade;

}tp_pessoa;

// ** Funções ** -------------------------------------------------------------------------------------------------

void valida_login(); // valida login, permitindo apenas pessoal altorizado para acessar e manipular a agenda
int menu_principal(); // menu principal da agenda
int sub_menu(); // menu para escolher qual telefone cadastrar
int menu_busca(int); // sub menu para busca e/ou busca e alteração/exclusão do contato
FILE *carregar_arquivo(char*); // cria/abre um arquivo para ser manipulado com o devido tratamento de erro
void verifica_abertura(FILE*); // checa abertura/criação do arquivo, encerra o programa caso haja um ponteiro nulo
long int contador_de_contatos(); // conta a quantidade de contatos cadastrados e retorna para a chamada da função
void add_contato(tp_pessoa[], tp_data); // Cadastra um novo contato
void exibir_agenda(tp_pessoa[]); // Mostra todos os contatos cadastrados
void exibi_contato(tp_pessoa[]); // busca e exibi todas as informações do contato especifico
void busca_contato(tp_pessoa[], tp_data, int); // busca um contato e exibe nome e cod de registro e da opção de alteração ou exclusão
void altera_contato(tp_pessoa*, int, tp_data); // altera um contato de acordo com nome ou registro
void exclui_contato(tp_pessoa*, int); // exclui um contato de acordo com nome ou registro
void lixeira(); // deleta o arquivo Agenda.txt e cria um novo em branco
void defineData(tp_data*, struct tm*, time_t*); // pega data e hora do S.O.

int main()
{   setlocale(LC_ALL, "Portuguese"); // para o sistema identificar caracteres especiais
    system("title Sistema ATD - Agenda Telefonica Digital"); // Coloca um titulo no programa ao rodar ele no console

    struct tm tempo; // variável para manipular data e hora
    time_t tempoSeg; // variável para manipular data e hora
    tp_data data; // variável para manipular data e hora
    defineData(&data, &tempo, &tempoSeg); // função para pegar data e hora atual do sistema operacional e armazenar na variável data

    char op; // opção selecionada no menu principal
    long int t = contador_de_contatos(); // quantidade de contatos cadastrados
    tp_pessoa *p = (tp_pessoa*) malloc(1 * sizeof(tp_pessoa));
    /*
        t é tamanho do vetor, será usado para alocar a memoria com realloc a medida que for necessário
        op guarda a opção que o usuário selecionou no menu
        data armazena data e hora atual do sistema operacional
        *p matriz unidimencional dinamicamente alocada para armazenar contatos
    */

    valida_login(); // para logar no sistema com a senha e usuário escrito no arquivo Admin.txt

    do{
        op = menu_principal(); // menu principal da agenda
        switch(op){
            case 1: // Cadastra um novo contato
                printf("\n------------------------------------------ Sistema ATD - Cadastro de Contato ------------------------------------------\n");

                p = (tp_pessoa*) realloc(p, 1 * sizeof(tp_pessoa)); // ajusta o tamanho do vetor para um contato que será adicionado
                add_contato(p, data);
                break;

            case 2: // Mostra todos os contatos já cadastrados
                printf("\n-------------------------------------------- Sistema ATD - Exibição Geral ---------------------------------------------\n");

                t = contador_de_contatos(); // quantidade de contatos cadastrados
                if(t == 0){
                    printf("Agenda vazia, nenhum contato cadastrado.\n\n");
                    system("pause");
                    printf("\n\n");
                }
                else{
                    p = (tp_pessoa*) realloc(p, t * sizeof(tp_pessoa)); // ajusta o tamanho do vetor para a quantidade de contatos
                    exibir_agenda(p); // Exibe todos os contatos da agenda
                }
                break;

            case 3: // Busca um contato já cadastrado
                printf("\n------------------------------------------------- Sistema ATD - Busca -------------------------------------------------\n");

                t = contador_de_contatos(); // quantidade de contatos cadastrados
                if(t == 0){
                    printf("Agenda vazia, nenhum contato cadastrado.\n\n");
                    system("pause");
                    printf("\n\n");
                }
                else{
                    p = (tp_pessoa*) realloc(p, t * sizeof(tp_pessoa)); // ajusta o tamanho do vetor para a quantidade de contatos
                    busca_contato(p, data, 0); // busca um contato específica passando 0, significa que ele vai buscar, e se encontrar o contato, disponiliza opções para o usuário
                }
                break;

            case 4: // Exibi um contato já cadastrado
                printf("\n------------------------------------------------- Sistema ATD - Busca -------------------------------------------------\n");

                t = contador_de_contatos(); // quantidade de contatos cadastrados
                if(t == 0){
                    printf("Agenda vazia, nenhum contato cadastrado.\n\n");
                    system("pause");
                    printf("\n\n");
                }
                else{
                    p = (tp_pessoa*) realloc(p, t * sizeof(tp_pessoa)); // ajusta o tamanho do vetor para a quantidade de contatos
                    exibi_contato(p);
                }
                break;

            case 5: // Altera um contato já cadastrado
                printf("\n----------------------------------------------- Sistema ATD - Alteração -----------------------------------------------\n");

                t = contador_de_contatos(); // quantidade de contatos cadastrados
                if(t == 0){
                    printf("Agenda vazia, nenhum contato cadastrado.\n\n");
                    system("pause");
                    printf("\n\n");
                }
                else{
                    p = (tp_pessoa*) realloc(p, t * sizeof(tp_pessoa)); // ajusta o tamanho do vetor para a quantidade de contatos
                    busca_contato(p, data, 1); // // busca um contato específica passando 1, significa que ele vai buscar, e se encontrar o contato, vai entrar na opção de alterar
                }
                break;

            case 6: // Exclui um contato específico da agenda
                printf("\n------------------------------------------- Sistema ATD - Busca e Exclusão --------------------------------------------\n");

                t = contador_de_contatos(); // quantidade de contatos cadastrados
                if(t == 0){
                    printf("Agenda vazia, nenhum contato cadastrado.\n\n");
                    system("pause");
                    printf("\n\n");
                }
                else{
                    p = (tp_pessoa*) realloc(p, t * sizeof(tp_pessoa)); // ajusta o tamanho do vetor para a quantidade de contatos
                    busca_contato(p, data, 2); // // busca um contato específica passando 2, significa que ele vai buscar, e se encontrar o contato, vai entrar na opção de excluir
                }
                break;

            case 7: // Exclui a agenda e cria uma nova vazia
                printf("\n----------------------------------------------- Sistema ATD - Exclusão ------------------------------------------------\n");

                lixeira(); // deleta a agenda criando uma nova
                break;

            case 0: // Encerra o programa
                break;

            default: // Qualquer opção invalida digitada pelo usuário
                printf("Opção inválida.\n");
        }

    }while(!(op == 0));

    printf("\n\n\n-----------------------------------------------------------------------------------------------------------------------");
    free(p); // libera a memoria alocada
    return 0;
}

void valida_login(){
    /*
        Função para validar login, tem como base o usuário e senha digitados no arquivo Admin.txt
        Caso senha e/ou usuário não estejam no arquivo o programa encerra, a senha deve ter no máximo 6 dígitos
        Após 5 erros na senha o programa encerra

    */

    FILE *arq_usuario = fopen("Admin.txt", "a+"); // permissão para criar o arquivo caso não existe, útil para evitar erros
    verifica_abertura(arq_usuario); // verifica se o arquivo foi aberto com êxito
    int t, q_tentativa_login = 0;
    rewind(arq_usuario); // permissão a+ abre o arquivo com o ponteiro no final dele, função rewind joga o ponteiro do arquivo para o inicio

    char login[25], senha[10], loginDigitado[25], senhaDigitada[7];

    if(fscanf(arq_usuario, "%s", login) == EOF){
        printf("\n\t**Impossível efetuar o login pois o usuário não foi cadastrado no arquivo Admin.txt**\n\n");
        system("pause");
        exit(1);

    }
    if(fscanf(arq_usuario, "%s", senha) == EOF){
        printf("\n\t**Impossível efetuar o login pois a senha não foi cadastrada no arquivo Admin.txt**\n\n");
        system("pause");
        exit(1);
    }

    //t = strlen(senha); // função está causando bugs na string de senha e login por algum motivo desconhecido
    for(t = 0; senha[t] != '\0'; t++); // ajusta o for que recebe a senha vinda do teclado

    int autorizado = 0; // 0 é false e 1 é true, para autorizar o login
    printf("\n--------------------------------------- Agenda Telefonica Digital - Tela de Login --------------------------------------\n");

    do{
        printf("| Login: ");
        gets(loginDigitado);
        printf("| Senha: ");
        for(int i = 0; i < t; i++){
            senhaDigitada[i] = getch();
        }
        printf("\n"); // espaçamento

        //printf("Login: %s | senha: %s\n", login, senha); printf usado para verificar o conteudo das string e ver se estava tudo certo
        //printf("login digitado:%s e senha digitada:%s\n\n", loginDigitado, senhaDigitada); printf usado para verificar o conteudo das string e ver se estava tudo certo

        if(strcmp(login, loginDigitado) == 0 && strcmp(senha, senhaDigitada) == 0){
            printf("\n\n"); // espaçamento
            autorizado = 1;
        }
        else{
            q_tentativa_login++;
            printf("\nLogin e/ou senha inválido! Após 5 tentativas o programa encerra.\n\n");
            if(q_tentativa_login == 5)
                exit(1);
        }

    }while(!(autorizado == 1));
    printf("\n--------------------------------------------- Sistema ATD - Menu Principal --------------------------------------------\n\n");

}

int menu_principal(){
    // Exibe um menu para o usuário decidir sua ação
    int op;
    printf("| 1 - Adicionar Contato\n"); // Cadastra um novo contato
    printf("| 2 - Exibir Agenda\n"); // Mostra todos os contatos já cadastrados
    printf("| 3 - Busca Contato\n"); // Busca um contato já cadastrado
    printf("| 4 - Exibir Contato\n"); // Exibe um contato já cadastrado
    printf("| 5 - Alterar Contato\n"); // Altera um contato já cadastrado
    printf("| 6 - Excluir Contato\n"); // Exclui um contato específico da agenda
    printf("| 7 - Apagar Agenda\n"); // Apaga todos os contatos na agenda
    printf("| 0 - Sair\n"); // Encerra o programa
    printf("| -> ");
    scanf("%d", &op);
    fflush(stdin); // limpar o buffer
    return op;
}

int sub_menu(){ // menu para o usuário decidir qual telefone cadastrar
    int op;
    printf("Deseja cadastrar qual telefone:\n");
    printf("1 - Celular\n2 - Fixo\n3 - Celular e Fixo\n-> ");
    scanf("%d", &op);
    fflush(stdin); // limpar o buffer
    return op;
}

int menu_busca(int op){
    /*
        Recebe um inteiro como parâmetro, caso esse inteiro seja 1, o menu primário de escolha de opções
        para o contato é mostrado, caso contrário, o menu secundário, que especifíca o tipo de busca é mostrado
    */
    if(op == 1){
        printf("1 - Alterar contato\n");
        printf("2 - Excluir Contato\n");
        printf("0 - Voltar ao menu principal\n");
        printf("-> ");
        scanf("%d", &op);
        fflush(stdin); // limpar o buffer
        return op;
    }
    else{
        printf("Buscar por nome(1) ou código de registro(2)?\n-> ");
        scanf("%d", &op);
        fflush(stdin); // limpar o buffer
        return op;
    }
}

FILE *carregar_arquivo(char modo[5]){ // Criação e/ou abertura de arquivos para serem manipulados
    FILE *arquivo = fopen("Agenda.txt", modo);
    verifica_abertura(arquivo);
    return arquivo;
}

void verifica_abertura(FILE *p){ // checa abertura/criação do arquivo, encerra o programa caso haja um ponteiro nulo
    if(p == NULL){
        printf("Erro na criação e/ou abertura do arquivo.\n");
        exit(1);
    }
}

long int contador_de_contatos(){ // retorna para a chamada da função o número de contatos
    /*
        Serve para contar quantos blocos de dados da struct tp_pessoa foram gravados no arquivo de texto, e assim, saber
        quantos contatos foram cadastrados, faz isso jogando o ponteiro do arquivo de texto pro final com fseek e usando
        ftell para obter os bytes e depois divide pelo tamanho do tp_pessoa
    */
    FILE *arquivo = carregar_arquivo("a");
    fseek(arquivo, 0, SEEK_END); // joga o ponteiro para o final do arquivo
    long int t = ftell(arquivo) / sizeof(tp_pessoa); // converte os bytes do arquivo em número de contatos cadastrados
    fclose(arquivo);
    return t;
}

void add_contato(tp_pessoa *p, tp_data data){ // cadastra um novo contato
    /*
        Recebe um ponteiro p para um vetor dinamicamente alocado com 1 posição, faz a leitura dos dados que o usuário insere
        colocando-os no vetor e depois grava isso no arquivo de texto
    */
    fflush(stdin); // limpar o buffer
    long int t = contador_de_contatos();
    int op; // op armazena opção referente ao telefone
    printf("Nome.......: ");
    gets(p->nome);
    printf("CPF........: ");
    gets(p->cpf);
    p->cod_de_registro = t+1; // codigo gerado com base na quantidade de contatos
    printf("Sexo.......: ");
    p->sexo = getche();
    p->sexo = toupper(p->sexo);
    printf("\nData de nascimento dia/mes/ano:\n");
    printf("\tDia - ");
    scanf("%d", &p->dia_nasc);
    printf("\tMês - ");
    scanf("%d", &p->mes_nasc);
    printf("\tAno - ");
    scanf("%d", &p->ano_nasc);
    fflush(stdin); // limpa buffar
    p->idade = data.ano - p->ano_nasc; // define a idade baseado no ano de nascimento
    printf("Endereço completo:\n");
    printf("\tLagradouro...: ");
    gets(p->endereco.lagradouro);
    printf("\tNúmero.......: ");
    gets(p->endereco.numero);
    printf("\tBairro.......: ");
    gets(p->endereco.bairro);
    printf("\tCEP..........: ");
    gets(p->endereco.cep);
    printf("\tCidade.......: ");
    gets(p->endereco.cidade);
    printf("\tEstado.......: ");
    gets(p->endereco.estado);
    printf("\tPaís.........: ");
    gets(p->endereco.pais);
    do{ // para o usuário escolher entre quais dos telefones ele quer cadastrar, ou cadastrar os 2
        op = sub_menu();
        switch(op){
            case 1: // Celular
                printf("\tCelular - ");
                gets(p->telefone.tel_celular);
                strcpy(p->telefone.tel_fixo," ");
                break;

            case 2: // Fixo
                printf("\tFixo - ");
                gets(p->telefone.tel_fixo);
                strcpy(p->telefone.tel_celular," ");
                break;

            case 3: // Celular e fixo
                printf("\tCelular - ");
                gets(p->telefone.tel_celular);
                printf("\tFixo - ");
                gets(p->telefone.tel_fixo);
                break;

            default: // Qualquer outra opção
                printf("Opção invalida, pelo menos um número deve ser cadastrado.\n");

        }
    }while(!(op == 1 || op == 2 || op == 3));

    FILE *arquivo = carregar_arquivo("a");
    if(fwrite(p, sizeof(*p), 1, arquivo) == 0){
        printf("Erro ao gravar os dados do contato!\n");
        system("pause");
        exit(1);
    }
    fclose(arquivo);

    printf("\n--------------------------------------------- Sistema ATD - Menu Principal --------------------------------------------\n\n");
}

void exibir_agenda(tp_pessoa p[]){ // Exibe todos os contatos da agenda
    /*
        Abre o arquivo Agenda.txt com permissão de leitura, usando fread lê as estruturas gravadas e as coloca em um vetor
        que depois é printado usando um for()
    */
    FILE *arquivo = carregar_arquivo("r");
    long int t = contador_de_contatos(); // pega o numero total de contatos
    int verificador = fread(p, t*sizeof(tp_pessoa), 1, arquivo);
    if(verificador == 0){
        printf("Ocorreu um erro inesperado na leitura do arquivo.\n\n");
        system("pause");
    }
    else{
        for(int i = 0; i < t; i++){
            printf(" ______________________________________________________\n|\n");
            printf("| Nome.......: %s\n", p[i].nome);
            printf("| CPF........: %s\n", p[i].cpf);
            printf("| Código.....: %d\n", p[i].cod_de_registro);
            printf("| Telefone:\n");
            printf("| \tCelular...: %s\n", p[i].telefone.tel_celular);
            printf("| \tFixo......: %s\n", p[i].telefone.tel_fixo);
            printf("| Sexo.......: %c\n", p[i].sexo);
            printf("| Data de nascimento %d/%d/%d\n", p[i].dia_nasc, p[i].mes_nasc, p[i].ano_nasc);
            printf("| Idade: %d\n", p[i].idade);
            printf("| Endereço completo:\n");
            printf("| \tLagradouro...: %s\n", p[i].endereco.lagradouro);
            printf("| \tNúmero.......: %s\n", p[i].endereco.numero);
            printf("| \tBairro.......: %s\n", p[i].endereco.bairro);
            printf("| \tCEP..........: %s\n", p[i].endereco.cep);
            printf("| \tCidade.......: %s\n", p[i].endereco.cidade);
            printf("| \tEstado.......: %s\n", p[i].endereco.estado);
            printf("| \tPaís.........: %s\n", p[i].endereco.pais);
            printf("|______________________________________________________\n\n");
        }
    }

    fclose(arquivo);
    printf("\n--------------------------------------------- Sistema ATD - Menu Principal --------------------------------------------\n\n");
}

void exibi_contato(tp_pessoa p[]){
    /*
        Exibe todas as informações de um contato específico através de uma busca feita pelo nome ou código de registro
        A variável verificador é desnecessária, porém eu acredito que usa-la dessa forma, facilita a legibilidade do código
    */
    FILE *arquivo = carregar_arquivo("r");
    int op, cod, verificador;
    char nome[80];
    long int t = contador_de_contatos();
    do{
        op = menu_busca(0);
        switch(op){
            case 1: // busca por nome
                printf("Digite o nome do contato, caracteres maiúsculos e minúsculos são considerados:\n-> ");
                gets(nome);
                verificador = fread(p, t*sizeof(tp_pessoa), 1, arquivo);
                if(verificador == 0){
                    printf("Ocorreu um erro inesperado na leitura do arquivo\n\n");
                    system("pause");
                    break;
                }
                for(int i = 0; i < t; i++){
                        printf("\n");
                    if(strcmp(nome, p[i].nome) == 0){
                        printf(" ______________________________________________________\n|\n");
                        printf("| Nome.......: %s\n", p[i].nome);
                        printf("| Código.....: %d\n", p[i].cod_de_registro);
                        printf("| Telefone:\n");
                        printf("| \tCelular...: %s\n", p[i].telefone.tel_celular);
                        printf("| \tFixo......: %s\n", p[i].telefone.tel_fixo);
                        printf("| Sexo.......: %c\n", p[i].sexo);
                        printf("| Idade: %d\n", p[i].idade);
                        printf("| Endereço completo:\n");
                        printf("| \tLagradouro...: %s\n", p[i].endereco.lagradouro);
                        printf("| \tNúmero.......: %s\n", p[i].endereco.numero);
                        printf("| \tBairro.......: %s\n", p[i].endereco.bairro);
                        printf("| \tCEP..........: %s\n", p[i].endereco.cep);
                        printf("| \tCidade.......: %s\n", p[i].endereco.cidade);
                        printf("| \tEstado.......: %s\n", p[i].endereco.estado);
                        printf("| \tPaís.........: %s\n", p[i].endereco.pais);
                        printf("|______________________________________________________\n\n");
                        break; // serve para evitar que o else a baixo seja execultado após o contato ser encontrado antes do final do vetor
                    }
                    else{
                        if(i == (t-1)){ // Verifica se o for já chegou no final do vetor
                            printf("Contato não encontrado.\n\n");
                            break;
                        }
                    }
                }

                break;

            case 2: // busca por codigo
                printf("Digite o código de registro: ");
                scanf("%d", &cod);
                fflush(stdin); // limpar o buffer
                verificador = fread(p, t*sizeof(tp_pessoa), 1, arquivo);
                if(verificador == 0){
                    printf("Ocorreu um erro inesperado na leitura do arquivo\n\n");
                    system("pause");
                    break;
                }
                for(int i = 0; i < t; i++){
                        printf("\n");
                    if(cod == p[i].cod_de_registro){
                        printf(" ______________________________________________________\n|\n");
                        printf("| Nome.......: %s\n", p[i].nome);
                        printf("| CPF........: %s\n", p[i].cpf);
                        printf("| Código.....: %d\n", p[i].cod_de_registro);
                        printf("| Telefone:\n");
                        printf("| \tCelular...: %s\n", p[i].telefone.tel_celular);
                        printf("| \tFixo......: %s\n", p[i].telefone.tel_fixo);
                        printf("| Sexo.......: %c\n", p[i].sexo);
                        printf("| Idade: %d\n", p[i].idade);
                        printf("| Endereço completo:\n");
                        printf("| \tLagradouro...: %s\n", p[i].endereco.lagradouro);
                        printf("| \tNúmero.......: %s\n", p[i].endereco.numero);
                        printf("| \tBairro.......: %s\n", p[i].endereco.bairro);
                        printf("| \tCEP..........: %s\n", p[i].endereco.cep);
                        printf("| \tCidade.......: %s\n", p[i].endereco.cidade);
                        printf("| \tEstado.......: %s\n", p[i].endereco.estado);
                        printf("| \tPaís.........: %s\n", p[i].endereco.pais);
                        printf("|______________________________________________________\n\n");
                        break; // serve para evitar que o else a baixo seja execultado após o contato ser encontrado antes do final do vetor
                    }
                    else{
                        if(i == (t-1)){ // Verifica se o for já chegou no final do vetor
                            printf("Contato não encontrado.\n\n");
                            break;
                        }
                    }

                }

                break;

            default: // opção invalida
                printf("Opção invalida.\n");
        }
    }while(!(op == 1 || op == 2 | op == 0));
    fclose(arquivo);
    printf("\n--------------------------------------------- Sistema ATD - Menu Principal --------------------------------------------\n\n");
}

void busca_contato(tp_pessoa *p, tp_data data, int k){
    /*
        Localiza um contato específico e da ao usuário a opção de alterar ou excluir o contato e também de retornar ao menu.
        Recebe int k como parâmetro, isso define se o usuário irá direto para opções de alteração ou exclusão do contato.
    */
    FILE *arquivo = carregar_arquivo("r");
    int op, cod, verificador;
    char nome[80];
    long int t = contador_de_contatos();
    do{
        op = menu_busca(0);
        switch(op){
            case 1: // busca por nome
                printf("Digite o nome do contato, caracteres maiúsculos e minúsculos são considerados:\n-> ");
                gets(nome);
                verificador = fread(p, t*sizeof(tp_pessoa), 1, arquivo);
                if(verificador == 0){
                    printf("Ocorreu um erro inesperado na leitura do arquivo\n\n");
                    system("pause");
                    break;
                }
                for(int i = 0; i < t; i++){
                        printf("\n");
                    if(strcmp(nome, p[i].nome) == 0){
                        printf(" ______________________________________________________\n|\n");
                        printf("| Nome.......: %s\n", p[i].nome);
                        printf("| Código.....: %d\n", p[i].cod_de_registro);
                        printf("|______________________________________________________\n\n");
                        if(k == 1 || k == 2){
                            if(k == 1){
                                op = 1;
                            }
                            else{
                                op = 2;
                            }
                        }
                        else{
                            op = menu_busca(1);
                        }

                        do{
                            switch(op){
                                case 1: // Alterar contato
                                    altera_contato(p, p[i].cod_de_registro, data);
                                    break;

                                case 2: // Excluir Contato
                                    exclui_contato(p, p[i].cod_de_registro);
                                    break;

                                case 0: // Voltar ao menu principal
                                    break;

                                default:
                                    printf("Opção inválida.\n");
                            }
                        }while(!(op == 1 | op == 2 | op == 0));
                        break;
                    }
                    else{
                        if(i == (t-1)){ // Verifica se o for já chegou no final do vetor
                            printf("Contato não encontrado.\n\n");
                            break;
                        }
                    }
                }

                break;

            case 2: // busca por codigo
                printf("Digite o código de registro: ");
                scanf("%d", &cod);
                fflush(stdin); // limpar o buffer
                verificador = fread(p, t*sizeof(tp_pessoa), 1, arquivo);
                if(verificador == 0){
                    printf("Ocorreu um erro inesperado na leitura do arquivo\n\n");
                    system("pause");
                    break;
                }
                for(int i = 0; i < t; i++){
                        printf("\n");
                    if(cod == p[i].cod_de_registro){
                        printf(" ______________________________________________________\n|\n");
                        printf("| Nome.......: %s\n", p[i].nome);
                        printf("| Código.....: %d\n", p[i].cod_de_registro);
                        printf("|______________________________________________________\n\n");
                        if(k == 1 || k == 2){
                            if(k == 1){
                                op = 1;
                            }
                            else{
                                op = 2;
                            }
                        }
                        else{
                            op = menu_busca(1);
                        }

                        do{
                            switch(op){
                                case 1: // Alterar contato
                                    altera_contato(p, p[i].cod_de_registro, data);
                                    break;

                                case 2: // Excluir Contato
                                    exclui_contato(p, p[i].cod_de_registro);
                                    break;

                                case 0: // Voltar ao menu principal
                                    break;

                                default:
                                    printf("Opção inválida.\n");
                            }
                        }while(!(op == 1 | op == 2 | op == 0));
                        break;
                    }
                    else{
                        if(i == (t-1)){ // Verifica se o for já chegou no final do vetor
                            printf("Contato não encontrado.\n\n");
                            break;
                        }
                    }

                }

                break;

            default: // opção invalida
                printf("Opção invalida.\n");
        }
    }while(!(op == 1 || op == 2 | op == 0));
    fclose(arquivo);
    printf("\n--------------------------------------------- Sistema ATD - Menu Principal --------------------------------------------\n\n");
}

void altera_contato(tp_pessoa p[], int k, tp_data data){
    /*
        A função carrega todos os contatos do arquivo de texto para um vetor p usando fread, com base no indice k recebido, a função
        encontra o contato que será alterado e o usuário insere novas informações em seus devidos campos no vetor, sobreescrevendo-os, depois
        o modo de leitura do arquivo é encerrado e começa um modo de escrita, que por sua vez cria um novo arquivo e grava o vetor todo de novo
        Nessa função, o código de registro não é alterado.
    */
    long int t = contador_de_contatos();
    k--; // K veio como um indice, mas aqui vai lidar com vetor, portando como vetor começa em 0 e não em 1, ele é decrementado em 1
    int op, verificador; // op armazena opção referente ao telefone
    FILE *arquivo = carregar_arquivo("r");
    verificador = fread(p, t*sizeof(tp_pessoa), 1, arquivo);
    if(verificador == 0){
        printf("Ocorreu um erro inesperado na leitura do arquivo.\n\n");
        system("pause");
        exit(1);
    }

    fflush(stdin); // limpar o buffer
    printf("Nome antigo: %s\n\t-> ", p[k].nome);
    gets(p[k].nome);
    printf("CPF antigo: %s\n\t-> ", p[k].cpf);
    gets(p[k].cpf);
    printf("Gênero antigo: %c\n\t-> ", p[k].sexo);
    p[k].sexo = getche();
    p[k].sexo = toupper(p[k].sexo);
    printf("\nData de nascimento %d/%d/%d:\n", p[k].dia_nasc, p[k].mes_nasc, p[k].ano_nasc);
    printf("\tDia - ");
    scanf("%d", &p[k].dia_nasc);
    printf("\tMês - ");
    scanf("%d", &p[k].mes_nasc);
    printf("\tAno - ");
    scanf("%d", &p[k].ano_nasc);
    fflush(stdin); // limpa buffar
    p[k].idade = data.ano - p[k].ano_nasc; // define a idade baseado no ano de nascimento
    printf("Endereço completo:\n");
    printf("\tLagradouro...: ");
    gets(p[k].endereco.lagradouro);
    printf("\tNúmero.......: ");
    gets(p[k].endereco.numero);
    printf("\tBairro.......: ");
    gets(p[k].endereco.bairro);
    printf("\tCEP..........: ");
    gets(p[k].endereco.cep);
    printf("\tCidade.......: ");
    gets(p[k].endereco.cidade);
    printf("\tEstado.......: ");
    gets(p[k].endereco.estado);
    printf("\tPaís.........: ");
    gets(p[k].endereco.pais);
    do{ // para o usuário escolher entre quais dos telefones ele quer cadastrar, ou cadastrar os 2
        op = sub_menu();
        switch(op){
            case 1: // Celular
                printf("\tCelular - ");
                gets(p[k].telefone.tel_celular);
                strcpy(p[k].telefone.tel_fixo," ");
                break;

            case 2: // Fixo
                printf("\tFixo - ");
                gets(p[k].telefone.tel_fixo);
                strcpy(p[k].telefone.tel_celular," ");
                break;

            case 3: // Celular e fixo
                printf("\tCelular - ");
                gets(p[k].telefone.tel_celular);
                printf("\tFixo - ");
                gets(p[k].telefone.tel_fixo);
                break;

            default: // Qualquer outra opção
                printf("Opção invalida, pelo menos um número deve ser cadastrado.\n");

        }
    }while(!(op == 1 || op == 2 || op == 3));

    fclose(arquivo); // encerrar permissão de leitura

    arquivo = carregar_arquivo("w"); // dando permissão para sobrescrever o arquivo com o novo vetor com dados alterados pelo usuario
    verificador = fwrite(p, sizeof(tp_pessoa), t, arquivo);
    if(verificador == 0){
        printf("Ocorreu um erro ao regravar os dados no arquivo.\n\n");
        system("pause");
        exit(1);
    }
    printf("Dados alterados com sucesso.\n\n");
    fclose(arquivo);

}

void exclui_contato(tp_pessoa p[], int k){
    /*
        Recebe um vetor e um indice k que sinaliza onde está o contato a ser excluido, faz a leitura do arquivo e coloca tudo dentro do vetor,
        copia os dados dos contatos subsequentes em cima do que precisa ser deletado, faz isso a partir do contato a ser deletado
        até o final do vetor, decremente a variavel t que determina a quantidade de contatos, realoca o vetor com o novo valor de t para remover
        a ultima posição do vetor e depois reescreve todo o arquivo de texto com o novo vetor.
        Se só haver 1 contato na agenda quando essa função for chamada e t tomar o valor de 0, o arquivo é criado, mas a alocação e escrita não é feita
        Como o código de registro é gerado com base na quantidade de contatos, caso contatos fossem deletados e os códigos que ficarem não fosse
        reorganizados, ao cadastrar novos contatos, o programa geraria códigos repetidos, para evitar isso, o código de cada contato após
        aquele que foi excluir é decrementado.
    */
    long int t = contador_de_contatos();
    k--; // K veio como um indice, mas aqui vai lidar com vetor, portando como vetor começa em 0 e não em 1, ele é decrementado em 1
    FILE *arquivo = carregar_arquivo("r");
    int verificador = fread(p, t*sizeof(tp_pessoa), 1, arquivo);
    if(verificador == 0){
        printf("Ocorreu um erro inesperado na leitura do arquivo.\n\n");
        system("pause");
        exit(1);
    }

    for(int i = k; i < t; i++){
        p[i] = p[i+1];
        p[i].cod_de_registro = p[i].cod_de_registro - 1; // reorganiza o código de registro
    }
    fclose(arquivo);

    t = t - 1;
    arquivo = carregar_arquivo("w"); // dando permissão para sobrescrever o arquivo com o novo vetor
    if(t != 0){
        p = (tp_pessoa*) realloc(p, t*sizeof(tp_pessoa));
        verificador = fwrite(p, sizeof(tp_pessoa), t, arquivo);
        if(verificador == 0){
            printf("Ocorreu um erro ao regravar os dados no arquivo.\n\n");
            system("pause");
            exit(1);
        }
    }
    printf("Contato excluido com sucesso.\n\n");
    fclose(arquivo);
}

void lixeira(){
    /*
        Poderia usar a função remove para apagar a agenda, mas daria mais margem de erro para o programa, se eu usar fopen("Agenda.txt", "w")
        o programa vai deletar o arquivo antigo e criar um novo, oque já compre o objetivo da função
    */
    FILE *arquivo = carregar_arquivo("w");
    printf("Agenda deletada.\n");
    fclose(arquivo);

    printf("\n--------------------------------------------- Sistema ATD - Menu Principal --------------------------------------------\n\n");
}

void defineData(tp_data *data, struct tm *tempo, time_t *tempoSeg){ // pega data e hora do sistema operacional
    time(tempoSeg);
    tempo = localtime(tempoSeg);
    data->dia = tempo->tm_mday;
    data->mes = tempo->tm_mon + 1;
    data->ano = tempo->tm_year + 1900;
    data->segundo = tempo->tm_sec;
    data->minuto = tempo->tm_min;
    data->hora = tempo->tm_hour;
}
