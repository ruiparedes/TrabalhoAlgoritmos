#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

typedef struct {
    int dia, mes, ano;
} DATA;

typedef struct {
    int codigo;
    char nome[50];
    char email[50];
    char morada[50];
    //   char codigoPostal[20];
    //  char localidade[50];
    char contacto[9];
    DATA data_nascimento;

} ALUNO;

typedef struct {
    int codigo;
    char nome[50];
    char email[50];
    char contacto[9];
    char disciplina[60];
} DOCENTE;

typedef struct {
    char codigo[15];
    char nome[50];
    int ano;
    char semestre[4];

} DISCIPLINA;

void pausa(char *msg) {
    printf("%s \n", msg);
    printf("%s \n", "Prima uma tecla para continuar");
    __fpurge(stdin);
    getchar();
    system("clear");
    //system(cls); -> windows
}

int InserirAluno(char *ficheiro_alunos) {
    ALUNO a;
    FILE *f;

    system("clear");
    //system("cls"); ->windows
    printf("%s\n", ".:: Inserir Aluno ::. \n ");
    printf("%s\n", "Codigo? ");
    scanf("%i", &a.codigo);
    __fpurge(stdin);
    //fflush(stdin); -> windows
    printf("%s\n", "Nome? ");
    scanf("%49[^\n]%*c", a.nome);
    printf("%s\n", "Data de Nascimento? ");
    scanf("%i-%i-%i", &a.data_nascimento.dia, &a.data_nascimento.mes, &a.data_nascimento.ano);
    __fpurge(stdin);
    printf("%s\n", "Morada? ");
    scanf("%49[^\n]%*c", a.morada);
    printf("%s\n", "Email? ");
    scanf("%s", a.email);
    printf("%s\n", "Contacto? ");
    scanf("%s", a.contacto);

    f = fopen(ficheiro_alunos, "a+b"); //a - append; + - cria se nao existir; b - binario
    fwrite(&a, sizeof (a), 1, f);
    fclose(f);

    return 1;
}

/* ################# INICIO DE VETORES ################*/

ALUNO *LerAlunosVetor(char *nome_ficheiro, int *N) {
    FILE *f;
    int i, n;
    ALUNO *alunos;

    f = fopen(nome_ficheiro, "rb");
    if (f == NULL) {
        printf("Error opening file : %s\n", strerror(errno));
        pausa("");
        return 0;
    }
    fseek(f, 0, SEEK_END); /* fim ficheiro */
    n = ftell(f) / sizeof (ALUNO);
    *N = n;
    fseek(f, 0, SEEK_SET); /* inicio ficheiro */

    if (n)
        alunos = (ALUNO *) malloc(n * sizeof (ALUNO)); /* reserva n elementos */
    else
        return 0;

    fread(alunos, sizeof (ALUNO), n, f); /* ler todos os alunos de uma s� vez */

    fclose(f);
    return alunos;
}

/* ################# FIM DE VETORES ##################*/


void ListarAlunos(char *ficheiro_alunos) {
    system("clear");
    //system("cls"); -> windows

    ALUNO a;
    FILE *f;
    int i = 0;

    printf("%s\n", "Lista de Alunos");
    printf("%10s %-50s %-18s %5s %35s %45s \n", "Codigo", "Nome", "Data Nascimento", "Email", "Morada", "Contacto");
    f = fopen(ficheiro_alunos, "rb");
    fseek(f, 0, SEEK_END);
    int nr = ftell(f) / sizeof (a); // Numero de registos
    fseek(f, 0, SEEK_SET);
    for (i = 0; i < nr; i++) {
        fread(&a, sizeof (a), 1, f);
        printf("%10i %-50s %02i-%02i-%04i %7s %-20s %13s %-43s %9s\n", a.codigo, a.nome
                , a.data_nascimento.dia, a.data_nascimento.mes, a.data_nascimento.ano, " ", a.email, " "
                , a.morada, a.contacto);
    }
    fwrite(&a, sizeof (a), 1, f);
    fclose(f);

}

