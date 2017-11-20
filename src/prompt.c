#include "smallsh.h"

void prompt(char *prom) {
    char* current_cwd = getcwd(NULL, 0);
    char* home_cwd = getenv("HOME");
    char print_cwd[CWD_SIZE];
    int home_dir_length = 0;
    int i, j = 0;

    strcpy(prom, "hbl-Command[");

    while(home_cwd[home_dir_length++] != '\0');
    if(memcmp(current_cwd, home_cwd, home_dir_length - 1) == 0) {
        print_cwd[j++] = '~';
        for(i = home_dir_length  - 1;
            current_cwd[i] != '\0';
            i++, j++) {
            print_cwd[j] = current_cwd[i];
        }
        print_cwd[j] = '\0';
    } else {
        strcpy(print_cwd, current_cwd);
    }
    strcat(prom, print_cwd);
    strcat(prom, "]>");
}