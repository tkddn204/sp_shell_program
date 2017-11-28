#include "smallsh.h"

int main(int argc, char *argv[])
{
    char prom[PROMPT_SIZE];

    prompt(prom); // 프롬프트 초기화
    
    while(userin(prom) != EOF) { // 문자열 버퍼 입력
        procline(); // 문자열 처리 루틴
        prompt(prom); // 프롬프트 재생성
    }
    return 0;
}
