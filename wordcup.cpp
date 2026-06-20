/*
 *  WORLDCUP 2026 - SISTEMA DE CONSULTA E SIMULACAO
 *  Copa do Mundo FIFA 2026 - EUA, Canada, Mexico
 *  Disciplina: Algoritmos e Programacao 2
 *  Aluno: Paulo Prado Minotto
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>   /*Srand*/
#include <ctype.h>  /*toLower*/ 
#define MAX_JOGADORES 26
#define MAX_SELECOES 48
#define MAX_POR_GRUPO 4
#define MAX_GRUPOS 12
#define PASTA_SELECOES "selecoes"

typedef struct {
    char nome[50];
    char posicao[20];
    char clube[40];
    int  numero;
    int  overall;
} Atleta;
typedef struct {
    char   nome[50];
    char   grupo;
    int    forca;
    Atleta jogadores[MAX_JOGADORES];
    int    numJogadores;
/* Estatisticas da fase de grupos */
    int    pontos;
    int    vitorias;
    int    empates;
    int    derrotas;
    int    golsFavor;
    int    golsContra;
} Selecao;
typedef struct {
    char     nome[20];
    Selecao *times[MAX_SELECOES];
    int    numTimes;
} Grupo;


void exibirMenuPrincipal(void);
void exibirMenuConsultas(void);
void exibirMenuSimulacao(void);
void exibirMenuArquivos(void);
int  carregarSelecaoArquivo(Selecao *s, const char *caminho);
void carregarTodasSelecoes(Selecao sel[]);
void salvarTodasSelecoes(Selecao sel[]);
void inicializarGrupos(Grupo grupos[], Selecao sel[]);
void toLower(char *s);
void limparBuffer(void);
void pausar(void);
void cabecalho(const char *titulo);
void cabecalhoGrupo(const char *titulo);
void consultarSelecao(Selecao sel[]);
void listarGrupo(Grupo grupos[]);
void listarTodasSelecoes(Selecao sel[]);
void buscarAtleta(Selecao sel[]);
void tabelaGrupos(Grupo grupos[]);
int  gerarPlacar(void);
void simularPartida(Selecao *a, Selecao *b);
void ordenarGrupo(Grupo *g);
void simularFaseGrupos(Grupo grupos[]);
void simularFaseGruposSilencioso(Grupo grupos[]);
void simularFaseEli(Selecao *a, Selecao *b, const char *fase, Selecao **vencedor);
void simularCopa(Grupo grupos[]);

int main (){
/*Semente para resultados aleatorios*/
    srand(time(NULL)); 
    Selecao selecoes[MAX_SELECOES];
    Grupo   grupos[MAX_GRUPOS];

    memset(selecoes, 0, sizeof(selecoes));
    memset(grupos, 0, sizeof(grupos));
/*zera as estruturas para evitar lixo de memoria*/
    carregarTodasSelecoes(selecoes);
    inicializarGrupos(grupos, selecoes);

    printf ("\n");
    printf ("Bem vindo ao WorldCup 2026 - Sistema de consulta e simulacao da Copa do Mundo FIFA 2026! \n");

    int opcao;
    do {
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBuffer(); /*Limpa o buffer para evitar problemas com scanf e getchar*/
        switch (opcao){
            case 1:{
                int sub;
                do {
                    exibirMenuConsultas(); /*CORRIGIDO: era exibirMenuPrincipal()*/
                    scanf("%d", &sub);
                    limparBuffer();
                    switch (sub){
                        case 1: consultarSelecao(selecoes); break;
                        case 2: listarGrupo(grupos); break; 
                        case 3: listarTodasSelecoes(selecoes); break;
                        case 4: buscarAtleta(selecoes); break; 
                        case 0: break;
                        default: printf("Opacao invalida! Tente novamente. \n");
                    }
                }while (sub != 0); break;
            }
            case 2: {
                int sub; 
                do {
                    exibirMenuSimulacao();
                    scanf("%d", &sub);
                    limparBuffer();
                    switch (sub){
                        case 1: simularFaseGrupos(grupos); break; 
                        case 2: simularCopa(grupos); break; 
                        case 3: tabelaGrupos(grupos); break; 
                        case 0: break; 
                        default: printf("Opacao invalida! Tente novamente. \n");
                    }
                }while (sub != 0); break;
            } 
            case 3: {
                int sub;
                do {
                    exibirMenuArquivos();
                    scanf("%d", &sub);
                    limparBuffer();
                    switch (sub) {
                        case 1:
                            carregarTodasSelecoes(selecoes);
                            inicializarGrupos(grupos, selecoes);
                            printf("\n  Selecoes recarregadas dos arquivos!\n");
                            pausar(); break;
                        case 2:
                            salvarTodasSelecoes(selecoes); break;
                        case 0: break;
                        default: printf("  Opcao invalida!\n");
                    }
                } while (sub != 0); break; 
            }
            case 0: printf ("Obrigado por usar WorldCup! \n"); break;
            default: printf ("Opacao invalida! Tente novamente. \n");
        }
    }while (opcao != 0);
    return 0;
}

