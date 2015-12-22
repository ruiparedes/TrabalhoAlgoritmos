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
    char codigoPosta[20];
    char localidade[50];
    char contacto[10];
    DATA data_nascimento;

} ALUNO;

typedef struct {
    char codigo[15];
    char nome[50];
    char ano;
    char semestre[3];

} DISCIPLINA;

void pausa(char *msg) {
    printf("%s \n", msg);
    printf("%s \n", "Prima uma tecla para continuar");
    __fpurge(stdin);
    getchar();
}

int InserirAluno(char *ficheiro_alunos) {
    ALUNO a;
    FILE *f;

    printf("%s\n", "Inserir Aluno ");
    printf("%s\n", "C�digo? ");
    scanf("%i", &a.codigo);
    printf("%s\n", "Nome? ");
    scanf("%s", a.nome);
    printf("%s\n", "Data de Nascimento? ");
    scanf("%i-%i-%i", &a.data_nascimento.dia, &a.data_nascimento.mes, &a.data_nascimento.ano);
    printf("%s\n", "Email? ");
    scanf("%s", a.email);

    f = fopen(ficheiro_alunos, "a+b"); //a - append; + - cria se nao existir; b - binario
    fwrite(&a, sizeof (a), 1, f);
    fclose(f);
    printf("%s", "teste");

    return 1;

}

int AlterarAluno(char *ficheiro_alunos) {
    printf("%s \n", "InserirAluno: Em Elabora�ao");
    printf("%s \n", "Prima uma tecla para continuar");
    pausa("");
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


int ListarTodos(char *ficheiro_alunos) {

    ALUNO a;
    FILE *f;
    int i = 0;

    printf("%s\n", "Lista de Alunos");
    printf("%10s %-50s %-18s %5s \n", "Codigo", "Nome", "Data Nascimento", "Email");
    f = fopen(ficheiro_alunos, "rb");
    fseek(f, 0, SEEK_END);
    int nr = ftell(f) / sizeof (a); // Numero de registos
    fseek(f, 0, SEEK_SET);
    for (i = 0; i < nr; i++) {
        fread(&a, sizeof (a), 1, f);
        printf("%10i %-50s %02i-%02i-%04i %7s %-20s \n", a.codigo, a.nome
                , a.data_nascimento.dia, a.data_nascimento.mes, a.data_nascimento.ano, " ", a.email);
    }
    fwrite(&a, sizeof (a), 1, f);
    fclose(f);


    return 1;
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
    //ORDENA��O

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

void ImportarDisciplinas() {
    FILE *f;
    char linha[300];

    f = fopen("horario_disciplinas.csv", "rt"); // read , text(?)

    if (f == NULL) {
        printf("Erro!");
        return;
    }

    FILE *f2;
    f2 = fopen("DISCIPLINAS.DAT", "wb"); // append "+(criar)" binnary

    if (f2 == NULL) {
        printf("erro");
        return;
    }

    int codDisc;
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

void ImportarDados() {
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

void ImportarDocentes() {

    FILE *f;
    DISCIPLINA d;
    DISCIPLINA docentes[50];
    int nd = 0;
    char linha[300];
    setlocale(LC_ALL, "Portuguese");

    f = fopen("horario_disciplina.csv", "rt");
    if (f == NULL) {
        printf("erro");
        return;
    }
    FILE *f2;
    f2 = fopen("DOCENTES.DAT", "wb");
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

int main(int argc, char** argv) {
    char op;
    char *ficheiro_alunos = "ALUNOS.DAT";

    setlocale(LC_ALL, "Portuguese");

    ALUNO *alunos = 0;
    int numAlunos;
    alunos = LerAlunosVetor("ALUNOS.DAT", &numAlunos);

    do {
        system("clear");
        printf("%s\n", "Gest�o de Alunos");
        printf("%s\n", "1 - Inserir");
        printf("%s\n", "2 - Alterar");
        printf("%s\n", "3 - Eliminar");
        printf("%s\n", "4 - Listar Tudo");
        printf("%s\n", "5 - Pesquisa por Nome");
        printf("%s\n", "6 - Voltar");
        printf("%s\n", "n - Listar Tudo por nome");
        printf("%s\n", "0 - Terminar");

        scanf(" %c", &op);

        switch (op) {
            case '0': return 1;
                break;

            case '1': InserirAluno(ficheiro_alunos);
                break;

            case '4': ListarTodos(ficheiro_alunos);
                pausa(" ");
                break;
            case '7': ImportarDados();
                pausa("");
                break;
            case '8': ImportarDisciplinas();
                pausa("");
                break;
            case '9': ImportarDocentes();
                pausa("");
                break;

            case 'n':
                ListarPorNome(ficheiro_alunos);
                pausa("");
                break;

            default:
                break;
        }
    } while (1);

    return 0;
}

