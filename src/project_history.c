#include "smallsh.h"

#define CAN_NOT_OPEN_HISTORY_FILE "can not open history file"

void helpPrint_history(); // 명렁어 사용 가이드
void hisroty_allPrint(FILE *); // 저장된 히스토리 모두 출력
void histroy_numPrint(FILE *, int); // 숫자로 받은 행의 히스토리 실행
int project_history(int, char **);

char *home;
char path[PATH_SIZE];

char prom[PROMPT_SIZE];


int project_history(int argc, char **argv) {

    FILE *fp;

    home = getenv("HOME");
    int n = 0;
    int history_num = 1;

    strcpy(path, home);
    strcat(path, HISTORY_FILE_NAME);
    
    if ((fp = fopen(path, "r")) == NULL) {
        perror("history file read");
    }

    if (argc > 2) {
        helpPrint_history();
        return 0;
    }

    if (argc == 1) {
        hisroty_allPrint(fp);
        return 1;
    }

    while ((n = getopt(argc, argv, "c")) != -1) {
        switch (n) {
            case 'c':  // history clear
                fp = fopen(path, "w");
                printf("history clear \n");
                if (fp == NULL) {
                    perror("history clear fail");
                }
                fclose(fp);
                return 1;

            case '?': // can
            default:
                helpPrint_history();
                return 0;
        }
    }


    if (argc == 2) {
        history_num = atoi(argv[1]);

        if (history_num == 0) {   // 히스토리 + 숫자가아닌 다른 문자입력시 예외처리
            helpPrint_history();
            return 0;
        }
        histroy_numPrint(fp, history_num);
    }
    return 1;
}

void helpPrint_history() {
    perror("can not parse args - history [c]\n");
    exit(22);
}

void hisroty_allPrint(FILE *fp) {

    int count = 1;
    char buf[BUFSIZ];

    while (fgets(buf, BUFSIZ, fp) != NULL) {
        printf("%d. ", count++);
        fputs(buf, stdout);
    }

    fclose(fp);

}

void histroy_numPrint(FILE *fp, int lineNum) {

    int i = 0;
    int n = 0;
    char buf[BUFSIZ];


    for (i = 0; i <= lineNum - 2; i++) {
        if (!fgets(buf, BUF_SIZE, fp))
            printf("not exit num line");
    }

    if (fgets(buf, BUFSIZ, fp) != NULL) {
//        for (n = 0;; n++) {
//            if (buf[n] == '\0') {
//                buf[n] = '\n';
//                buf[n + 1] = '\0';
//                break;
//            }
//        }
        fputs(buf, stdin);
        if (custom_userIn(buf) != EOF) {
            procline();
        }
        return;
    }

    fclose(fp);
    printf("not exit num line \n");
}