void AlterarAlunoPorCodigo(char *nome_ficheiro) {
    system("clear");
    //system("cls"); -> windows

    ALUNO a;

    int codigo;
    printf("%s", "Codigo do aluno a alterar? ");
    scanf("%d", &codigo);

    FILE *f, *fp;
    int i = 0;

    f = fopen(nome_ficheiro, "r+b");
    fp = fopen("tmp.bin", "w+b");

    fseek(f, 0, SEEK_END);
    int nr = ftell(f) / sizeof (a); // Numero de registos
    fseek(f, 0, SEEK_SET);

    for (i = 0; i < nr; i++) {
        fread(&a, sizeof (a), 1, f);

        if (a.codigo == codigo) {
            __fpurge(stdin);
            //fflush(stdin); -> windows
            printf("%s\n", "Nome? ");
            scanf("%49[^\n]%*c", a.nome);
            printf("%s\n", "Data de Nascimento? ");
            scanf("%i-%i-%i", &a.data_nascimento.dia, &a.data_nascimento.mes, &a.data_nascimento.ano);
            __fpurge(stdin);
            printf("%s\n", "Morada? ");
            scanf("%49[^\n]%*c", a.morada);
            printf("%s\n", "Email? ");
            scanf("%s", a.email);
            printf("%s\n", "Contacto? ");
            scanf("%s", a.contacto);
            fwrite(&a, sizeof (a), 1, fp);
        } else {
            fwrite(&a, sizeof (a), 1, fp);
        }
    }
    fclose(f);
    fclose(fp);

    remove(nome_ficheiro);
    rename("tmp.bin", nome_ficheiro);
}

void EliminarAlunoPorCodigo(char *nome_ficheiro) {
    system("clear");
    //system("cls"); -> windows

    ALUNO a;

    printf(".:: Eliminar Aluno ::.\n");

    int codigo;
    printf("%s", "Codigo do aluno a eliminar? ");
    scanf("%d", &codigo);

    FILE *f, *fp;
    int i = 0;

    f = fopen(nome_ficheiro, "r+b");
    fp = fopen("tmp.bin", "w+b");

    fseek(f, 0, SEEK_END);
    int nr = ftell(f) / sizeof (a); // Numero de registos
    fseek(f, 0, SEEK_SET);

    for (i = 0; i < nr; i++) {
        fread(&a, sizeof (a), 1, f);

        if (a.codigo == codigo) {
            printf("\n Aluno de codigo % d removido!\n", codigo);
        } else {
            fwrite(&a, sizeof (a), 1, fp);
        }
    }
    fclose(f);
    fclose(fp);

    remove(nome_ficheiro);
    rename("tmp.bin", nome_ficheiro);
}

int ListarPorNome(char *ficheiro_alunos) {
    //ELABORADO POR RUI PAREDES Dia 14/12/2015
    ALUNO Alunos[1000];
    ALUNO a;
    FILE *f;
    int i = 0;


    f = fopen(ficheiro_alunos, "rb");
    fseek(f, 0, SEEK_END);
    int nr = ftell(f) / sizeof (a); // Numero de registos
    fseek(f, 0, SEEK_SET);
    for (i = 0; i < nr; i++) {
        fread(&a, sizeof (a), 1, f);
        memcpy(&Alunos[i], &a, sizeof (a) + 1);
    }
    fclose(f);
    //ORDENACAO

    for (i = 0; i < nr - 1; i++) {
        int posmenor = i;
        int j;
        for (j = i + 1; j < nr; j++) {
            if (strcmp(Alunos[j].nome,
                    Alunos[posmenor].nome) < 0) {
                posmenor = j;
            }
        }
        ALUNO aux;
        memcpy(&aux, &Alunos[i], sizeof (a));
        memcpy(&Alunos[i], &Alunos[posmenor], sizeof (a));
        memcpy(&Alunos[posmenor], &aux, sizeof (a));
    }
    //ESCRITA RESULTADOS
    printf("%s\n", "Lista de Alunos");
    printf(" %-50s %-20s\n", "Nome", "Email");
    for (i = 0; i < nr; i++) {
        printf(" %-50s %-20s \n", Alunos[i].nome, Alunos[i].email);
    }
    return 1;
}

