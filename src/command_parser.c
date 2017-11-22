#include "smallsh.h"

// return 1 -> parent program failed
// return 0 -> child program success;
// return -1 -> not found, call execvp;
int command_parser(int pid, int argc, char **cline) {
    // parent program
    if(pid > 0) {
        if(strcmp("cd", *cline) == 0)
            project_cd(argc, cline);
        else
            return 1;
    } else if(pid == 0) {
        if(strcmp("exit", *cline) == 0) {
            kill(getppid(), SIGINT);
            exit(0);
        }
        else if(strcmp("history", *cline) == 0)
            project_history(argc, cline);
        else if(strcmp("alias", *cline) == 0)
            project_alias(argc, cline);
        else if(strcmp("ps", *cline) == 0)
            project_ps(argc, cline);
        else if(strcmp("df", *cline) == 0)
            project_df(argc, cline);
        else if(strcmp("du", *cline) == 0)
            project_du(argc, cline);
        else
            return -1;
    }
    return 0;
}
