#include "get_path.h"
#define PROMPTMAX 50
#define MAXARGS 10


int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list ( char *dir );
void printWorking (char *pwd);
int getPID();

void killPID(char *pid, char * signalNumber);
char *newPrompt(char * currentPrompt, char * currentHeader, char * currentDIR, char * promptName);
void printenv(char **envp);
void print_env_variable(char *env_var);
void freepathlist(struct pathelement *head);
void set_env(char *envname, char *envval);
int runExternalCommand(int argsct, char **args, char **envp);
void handle_sigchild(int sig);

void kill_process_signal(char* process, char *signal);
void kill_process(char *process);

