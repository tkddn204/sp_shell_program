#include "smallsh.h"

int main(int argc, char *argv[])
{
    char prom[PROMPT_SIZE];
    prompt(prom);
    while(userin(prom) != EOF) {
        procline();
        prompt(prom);
    }
    return 0;
}