void ListarDisciplinas(char *ficheiro_alunos) {
    system("clear");
    //system("cls"); -> windows

    DISCIPLINA disc;
    FILE *f;
    int i = 0;

    printf("%s\n", ".:: Lista de Disciplinas ::.");
    printf("%10s %-50s %4s %-10s", "Codigo", "Nome", "Ano", "Semestre");

    f = fopen(ficheiro_alunos, "rb");
    fseek(f, 0, SEEK_END);
    int nr = ftell(f) / sizeof (disc); // Numero de registos

    fseek(f, 0, SEEK_SET);
    for (i = 0; i < nr; i++) {
        fread(&disc, sizeof (disc), 1, f);
        printf("%-10s %-50s %4d %-10s", disc.codigo, disc.nome, disc.ano, disc.semestre);
    }

    fwrite(&disc, sizeof (disc), 1, f);
    fclose(f);

}

void ImportarDisciplinas() {
    system("clear");
    //system("cls"); -> windows
    
    FILE *f;
    char linha[300];

    f = fopen("horario_disciplinas.csv", "rt"); // read , text

    if (f == NULL) {
        printf("Erro!");
        return;
    }

    FILE *f2;
    f2 = fopen("DISCIPLINAS.DAT", "w+b"); // append "+(criar)" binnary

    if (f2 == NULL) {
        printf("erro");
        return;
    }

    DISCIPLINA dis;

    fgets(linha, 299, f);
    while (fgets(linha, 299, f) != NULL) {

        int c = 0, i, pos = 0;
        int n = strlen(linha);
        for (i = 0; i < n; i++) {
            if (linha[i] == ';') {
                pos = i;
                c++;
                if (c == 13) break;
            }
        }

        char aux[50];
        strcpy(linha, &linha[pos + 1]);

        sscanf(linha, "%[^\;];%[^\;]", dis.codigo, dis.nome);

        printf("%s %s\n", dis.codigo, dis.nome);
        strcpy(linha, "");
        dis.ano = 1;
        strcpy(dis.semestre, "S1");
        fwrite(&dis, sizeof (dis), 1, f2);
    }

    fclose(f);
    fclose(f2);


}

void ImportarDadosAlunos() {

    system("clear");
    // system("cls"); . windows

    FILE *f;
    char linha[300];

    f = fopen("dados_alunos.csv", "rt");

    if (f == NULL) {
        printf("Erro!");
        return;
    }

    int CdAluno;
    char NmAluno[50];

    int d, m, a;
    char email[100];
    fgets(linha, 299, f);

    while (fgets(linha, 299, f) != NULL) {
        //  printf("%s\n", linha);
        sscanf(linha, "%d;%[^\;];%d-%d-%d;%s", &CdAluno, NmAluno, &d, &m, &a, email);
        printf("%i %s %d-%d-%d %s\n", CdAluno, NmAluno, d, m, a, email);
    }

    //a.CdAluno = CdAluno;
    // strcpy(a.NmAluno, NmAluno);


    fclose(f);
}

void EliminarDocentePorCodigo(char *nome_ficheiro) {
    system("clear");
    //system("cls"); -> windows

    DOCENTE d;

    int codigo;
    printf(".:: Eliminar Docente ::.\n");
    printf("%s", "Codigo do docente a eliminar? ");
    scanf("%d", &codigo);

    FILE *f, *fp;
    int i = 0;

    f = fopen(nome_ficheiro, "r+b");
    fp = fopen("tmp.bin", "w+b");

    fseek(f, 0, SEEK_END);
    int nr = ftell(f) / sizeof (d); // Numero de registos
    fseek(f, 0, SEEK_SET);

    for (i = 0; i < nr; i++) {
        fread(&d, sizeof (d), 1, f);

        if (d.codigo == codigo) {
            printf("\n Docente de codigo % d removido!\n", codigo);
        } else {
            fwrite(&d, sizeof (d), 1, fp);
        }
    }
    fclose(f);
    fclose(fp);

    remove(nome_ficheiro);
    rename("tmp.bin", nome_ficheiro);
}

int InserirDocente(char *ficheiro_docentes) {
    DOCENTE d;
    FILE *f;

    system("clear");
    //system("cls"); -> windows

    printf("%s\n", ".:: Inserir Docente ::. \n");
    printf("%s\n", "Codigo? ");
    scanf("%d", &d.codigo);
    __fpurge(stdin);
    //fflush(stdin); -> windows
    printf("%s\n", "Nome? ");
    scanf("%49[^\n]%*c", d.nome);
    printf("%s\n", "Disciplina? ");
    scanf("%49[^\n]%*c", d.disciplina);
    printf("%s\n", "Email? ");
    scanf("%s", d.email);
    printf("%s\n", "Contacto? ");
    scanf("%s", d.contacto);

    f = fopen(ficheiro_docentes, "a+b"); //a - append; + - cria se nao existir; b - binario
    fwrite(&d, sizeof (d), 1, f);
    fclose(f);
}

