#include "main.h"

char *prompt = "Command>"; /* Prompt */

int main(int argc, char *argv[])
{
    while(userin(prompt) != EOF);
    procline();
    return 0;
}
