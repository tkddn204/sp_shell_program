#include "smallsh.h"

// new constant value
#define EOL 1 /* End Of Line */
#define ARG 2 /* normal argument */
#define AMPERSAND 3
#define SEMICOLON 4
#define PIPE 5
#define REDIRECTION_LEFT 6
#define REDIRECTION_RIGHT 7

static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr, *tok;
static char special[] = {' ', '\t', '&', ';', '\n', '\0', '|', '<', '>'};

/* 프롬프트(prompt)를 인쇄하고 키보드에서 한 줄의 */
/* 입력이 들어오기를 기다린다. 받은 입력은 무엇이건 */
/* 프로그램 버퍼에 저장한다. */
int userin(char *p) {
    FILE *fp;
    char *home = getenv("HOME");
    char path[PATH_SIZE];
    int c, count;

    strcpy(path, home);
    strcat(path, HISTORY_FILE_NAME);

    if ((fp = fopen(path, "a")) == NULL) {
        perror("history file open");
    }

    /* initialization for later routines */
    ptr = inpbuf;
    tok = tokbuf;
    /* display prompt */
    printf("%s ", p);
    count = 0;
    while (1) {
        if ((c = getchar()) == EOF) {
            fclose(fp);
            return EOF;
        }
        if (count < MAXBUF) inpbuf[count++] = c;
        if (c == '\n' && count < MAXBUF) {
            inpbuf[count] = '\0';
            break;
        }
        /* if line too long, restart */
        if (c == '\n') {
            printf(" smallsh : input line too long\n");
            count = 0;
            printf("%s ", p);
        }
    }

    if (count == 1 && inpbuf[count - 1] == '\n') {
        fclose(fp);
        return count;
    }

    fputs(inpbuf, fp);
    fclose(fp);
    return count;
}

int custom_userIn(char *command) {
    FILE *fp;
    char *home = getenv("HOME");
    char path[PATH_SIZE];
    int c, count;
    int n = 0;

    strcpy(path, home);
    strcat(path, HISTORY_FILE_NAME);

    if ((fp = fopen(path, "a")) == NULL) {
        perror("history file open");
    }

    /* initialization for later routines */
    ptr = inpbuf;
    tok = tokbuf;
    /* display prompt */

    count = 0;
    while (1) {
        if ((c = command[n++]) == '\0') {
            fclose(fp);
            return EOF;
        }
        if (count < MAXBUF) inpbuf[count++] = c;
        if (c == '\n' && count < MAXBUF) {
            inpbuf[count] = '\0';
            break;
        }
    }
//    fputs(inpbuf, fp);
    fclose(fp);
    return count;
}
/* get token and place into tokbuf */
int gettok(char **outptr)
{
    int type;
    /* outptr 문자열을 tok 로 지정한다 */
    *outptr = tok;
    /* 토큰을 포함하고 있는 버퍼로부터 여백을 제거한다 */
    while( *ptr == ' ' || *ptr == '\t') ptr++;
    /* 토큰 포인터를 버퍼 내의 첫 토큰으로 지정한다 */
    *tok++ = *ptr;
    /* 버퍼내의 토큰에 따라 유형 변수를 지정한다 */
    switch(*ptr++) {
        case '\n' : type = EOL;
        /* printf(" type == EOL getok()\n"); */
        break;
        case '&' : type = AMPERSAND;
        /* printf(" type == AMPERSAND getok()\n"); */
        break;
        case ';' : type = SEMICOLON;
        /* printf(" type == SEMICOLON getok()\n"); */
        break;
        case '|' : type = PIPE;
        /* printf(" type == PIPE getok()\n"); */
        break;
        case '<' : type = REDIRECTION_LEFT;
        /* printf(" type == REDIRECTION_LEFT getok()\n"); */
        break;
        case '>' : type = REDIRECTION_RIGHT;
        /* printf(" type == REDIRECTION_RIGHT getok()\n"); */
        break;
        default : type = ARG;
        /* printf(" type == ARG getok()\n"); */
        while(inarg(*ptr)) *tok++ = *ptr++;
        break;
    }
    *tok++ = '\0';
    return type;
}

/* are we in an ordinary argument */
int inarg(char c)
{
    char *wrk;
    for(wrk = special; *wrk != '\0'; wrk++) {
        if (c == *wrk) {
            //printf(" special arg : %c inarg()\n", *wrk);
            return 0;
        }
    }
    return 1;
}

/* 입력 줄을 아래와 같이 처리한다 : */
/* */
/* gettok을 이용하여 명령문을 구무분석(parse) 하고 */
/* 그 과정에서 인수의 목록을 작성한다. 개행문자나 */
/* 세미콜론(;)을 만나면 명령을 수행하기 위해 */
/* runcommand라 불리는 루틴을 호출한다. */
void procline()
{
    char *arg[MAXARG+1]; /* runcommand를 위한 포인터 배열 */
    int toktype; /* 명령내의 토근의 유형 */
    int narg; /* 지금까지의 인수 수 */
    int type; /* FOREGROUND or BACKGROUND */
    int special_type = 0; /* PIPE or REDIRECTION */
    /* 토큰 유형에 따라 행동을 취한다. */
    for (narg = 0;;) { /* loop FOREVER */
        switch(toktype = gettok(&arg[narg])) {
            case ARG :
                if (narg < MAXARG) narg++;
                break;
            case EOL :
            case SEMICOLON :
            case AMPERSAND :
                type = (toktype == AMPERSAND) ?
                    BACKGROUND : FOREGROUND;
                if (narg != 0) {
                    arg[narg] = NULL;
                    if(special_type == 0) {
                        runcommand(narg, arg, type);
                    } else if(special_type == PIPE) {
                        runcommand_pipe(narg, arg, type);
                    } else {
                        runcommand_redirection(narg, arg, type);
                    }
                }
                if (toktype == EOL) return;
                narg = 0;
                break;
            case PIPE :
            case REDIRECTION_LEFT :
            case REDIRECTION_RIGHT :
                special_type = toktype;
                break;
        }
    }
}
/* wait를 선택사항으로 하여 명령을 수행한다. */
/* 명령을 수행하기 위한 모든 프로세스를 시작하게 한다. */
/* 만일 where가 smallsh.j에서 정의된 값 BACKGROUND로 */
/* 지정되어 있으면 waitpid 호출은 생략되고 runcommand는 */
/* 단순히 프로세스 식별번호만 인쇄하고 복귀한다. */
int runcommand(int argc, char **cline, char where)
{
    int pid; // , exitstat, ret;
    int status;
    int pr_code;

    if ((pid = fork()) < 0) {
        perror("smallsh");
        return -1;
    }
    
    command_parser(pid, argc, cline);
    
    /* code for parent */
    /* if background process, print pid and exit */
    if (where == BACKGROUND) {
        printf("[Process id %d]\n",pid);
        return 0;
    }
    /* 프로세스 pid가 퇴장할 때까지 기다린다. */
    if (waitpid(pid, &status, 0) == -1)
        return -1;
    else
        return status;
}