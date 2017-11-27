#include "smallsh.h"

#define ALIAS_FILE_NAME "/.alias"

void command_parser(int *pid, int argc, char **cline) {
    if (pid == -1) {
        perror("failed fork");
        exit(1);
    } else if (pid > 0) { // parent program
        if (strcmp("cd", *cline) == 0)
            project_cd(argc, cline);
    } else if (pid == 0) { // child program
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
        else if (alias_check(cline[0]) == -1) {
            execvp(*cline, cline);
            perror(*cline);
            exit(127);
        }
        exit(0);
    }
}

int alias_check(char *cline) {

    char path[PATH_SIZE];
    char *home;
    FILE *fp = NULL;
    char buf[BUFSIZ];
    char cmd_buf[BUFSIZ];
    int i = 0;
    int j = 0;

    home = getenv("HOME");
    strcpy(path, home);
    strcat(path, ALIAS_FILE_NAME);
    fp = fopen(path, "r");

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