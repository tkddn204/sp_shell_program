#include "smallsh.h"

#include <time.h>

void prompt(char *prom) {
    // getcwd로 현재 디렉토리 문자열을 얻음
    char* current_cwd = getcwd(NULL, 0);

    // getenv("HOME")으로 환경변수에서 홈 디렉토리
    // 문자열을 얻음
    char* home_cwd = getenv("HOME");

    char print_cwd[CWD_SIZE];
    int home_dir_length = 0;
    
    // 시간 출력을 위한 time 구조체
    time_t timer;
    struct tm *t;
    int hour;
    char* mid;

    int i, j = 0;

    // time 함수를 이용하여 현재 시간을 구함
    timer = time(NULL);
    
    // localtime 함수로 현재 시간의 정보를 구조체에 삽입
    t = localtime(&timer);

    // AM, PM 처리 로직
    if(t->tm_hour < 12) {
        mid = "AM";
        hour = t->tm_hour;
    } else {
        mid = "PM";
        hour = t->tm_hour - 12;
    }

    // 시간 정보를 문자열에 삽입
    sprintf(prom, "InfoSH(%2d.%2d, %s %02d:%02d)[",
            t->tm_mon+1, t->tm_mday, mid, hour, t->tm_min);
    
    // 홈 디렉토리의 길이를 알아냄
    while(home_cwd[home_dir_length++] != '\0');

    // 현재 절대 경로에서 홈 디렉토리 경로의 길이까지를
    // '~' 문자로 변환하는 로직
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
    
    // 최종적으로 패스를 프롬프트에 append
    strcat(prom, print_cwd);
    strcat(prom, "]>");
}