void exibirMenuPrincipal(void){
    printf ("\n"); 
    printf ("Menu principal! \n");
    printf ("1 - Consultas \n");
    printf ("2 - Simulacoes \n");
    printf ("3 - Arquivos \n");
    printf ("0 - Sair \n");
    printf ("Escolha uma opcao acima: "); 
}
void exibirMenuConsultas(void){
    printf ("\n");
    printf ("Menu de consultas \n");
    printf ("1 - Consultar selecao \n");
    printf ("2 - Listar grupo \n");
    printf ("3 - Listar todas as selecoes \n");
    printf ("4 - Buscar atleta \n");
    printf ("0 - Voltar ao menu principal \n");
    printf ("Escolha uma opcao acima: ");
}
void exibirMenuSimulacao(void){
    printf ("\n");
    printf ("Menu de simulacao \n");
    printf ("1 - Simular fase de grupos \n");
    printf ("2 - Simular Copa do Mundo FIFA 2026 \n");
    printf ("3 - Exibir tabela da fase de Grupos \n");
    printf ("0 - Voltar ao menu principal \n"); 
}
void exibirMenuArquivos(void) {
    printf("\n");
    printf(" Menu de Arquivos \n");
    printf("1 - Recarregar selecoes dos arquivos .txt\n");
    printf("2 - Salvar alteracoes nos arquivos .txt\n");
    printf("0 - Voltar \n");
    printf("  Escolha: "); 
}
void cabecalho(const char *titulo){
    printf ("\n========================================\n");
    printf ("  %s\n", titulo);
    printf ("========================================\n");
}
void cabecalhoGrupo(const char *titulo){
    printf ("\n--- Grupo %s ---\n", titulo);
}

