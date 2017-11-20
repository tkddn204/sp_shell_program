#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// default functions
int userin(char *p);
int gettok(char **outptr);
int inarg(char c);
void procline();
int runcommand(char **cline, char where);

// project functions
int project_ps(char *cline);
int project_df(cline);
int project_du(cline);
int project_print_recode(cline);