void ListarDocentes(char *ficheiro_docentes) {
    system("clear");
    //system("cls"); -> windows

    DOCENTE d;
    FILE *f;
    int i = 0;

    printf("%s\n", ".:: Lista de Docentes ::. \n");
    printf("%10s %-50s %-18s %-50s %-50s %-10s", "Codigo", "Nome", "Disciplina", "Email", "Contacto");

    f = fopen(ficheiro_docentes, "rb");
    fseek(f, 0, SEEK_END);
    int nr = ftell(f) / sizeof (d); // Numero de registos
    fseek(f, 0, SEEK_SET);

    for (i = 0; i < nr; i++) {
        fread(&d, sizeof (d), 1, f);
        printf("%d %50s %50s %50s %10s", d.codigo, d.nome, d.disciplina, d.email, d.contacto);
    }

    fwrite(&d, sizeof (d), 1, f);
    fclose(f);
}

void ImportarDocentes(char* ficheiro_docentes) {

    FILE *f;
    DISCIPLINA d;
    DISCIPLINA docentes[50];
    int nd = 0;
    char linha[300];

    f = fopen("horario_disciplina.csv", "rt");
    if (f == NULL) {
        printf("erro");
        return;
    }
    FILE *f2;
    f2 = fopen(ficheiro_docentes, "wb");
    if (f2 == NULL) {
        printf("erro");
        return;
    }
    //data_id;CdTurma;DgTurma;DiaSemana;HoraIni;MinutoIni;HoraFim;MinutoFim;CdRegime;DgRegime;NmDocente;Sala;CdDocente;CdDis;NmDis;CdSala;CdPEstudo;AbrDis;CdCampus;CdEdificio;CdPiso
    //42338; 25; T1; 4; 11; 0; 13; 0; 3; Te�rico - Pr�ticas; Ant�nio M�rio Ribeiro Martins; 59; 256; 56C1024; Sistemas Digitais II; 33; 2; SD II;;;
    fgets(linha, 299, f);
    while (fgets(linha, 299, f) != NULL) {
        //
        int c = 0;
        int n = strlen(linha);
        int pos = 0;
        int i;
        for (i = 0; i < n; i++) {
            if (linha[i] == ';') {
                pos = i;
                c++;
                if (c == 10)
                    break;
            }
        }

        strcpy(linha, &linha[pos + 1]);
        //printf("%s\n", linha);
        char aux[20];

        sscanf(linha, "%[^\;];%[^\;];%d", d.nome, aux, &d.codigo);
        //printf("%d %s %s\n", d.CdDoc,aux, d.NmDoc);
        int e = 0;
        for (i = 0; i < nd; i++) {
            if (docentes[i].codigo == d.codigo) {
                e = 1;
                break;
            }
        }
        if (e == 0) {
            docentes[nd++] = d;
        }

    }
    int i;
    fclose(f);
    for (i = 0; i < nd; i++) {
        printf("%d %s\n", docentes[i].codigo, docentes[i].nome);
    }

    fwrite(docentes, sizeof (d), nd, f2);
    fclose(f2);

    return;
}