int carregarSelecaoArquivo(Selecao *s, const char *caminho) {
    FILE *arq = fopen(caminho, "r");
    if (arq == NULL) return 0;
    /* Le primeira linha: nome|grupo|forca */
    fscanf(arq, "%49[^|]|%c|%d\n", s->nome, &s->grupo, &s->forca);
    /* Le jogadores linha por linha ate fim do arquivo */
    int j = 0;
    while (j < MAX_JOGADORES && fscanf(arq, "%49[^|]|%19[^|]|%39[^|]|%d|%d\n", 
        s->jogadores[j].nome, s->jogadores[j].posicao, s->jogadores[j].clube, 
        &s->jogadores[j].numero, &s->jogadores[j].overall) == 5) {
        j++;
    }
    s->numJogadores = j;
    fclose(arq);
    return 1;
}
void carregarTodasSelecoes(Selecao sel[]) {
    /* Lista de nomes dos arquivos (sem .txt) */
    const char *arquivos[MAX_SELECOES] = {
        /* Grupo A */ "mexico",      "africa_do_sul", "coreia_do_sul", "rep_tcheca",
        /* Grupo B */ "canada",      "qatar",         "suica",         "italia",
        /* Grupo C */ "brasil",      "marrocos",      "haiti",         "escocia",
        /* Grupo D */ "eua",         "paraguai",      "australia",     "turquia",
        /* Grupo E */ "alemanha",    "curcao",        "costa_marfim",  "equador",
        /* Grupo F */ "holanda",     "japao",         "tunisia",       "suecia",
        /* Grupo G */ "belgica",     "egito",         "ira",           "nova_zelandia",
        /* Grupo H */ "espanha",     "cabo_verde",    "arabia_saudita","uruguai",
        /* Grupo I */ "franca",      "senegal",       "noruega",       "iraque",
        /* Grupo J */ "argentina",   "algeria",       "austria",       "jordania",
        /* Grupo K */ "portugal",    "uzbequistao",   "colombia",      "rd_congo",
        /* Grupo L */ "inglaterra",  "croacia",       "gana",          "panama"
    };
    int carregados = 0; /*Contador que vai acumulando quantos arquivos foram lidos com sucesso*/
    for (int i = 0; i < MAX_SELECOES; i++) {
        char caminho[100];
        sprintf(caminho, "%s/%s.txt", PASTA_SELECOES, arquivos[i]);
        if (carregarSelecaoArquivo(&sel[i], caminho))
            carregados++;
        else
            printf("  AVISO: nao encontrei '%s'\n", caminho);
    }
    printf("\n  %d selecoes carregadas dos arquivos!\n", carregados);
}
void salvarTodasSelecoes(Selecao sel[]) {
    const char *arquivos[MAX_SELECOES] = {
        /* Grupo A */ "mexico",      "africa_do_sul", "coreia_do_sul", "rep_tcheca",
        /* Grupo B */ "canada",      "qatar",         "suica",         "italia",
        /* Grupo C */ "brasil",      "marrocos",      "haiti",         "escocia",
        /* Grupo D */ "eua",         "paraguai",      "australia",     "turquia",
        /* Grupo E */ "alemanha",    "curcao",        "costa_marfim",  "equador",
        /* Grupo F */ "holanda",     "japao",         "tunisia",       "suecia",
        /* Grupo G */ "belgica",     "egito",         "ira",           "nova_zelandia",
        /* Grupo H */ "espanha",     "cabo_verde",    "arabia_saudita","uruguai",
        /* Grupo I */ "franca",      "senegal",       "noruega",       "iraque",
        /* Grupo J */ "argentina",   "algeria",       "austria",       "jordania",
        /* Grupo K */ "portugal",    "uzbequistao",   "colombia",      "rd_congo",
        /* Grupo L */ "inglaterra",  "croacia",       "gana",          "panama"
    };
    int salvos = 0;
    for (int i = 0; i < MAX_SELECOES; i++) {
        if (sel[i].nome[0] == '\0') continue;
        char caminho[100];
        sprintf(caminho, "%s/%s.txt", PASTA_SELECOES, arquivos[i]);
        FILE *arq = fopen(caminho, "w");
        if (!arq) { printf("  ERRO ao salvar '%s'\n", caminho); continue; }
        fprintf(arq, "%s|%c|%d\n", sel[i].nome, sel[i].grupo, sel[i].forca);
        for (int j = 0; j < sel[i].numJogadores; j++) {
            Atleta *a = &sel[i].jogadores[j];
            fprintf(arq, "%s|%s|%s|%d|%d\n", a->nome, a->posicao, a->clube, a->numero, a->overall);
        }
        fclose(arq);
        salvos++;
    }
    printf("\n  %d selecoes salvas nos arquivos!\n", salvos);
    pausar();
}
void inicializarGrupos(Grupo grupos[], Selecao sel[]) {
    /* Nomeia os grupos de A a L */
    for (int g = 0; g < MAX_GRUPOS; g++) {
        grupos[g].nome[0] = (char)('A' + g);
        grupos[g].nome[1] = '\0';
        grupos[g].numTimes = 0;
    }
    /* Distribui as selecoes nos grupos conforme o campo grupo */
    for (int i = 0; i < MAX_SELECOES; i++) {
        if (sel[i].nome[0] == '\0') continue;
        int gi = (int)(toupper((unsigned char)sel[i].grupo) - 'A');
        if (gi < 0 || gi >= MAX_GRUPOS) continue;
        grupos[gi].times[grupos[gi].numTimes++] = &sel[i];
    }
}
/*Converte string para minusculas para facilitar comparacoes e buscas sem diferenciar maiusculas/minusculas*/
void toLower(char *s) { /* string em C termina com \0, ent o enquanto si for qlq letra o for continua*/
    for (int i = 0; s[i]; i++) s[i] = (char)tolower((unsigned char)s[i]);
}/*tolower exige valor positivo*/
/*Pausa para o usuario ler mensagens antes de continuar*/
void limparBuffer(void) {
    while (getchar() != '\n');
}
void pausar(void){
    printf ("Pressione Enter para continuar...");
    getchar();
}
void consultarSelecao(Selecao sel[]){
    char busca[50], tmp1[50], tmp2[50]; /*v temporarios para comparacao sem diferenciar maiusculas/minusculas*/
    printf ("\nDigite o nome da selecao que deseja consultar: ");
    scanf("%49[^\n]", busca); /*Le a linha inteira, incluindo espacos, ate o \n ou limite de 49 caracteres*/
    limparBuffer();
    strncpy(tmp1, busca, sizeof(tmp1)); /*Copia a string de busca para tmp1 para converter em minusculas*/
    tmp1[sizeof(tmp1)-1] = '\0';
    toLower(tmp1); 

    int achei = 0;
    for (int i = 0; i < MAX_SELECOES; i++){
        strncpy(tmp2, sel[i].nome, sizeof(tmp2)); /*Copia o nome da selecao para tmp2 para converter em minusculas*/
        tmp2[sizeof(tmp2)-1] = '\0';
        toLower(tmp2); /*se retornar 0 sao iguais*/
        if (strcmp(tmp1, tmp2) == 0) {
            achei = 1; /*imprime os dados*/
            printf ("\nSelecao: %s\n", sel[i].nome);
            printf ("Grupo: %c\n", sel[i].grupo);
            printf ("Forca: %d\n", sel[i].forca);
            printf ("Jogadores convocados: %d\n\n", sel[i].numJogadores);
            printf("  %-4s  %-25s  %-15s  %-28s  OVR\n","Num","Nome","Posicao","Clube");
            printf("  %-4s  %-25s  %-15s  %-28s  ---\n", "---",
                "-------------------------","---------------","----------------------------");
            for (int j = 0; j < sel[i].numJogadores; j++) {
                Atleta *a = &sel[i].jogadores[j];
                printf("  %-4d  %-25s  %-15s  %-28s  %d\n", a->numero, a->nome, a->posicao,
                a->clube, a->overall);
            }
            break;
        }
    }
    if (!achei) printf("\n  Selecao '%s' nao encontrada!\n", busca);
    pausar();
}
void listarGrupo(Grupo grupos[]){
    char letra;
    printf("\nDigite a letra do grupo que deseja listar (A-L): ");
    scanf(" %c", &letra); /*Espaco antes de %c para ignorar qualquer \n ou espaco restante no buffer*/
    limparBuffer();
    letra = (char)toupper((unsigned char)letra); /*Converte para maiuscula para facilitar comparacao*/
    int gi = letra - 'A'; /*Calcula o indice do grupo (0 para A, 1 para B, etc)*/
    if (gi < 0 || gi >= MAX_GRUPOS) {
        printf("\n  Grupo '%c' invalido! Escolha entre A e L.\n", letra);
        return;
    }
    cabecalhoGrupo(grupos[gi].nome);
    if (grupos[gi].numTimes == 0) { /*CORRIGIDO: if/else faltando*/
        printf("\n Nenhuma selecao cadastrada neste grupo!\n");
    } else {
        for (int t = 0; t < grupos[gi].numTimes; t++) {
            Selecao *s = grupos[gi].times[t]; 
            printf("  %-25s  Forca: %d\n", s->nome, s->forca);/*Exibe nome e forca de cada selecao do grupo*/
        }/*s-> acessa pelo endereco*/
    }
    pausar();
}
void listarTodasSelecoes(Selecao sel[]){
    printf("\nSELECOES NO SISTEMA\n");
    printf("  %-3s  %-20s  GRP  FORCA\n", "N.", "Selecao");
    printf("  %-3s  %-20s  ---  -----\n", "---", "--------------------");
    for (int i = 0; i < MAX_SELECOES; i++) {
        if (sel[i].nome[0] == '\0') continue;
        printf("  %-3d  %-20s   %c    %d\n",
            i+1, sel[i].nome, sel[i].grupo, sel[i].forca);
    }
    pausar();
}
void buscarAtleta(Selecao sel[]) {
    char busca[50], tmp1[50], tmp2[50];
    printf("\n  Digite o nome do atleta: ");
    scanf("%49[^\n]", busca);
    limparBuffer();
    strncpy(tmp1, busca, sizeof(tmp1));
    tmp1[sizeof(tmp1)-1] = '\0';
    toLower(tmp1);
    int achei = 0;
    for (int i = 0; i < MAX_SELECOES; i++) {
        for (int j = 0; j < sel[i].numJogadores; j++) {
            strncpy(tmp2, sel[i].jogadores[j].nome, sizeof(tmp2));
            tmp2[sizeof(tmp2)-1] = '\0';
            toLower(tmp2);
            if (strstr(tmp2, tmp1) != NULL) {
                achei = 1;
                Atleta *a = &sel[i].jogadores[j];
                printf("\n  +--------------------------------+\n");
                printf("  | Atleta : %-22s|\n", a->nome);
                printf("  | Selecao: %-22s|\n", sel[i].nome);
                printf("  | Grupo  : %c                      |\n", sel[i].grupo);
                printf("  | Posicao: %-22s|\n", a->posicao);
                printf("  | Clube  : %-22s|\n", a->clube);
                printf("  | Numero : %-22d|\n", a->numero);
                printf("  | Overall: %-22d|\n", a->overall);
                printf("  +--------------------------------+\n");
            }
        }
    }
    if (!achei)
        printf("\n  Atleta '%s' nao encontrado!\n", busca);
    pausar();
}
void tabelaGrupos(Grupo grupos[]) {
    char letra;
    printf("\n  Grupo (A-L): ");
    scanf(" %c", &letra);
    limparBuffer();
    letra = (char)toupper((unsigned char)letra);
    int gi = letra - 'A';
    if (gi < 0 || gi >= MAX_GRUPOS || grupos[gi].numTimes == 0) {
        printf("  Grupo invalido ou sem selecoes.\n");
        return;
    }
    ordenarGrupo(&grupos[gi]);
    cabecalhoGrupo(grupos[gi].nome);
    printf("  %-20s  PTS   V   E   D  GF  GC  SG\n", "Selecao");
    printf("  %-20s  ---   -   -   -  --  --  --\n", "--------------------");
    for (int t = 0; t < grupos[gi].numTimes; t++) {
        Selecao *s = grupos[gi].times[t];
        int sg = s->golsFavor - s->golsContra;
        printf("  %-20s  %3d   %d   %d   %d  %2d  %2d  %+d\n",
            s->nome, s->pontos,
            s->vitorias, s->empates, s->derrotas,
            s->golsFavor, s->golsContra, sg);
    }
    pausar();
}
/*rand() % 4 gera sempre 0, 1, 2 ou 3*/
int gerarPlacar(void) {
    return rand() % 4;
}
void simularPartida(Selecao *a, Selecao *b) {
    int golsA = gerarPlacar(); /*CORRIGIDO: faltava ;*/
    int golsB = gerarPlacar(); /*CORRIGIDO: faltava ;*/
    printf("\n  %s %d x %d %s\n", a->nome, golsA, golsB, b->nome);
    /*Atualiza estatisticas de cada selecao com base no resultado*/
    a->golsFavor += golsA;
    a->golsContra += golsB;
    b->golsFavor += golsB;
    b->golsContra += golsA;
    if (golsA > golsB) {
        a->pontos += 3;
        a->vitorias++;
        b->derrotas++;
    } else if (golsA < golsB) {
        b->pontos += 3;
        b->vitorias++;
        a->derrotas++;
    } else {
        a->pontos += 1;
        b->pontos += 1;
        a->empates++;
        b->empates++;
    }
}
void ordenarGrupo(Grupo *g) { /*usado bubble sort para ordenar as selecoes*/
    int i, j;
    for (i = 0; i < g->numTimes - 1; i++) {
        for (j = 0; j < g->numTimes - 1 - i; j++) {
            Selecao *a = g->times[j]; /*Ponteiro para a selecao na posicao j*/
            Selecao *b = g->times[j+1]; 
            /* Calcula saldo de gols dos dois */
            int sgA = a->golsFavor - a->golsContra;
            int sgB = b->golsFavor - b->golsContra;
            /* Decide se troca: 1o pontos, 2o saldo, 3o gols */
            int trocar = 0;
            if (a->pontos < b->pontos)          trocar = 1;
            else if (a->pontos == b->pontos) {
                if (sgA < sgB)                  trocar = 1;
                else if (sgA == sgB) {
                    if (a->golsFavor < b->golsFavor) trocar = 1;
                }
            }
            if (trocar) { 
                g->times[j]   = b; 
                g->times[j+1] = a;
            }
        }
    }
}
void simularFaseGrupos(Grupo grupos[]){
    cabecalho("SIMULACAO DA FASE DE GRUPOS!");
    /*reseta as estatisticas antes de comecar o modulo*/
    for (int g = 0; g < MAX_GRUPOS; g++)
        for (int t = 0; t < grupos[g].numTimes; t++){ 
            Selecao *s = grupos[g].times[t]; 
            s->pontos = 0;
            s->vitorias = 0;
            s->empates = 0;
            s->derrotas = 0;
            s->golsFavor = 0;
            s->golsContra = 0;
        }
    /*todos contra todos dentro de cada grupo*/
    for (int g = 0; g < MAX_GRUPOS; g++){
        if (grupos[g].numTimes < 2) continue; /*Se tiver menos de 2 selecoes, nao simula*/  
        printf ("\n  Simulando Grupo %c...\n", grupos[g].nome[0]);
        int n = grupos[g].numTimes; /*numero de selecoes no grupo*/
        for (int i = 0; i < n; i++)  /*Cada selecao joga contra todas as outras uma vez*/
            for (int j = i + 1; j < n; j++)  /*Evita repetir partidas e evita que uma selecao jogue contra si mesma*/
                simularPartida(grupos[g].times[i], grupos[g].times[j]); /*Simula a partida entre as selecoes i e j*/
        ordenarGrupo(&grupos[g]); /*Ordena o grupo apos simular todas as partidas*/
        printf ("\n  Tabela do Grupo %c:\n", grupos[g].nome[0]);
        printf("  %-20s  PTS   V   E   D  GF  GC  SG\n", "Selecao");
        printf("  %-20s  ---   -   -   -  --  --  --\n", "--------------------");
        for (int t = 0; t < grupos[g].numTimes; t++) {
            Selecao *s = grupos[g].times[t];
            int sg = s->golsFavor - s->golsContra;
            printf("  %-20s  %3d   %d   %d   %d  %2d  %2d  %+d\n",
                s->nome, s->pontos,
                s->vitorias, s->empates, s->derrotas,
                s->golsFavor, s->golsContra, sg);
        }
    }
    pausar();
}
/*Versao silenciosa usada internamente por simularCopa, sem pausar entre grupos*/
void simularFaseGruposSilencioso(Grupo grupos[]){
    for (int g = 0; g < MAX_GRUPOS; g++)
        for (int t = 0; t < grupos[g].numTimes; t++){
            Selecao *s = grupos[g].times[t];
            s->pontos = s->vitorias = s->empates =
            s->derrotas = s->golsFavor = s->golsContra = 0;
        }
    for (int g = 0; g < MAX_GRUPOS; g++){
        if (grupos[g].numTimes < 2) continue;
        int n = grupos[g].numTimes;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                simularPartida(grupos[g].times[i], grupos[g].times[j]);
        ordenarGrupo(&grupos[g]);
    }
}
void simularFaseEli(Selecao *a, Selecao *b, const char *fase, Selecao **vencedor) {
    int ga = gerarPlacar();
    int gb = gerarPlacar();
    printf("\n  [%s]\n", fase); /*Exibe a fase da copa (oitavas, quartas, semi, final)*/
    printf("  %-20s %d x %d %-20s\n", a->nome, ga, gb, b->nome); 
    if (ga == gb) {
        /* Empate: penaltis decididos por cara ou coroa */
        int penalti = rand() % 2; /* 0 ou 1, 50% cada */
        printf("  => Prorrogacao!\n");
        if (penalti == 0) {
            printf("  Penaltis: %s vence!\n", a->nome);
            *vencedor = a;
        } else {
            printf("  Penaltis: %s vence!\n", b->nome);
            *vencedor = b;
        }
    } else if (ga > gb) {
        printf("  => %s avanca!\n", a->nome);
        *vencedor = a;
    } else {
        printf("  => %s avanca!\n", b->nome);
        *vencedor = b;
    }
}
void simularCopa(Grupo grupos[]){
    cabecalho("SIMULACAO DA COPA DO MUNDO FIFA 2026!");
    /* Reseta e simula fase de grupos de forma silenciosa */
    simularFaseGruposSilencioso(grupos);
    /* Monta o chaveamento das oitavas: 1o e 2o de cada grupo */
    printf("\n  Classificados para as Oitavas de Final:\n");
    Selecao *oitavas[24]; /*12 grupos x 2 classificados = 24*/
    int idc = 0; /*indice do vetor de classificados*/
    for (int g = 0; g < MAX_GRUPOS; g++) {
        if (grupos[g].numTimes >= 2) {
            printf("  Grupo %s: 1o %s | 2o %s\n",
                grupos[g].nome,
                grupos[g].times[0]->nome,
                grupos[g].times[1]->nome);
            oitavas[idc++] = grupos[g].times[0]; /*1o colocado do grupo*/
            oitavas[idc++] = grupos[g].times[1]; /*2o colocado do grupo*/
        }
    }
    /* Oitavas de final */
    cabecalho("OITAVAS DE FINAL");
    Selecao *quartas[12];
    int numQuartas = 0;
    for (int i = 0; i + 1 < idc && numQuartas < 12; i += 2) {
        Selecao *v = NULL; /*Ponteiro para armazenar o vencedor da partida*/
        simularFaseEli(oitavas[i], oitavas[i+1], "Oitavas de Final", &v);
        if (v) quartas[numQuartas++] = v; /*Armazena o vencedor das oitavas para as quartas*/
    }
    /* Quartas de final */
    cabecalho("QUARTAS DE FINAL");
    Selecao *semi[6];
    int numSemi = 0;
    for (int i = 0; i + 1 < numQuartas && numSemi < 6; i += 2) {
        Selecao *v = NULL;
        simularFaseEli(quartas[i], quartas[i+1], "Quartas de Final", &v);
        if (v) semi[numSemi++] = v; /*Armazena o vencedor das quartas para as semifinais*/
    }
    /* Semifinais */
    cabecalho("SEMIFINAIS");
    Selecao *finalistas[4];
    int numFinalistas = 0;
    for (int i = 0; i + 1 < numSemi && numFinalistas < 4; i += 2) { /*CORRIGIDO: loop gerava so um vencedor*/
        Selecao *v = NULL;
        simularFaseEli(semi[i], semi[i+1], "Semifinal", &v);
        if (v) finalistas[numFinalistas++] = v; /*Armazena o vencedor das semifinais para a final*/
    }
    /* Final */
    if (numFinalistas >= 2) {
        cabecalho("GRANDE FINAL");
        Selecao *vencedor = NULL;
        simularFaseEli(finalistas[0], finalistas[1], "Final", &vencedor);
        if (vencedor)
            printf("\n  CAMPEAO DA COPA DO MUNDO FIFA 2026: %s!\n", vencedor->nome);
    }
    pausar();
}