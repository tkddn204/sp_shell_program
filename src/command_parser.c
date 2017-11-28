#include "smallsh.h"

#define ALIAS_FILE_NAME "/.alias"

// 명령어 해석을 진행하는 부분
void command_parser(int pid, int argc, char **cline) {
    if (pid == -1) {
        perror("failed fork");
        exit(1);
    } else if (pid > 0) { // 부모 프로세스인 경우
        if (strcmp("cd", *cline) == 0)
            project_cd(argc, cline);
    } else if (pid == 0) { // 자식 프로세스인 경우
        if (strcmp("exit", *cline) == 0) {
            kill(getppid(), SIGINT);
        } else if (strcmp("history", *cline) == 0)
            project_history(argc, cline);
        else if (strcmp("alias", *cline) == 0)
            project_alias(argc, cline);
        else if (strcmp("ps", *cline) == 0)
            project_ps(argc, cline);
        else if (strcmp("df", *cline) == 0)
            project_df(argc, cline);
        else if (strcmp("du", *cline) == 0)
            project_du(argc, cline);
        // alias 명령어 처리 로직 실행
        else if (alias_check(cline[0]) == -1) {
            execvp(*cline, cline);
            perror(*cline);
            exit(127);
        }
        else if (strcmp("cd", *cline) == 0) {
            // nothing
        }
        exit(0);
    }
}

// alias의 파일을 불러와서
// alias에 등록된 명령어인지 체크하는 부분
int alias_check(char *cline) {

    char path[PATH_SIZE];
    char *home;
    FILE *fp = NULL;
    char buf[BUFSIZ];
    char cmd_buf[BUFSIZ];
    int i = 0;
    int j = 0;

    // getenv로 홈디렉토리를 불러온 뒤
    // .alias 파일을 불러옴
    home = getenv("HOME");
    strcpy(path, home);
    strcat(path, ALIAS_FILE_NAME);
    fp = fopen(path, "r");


    // alias 파일에서 정보를 읽어와 일치하는 명령어를 찾음
    while (fgets(buf, BUFSIZ, fp) != NULL) {
        for (i = 0; buf[i] != '='; i++);
        buf[i] = '\0';

        if (!strcmp(buf, cline)) {

            i = i + 2;

            while (buf[i] != '\0') {
                cmd_buf[j++] = buf[i++];
            }

            cmd_buf[j - 2] = '\n';
            cmd_buf[j - 1] = '\0';

            if (custom_userIn(cmd_buf) != EOF) {
                procline();
            }
            return 0;
        }
    }
    return -1;
}