void MenuAlunos() {
    char op;
    char *ficheiro_alunos = "ALUNOS.DAT";

    
    remove("ALUNOS.DAT");
    system("cp ALUNOS_BACKUP.DAT ALUNOS.DAT");
      
    
    ALUNO *alunos = 0;
    int numAlunos;

    alunos = LerAlunosVetor("ALUNOS.DAT", &numAlunos);

    do {
        system("clear");
        printf("%s\n", ".:: ALUNOS ::.\n");
        printf("%s\n", "- 1 - Importar");
        printf("%s\n", "- 2 - Inserir");
        printf("%s\n", "- 3 - Alterar");
        printf("%s\n", "- 4 - Eliminar");
        printf("%s\n", "- 5 - Listar Tudo");
        printf("%s\n", "- 6 - Pesquisar");
        printf("%s\n", "- 7 - Resumir");
        printf("%s\n", "- 8 - Agrupar");
        printf("%s\n", "- 9 - Contabilizar");
        printf("%s\n", "- A - Gerar Horario A4");
        printf("%s\n", "- B - Gerar Horario Cartao de Credito");
        printf("%s\n", "- C - Classificar Qualidade de Horarios");
        printf("%s\n", "- 0 - Menu Principal");

        scanf(" %c", &op);

        switch (op) {
            case '0':
                MenuPrincipal();
                pausa("");
                break;
            case '1':
                ImportarDadosAlunos();
                pausa("");
                break;
            case '2':
                InserirAluno(ficheiro_alunos);
                pausa("");
                break;
            case '3':
                AlterarAlunoPorCodigo(ficheiro_alunos);
                pausa("");
                break;
            case '4':
                EliminarAlunoPorCodigo(ficheiro_alunos);
                pausa("");
                break;
            case '5': ListarAlunos(ficheiro_alunos);
                pausa(" ");
                break;
            case '7':;
                pausa("");
                break;
            case 'n': // PARA ALTERAR
                ListarPorNome(ficheiro_alunos);
                pausa("");
                break;
            default:
                break;
        }
    } while (1);

}

void MenuDocentes() {
    char op;

    char *ficheiro_docentes = "DOCENTES.DAT";
    //remove("DOCENTES.DAT");

    do {
        system("clear");
        // system("cls") . windows
        printf("%s\n", ".:: DOCENTES ::. \n");
        printf("%s\n", "- 1 - Inserir");
        printf("%s\n", "- 2 - Eliminar");
        printf("%s\n", "- 3 - Listar Tudo");
        printf("%s\n", "- 4 - Importar");
        printf("%s\n", "- 0 - Menu Principal");

        scanf(" %c", &op);

        switch (op) {
            case '0':
                MenuPrincipal();
                pausa("");
                break;

            case '1':
                InserirDocente(ficheiro_docentes);
                pausa("");
                break;

            case '2':
                EliminarDocentePorCodigo(ficheiro_docentes);
                pausa("");
                break;

            case '3':
                ListarDocentes(ficheiro_docentes);
                pausa("");
                break;

            case '4':
                ImportarDocentes(ficheiro_docentes);
                pausa("");
                break;

            default:
                break;
        }
    } while (1);
}

void MenuDisciplinas() {
    char op;

    char *ficheiro_disciplinas = "DOCENTES.DAT";


    do {
        system("clear");
        // system("cls") . windows
        printf("%s\n", ".:: DISCIPLINAS ::.\n");
        printf("%s\n", "- 1 - Inserir");
        printf("%s\n", "- 2 - Eliminar");
        printf("%s\n", "- 3 - Listar");
        printf("%s\n", "- 4 - Importar Disciplinas");
        printf("%s\n", "- 0 - Menu Principal");

        scanf(" %c", &op);

        switch (op) {
            case '0':
                MenuPrincipal();
                pausa("");
                break;
            case '1':
                break;
            case '3':
                ListarDisciplinas(ficheiro_disciplinas);
                pausa("");
                break;
            case '4':
                ImportarDisciplinas();
                pausa("");
                break;


            default:
                break;
        }
    } while (1);
}

void MenuPrincipal() {
    char op;

    setlocale(LC_ALL, "Portuguese");

    do {
        system("clear");
        // system("cls") . windows
        printf("%s\n", ".:: MENU PRINCIPAL ::.\n");
        printf("%s\n", "- 1 - Alunos");
        printf("%s\n", "- 2 - Docentes");
        printf("%s\n", "- 3 - Disciplinas");
        printf("%s\n", "- 4 - Horario das Disciplinas");
        printf("%s\n", "- 5 - Disciplinas do Aluno");
        printf("%s\n", "- 0 - Sair");

        scanf(" %c", &op);

        switch (op) {
            case '0':
                exit(0);
            case '1':
                MenuAlunos();
                pausa("");
                break;
            case '2':
                MenuDocentes();
                pausa("");
                break;
            case '3':
                MenuDisciplinas();
                pausa("");
            default:
                break;
        }
    } while (1);
}

int main(int argc, char** argv) {

    MenuPrincipal();
    pausa("");
    return 0;
}

