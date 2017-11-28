#include "smallsh.h"

int project_export(int argc, char **argv) {
    int n;
    char *path; // environment value path
    if(argc > 3 || argc < 3) {
        printf("\n\nUsage : export [-grs]\n\n");
        exit(1);
    }
    while((n = getopt(argc,argv,"sgr"))!=-1) { //option
        switch(n) {
            case 's':
                if(putenv(argv[2])==-1) { // add environment value
                    printf("\n\nputenv failed\n\n");
                    exit(1);
                }
                printf("\n\nputenv success\n\n");
                break;
            case 'g':
                if((path=getenv(argv[2]))==NULL) { //get environment value
                    printf("\ngetenc failed\n\n");
                    exit(1);
                }
                printf("\n\n%s\n\n",path);
                break;
            case 'r':
                if(unsetenv(argv[2])==-1) { //remove environment value
                    printf("\n\nunsetenv failed\n\n");
                    exit(1);
                }
                printf("\n\nunsetenv success\n\n");
                break;
            case '?':
                break;
            default:
                printf("\n\nUsage : export [-grs]\n\n");
                exit(1);
        }
    } 
    return 0;
}


// int main(int argc,char **argv) {
//     project_export(argc, argv);
//     return 0;
// }