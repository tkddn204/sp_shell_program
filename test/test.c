#include <stdio.h>
#include <stdlib.h>
 
#define CWD_SIZE 144

// 단위 테스트를 진행
int main() {
    char prom[CWD_SIZE];
    char* buf_cwd;
    int i;
    int buf_length = 0;

    buf_cwd = getcwd(NULL, 0);
    while(buf_cwd[buf_length++] != '\0');
    printf("%s : %d\n", buf_cwd, buf_length);
    return 0;
}