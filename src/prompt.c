#include "smallsh.h"

#include <time.h>

void prompt(char *prom) {
    char* current_cwd = getcwd(NULL, 0);
    char* home_cwd = getenv("HOME");
    char print_cwd[CWD_SIZE];
    int home_dir_length = 0;
    
    time_t timer;
    struct tm *t;
    int hour;
    char* mid;

    int i, j = 0;

    timer = time(NULL);
    t = localtime(&timer);

    if(t->tm_hour < 12) {
        mid = "AM";
        hour = t->tm_hour;
    } else {
        mid = "PM";
        hour = t->tm_hour - 12;
    }

    sprintf(prom, "InfoSH(%2d.%2d, %s %02d:%02d)[",
            t->tm_mon+1, t->tm_day, mid, hour, t->tm_min);

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