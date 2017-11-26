#include "smallsh.h"
#include <memory.h>
#include <wchar.h>


#define ALIAS_FILE_NAME "/.alias"
#define TMP_ALIAS_FILE_NAME "/.alias_tmp"

#define DO_NOT_WRITE_ARGS "please write args"
#define CAN_NOT_OPEN_ALIAS_FILE "can not open alias file"

void helpPrint_alias();

int alias_addAlias(FILE *, char *);

void alias_allPrint(FILE *);

void alias_unAlias(FILE *, char *);

int uflag;

char path[PATH_SIZE];
char prom[PROMPT_SIZE];
char *home;

int project_alias(int argc, char **argv) {

    FILE *fp = NULL;
    home = getenv("HOME");
    int n = 0;
    int optcount = 0;
    char add_tmp[BUFSIZ];

    memset(add_tmp, 0, BUFSIZ);

    strcpy(path, home);
    strcat(path, ALIAS_FILE_NAME);


    while ((n = getopt(argc, argv, "cu")) != -1) {

        switch (n) {
            case 'c':  // history clear
                fp = fopen(path, "w");
                printf("alias clear \n");
                if (fp == NULL) {
                    perror("alias clear fail");
                }
                fclose(fp);
                return 1;

            case 'u':
                uflag = 1;
                optcount++;
                break;

            case '?': // can
            default:
                helpPrint_alias();
                fclose(fp);
                return 0;
        }
    }


    if (argc == 2 && uflag == 1) {
        helpPrint_alias();
        return 0;
    }

    if (argc >= 2 && uflag == 0) { // 명령어추가

        int iter = argc - 1;
        int j = 0;

        strcpy(add_tmp, argv[1]);
        strcat(add_tmp, " ");

        for (j = 2; j <= iter; j++) {
            strcat(add_tmp, argv[j]);
            strcat(add_tmp, " ");

        }
        fp = fopen(path, "a");
        alias_addAlias(fp, add_tmp);


    } else if (argc == 1) { //명령어 출력
        fp = fopen(path, "r");
        printf("alias print \n");
        alias_allPrint(fp);

    } else if (argc == 3 && uflag == 1) {  //명령어 삭
        fp = fopen(path, "r");
        printf("alias rm \n");
        alias_unAlias(fp, argv[2]);

    }
    fclose(fp);
    return 1;
}

void helpPrint_alias() {
    perror(stderr, "can not parse args - alias [c], [u] OR alias_command='command' \n"); //stderr
    exit(22);
}

void alias_allPrint(FILE *fp) {
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, fp) != NULL) {
        fputs(buf, stdout);
    }


}

int alias_addAlias(FILE *fp, char *alias) {

    int i = 0;
    int count = 0;
    int count2 = 0;
    int equalFlag = 0;
    int dotflag = 0;
    char *command = NULL;
    char buf[BUFSIZ];
    char alias_command[BUFSIZ];
    char real_command[BUFSIZ];

    //검사를 위해 읽기전용으로 파일생성자 생성
    FILE *rfp = fopen(path, "r");

    command = alias;
    strcat(command, "\0");


    //유효성체크 - 현재 '=' 값 체크
    while (command[count] != '\0') {
        if (command[count] == '=') {
            equalFlag = 1;
        }
        count++;
    }

    if (equalFlag == 0) {
        helpPrint_alias();
        return -1;
    }




    //alias 명령어 저장
    count = 0;

    while (command[count] != '=') {
        alias_command[count] = command[count];
        count++;
    }

    alias_command[count] = '\0';
    count++;

    //alias 명령어 중복 검사
    while (fgets(buf, BUFSIZ, rfp) != NULL) {

        for (i = 0; buf[i] != '='; i++);
        buf[i] = '\0';


        if (!strcmp(buf, alias_command)) {
            printf("duplication alias command \n");
            return -1;
        }
    }


    //작은따옴표체크
    if (command[count] == '\'') {
        dotflag = 1;
        count++;
    }




    // alias 실제명령어 저장
    while (command[count] != '\0') {
        real_command[count2++] = command[count++];

    }

    real_command[count2] = '\0';



    //작은따옴표 및 명령어 끝부분처리
    if (real_command[count2 - 2] == '\'' && dotflag == 1)
        real_command[count2 - 2] = '\0';

    else if (real_command[count2 - 2] != '\'' && dotflag == 1) {
        helpPrint_alias();
        return -1;
    } else if (dotflag == 0) {
        real_command[count2 - 1] = '\0';

    }

//    printf("실제 명령어 체크 %s \n", real_command);

    strcat(alias_command, "=\'");
    strcat(alias_command, real_command);
    strcat(alias_command, "\'\n");
    //명령어 파일에 실제 저장
    fputs(alias_command, fp);


    return 1;
}


void alias_unAlias(FILE *fp, char *alias) {

    int i = 0;
    char buf[BUFSIZ];
    int line = 1;
    int findFlag = 0;
    char tmpPath[BUFSIZ];


    strcat(alias, "\0");

    //임시파일 경로생성
    strcpy(tmpPath, home);
    strcat(tmpPath, TMP_ALIAS_FILE_NAME);


    FILE *nfp = fopen(tmpPath, "w"); //새로 쓸 파일
    if (nfp == NULL) {
        perror("delete fail");
        exit(1);
    }


    // 제거할 명령어 라인 검사
    while (fgets(buf, BUFSIZ, fp) != NULL) {

        for (i = 0; buf[i] != '='; i++);

        buf[i] = '\0';

        if (!strcmp(buf, alias)) {
            findFlag = 1;
            break;
        }

        line++;
    }

    if (findFlag == 0) {
        printf("not found alias command\n");
        return;
    }


    fseek(fp, 0, SEEK_SET);


    //임시 파일에 삭제할 명령어 제외하고 복사
    i = 1;
    while (fgets(buf, BUFSIZ, fp) != NULL) {
        if (line != i) {
            fputs(buf, nfp);
        }
        i++;
    }

    //원본 삭제 및 임시파일 원본으로 변환
    unlink(path);
    rename(tmpPath, path);


    fclose(nfp);
}