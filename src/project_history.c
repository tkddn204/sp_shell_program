#include "smallsh.h"

#define HISTORY_FILE_NAME "/.history"
#define CAN_NOT_OPEN_HISTORY_FILE "can not open history file"

void helpPrint_history(); // 명렁어 사용 가이드
void hisroty_allPrint(FILE *); // 저장된 히스토리 모두 출력
void histroy_numPrint(FILE *, int); // 숫자로 받은 행의 히스토리 실행
int project_history(int, char **);

char *home;
char path[PATH_SIZE];
char prom[PROMPT_SIZE];


int project_history(int argc, char **argv) {

    FILE *fp = NULL;


    home = getenv("HOME");
    int n = 0;
    int history_num = 1;


    // 히스토리 저장파일 경로생성
    strcpy(path, home);
    strcat(path, "/.history");

    // 히스토리파일 유효성 검사
    fp = fopen(path, "r");
    if (fp == NULL) {
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

    // 옵션처리
    while ((n = getopt(argc, argv, "c")) != -1) {

        switch (n) {
            // 히스토리 파일 초기화
            case 'c':
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


    // 히스토리 숫자 조합의 입력시
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


void hisroty_allPrint(FILE *fp) {

    int count = 1;
    char buf[BUFSIZ];


    //히스토리 파일에서 모든 정보를 읽어 출력함
    while (fgets(buf, BUFSIZ, fp) != NULL) {
        printf("%d. ", count++);
        fputs(buf, stdout);
    }

    fclose(fp);

}

void histroy_numPrint(FILE *fp, int lineNum) {

    int i = 0;

    char buf[BUFSIZ];


    // lineNum을 통해 받은 히스토리 번호를 통해 파일에서 해당 행까지 이동
    for (i = 0; i <= lineNum - 2; i++) {
        if (!fgets(buf, BUF_SIZE, fp))
            printf("not exit num line");
    }


    if (fgets(buf, BUFSIZ, fp) != NULL) { // 목표한 행의 히스토리를 저장

        if (custom_userIn(buf) != EOF) { // 히스토리에서 가져온 명령어 처리
            procline();
        }


        return;
    }

    fclose(fp);
    printf("not exit num line \n");
}

void helpPrint_history() {
    (void) fprintf(stderr, "can not parse args - history [c]\n"); //stderr
}