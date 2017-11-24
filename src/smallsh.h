#ifndef SMALLSH_H
#define SMALLSH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT_SIZE 256
#define CWD_SIZE 256
#define PATH_SIZE 256
#define BUF_SIZE 512

// default functions
int userin(char *p);
int gettok(char **outptr);
int inarg(char c);
void procline();
int runcommand(int argc, char **cline, char where);

// project functions
void prompt(char *prom);
int command_parser(int pid, int argc, char **cline);
int project_cd(int argc, char **argv);

int project_history(int argc, char **argv);
int project_alias(int argc, char **argv);

int project_ps(int argc, char **argv);
int project_df(int argc, char **argv);
int project_du(int argc, char **argv);

#endif