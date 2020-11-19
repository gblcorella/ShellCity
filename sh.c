#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <wordexp.h>
#include <glob.h>
#include <utmp.h>
#include <errno.h>
#include "sh.h"

#define BUFFER_SIZE 1000
#define MAXLINE 4096

// Gabriel Corella
// CISC 361: Unix Shell Program Fall 2019

pid_t cpid;
int sh(int argc, char **argv, char **envp){
  char	buf[MAXLINE];
  pid_t	pid;
  char *arg, *commandpath, *p, *cwd, *owd, *command;
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  //  char *commandline = calloc(MAX_CANON, sizeof(char));
  // = calloc(MAX_CANON, sizeof(char));

  char **args = calloc(MAXARGS, sizeof(char*));
  char *promptPrefix;
  promptPrefix = " ";
  int uid, i, status, argsct, child_pid, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;
/Users/gabecorella/Downloads/README.md
  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
  out with*/

  if ( (cwd = getcwd(NULL, 0)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(cwd) + 1, sizeof(char));
  memcpy(owd, cwd, strlen(cwd));
  prompt[0] = ' '; prompt[1] = '\0';
  pathlist =get_path();
  struct pathelement *temp;
  //  free(promptPrefix);
  free(owd);
  free(cwd);
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGTERM, SIG_IGN);
  signal(SIGCHLD, handle_sigchild);
  printf("Welcome to Shell City \n");
  while (go == 1){
    //printf("%s", prompt);
    cwd = getcwd(NULL,0);
    printf("%s[%s]>>>", promptPrefix, cwd);
    free(cwd);
    int j;
    for (int j = 0; j < MAXARGS; j++) {
      free(args[j]);
    }
    free(args);
    char **args = (char**)calloc(MAXARGS, sizeof(char*));
    while (fgets(buf, MAXLINE, stdin) != NULL) {
      if (buf[strlen(buf) - 1] == '\n'){
        buf[strlen(buf) - 1] = 0;
      }
      int num_args = 0;
      int argsct = 0;
      int count = 0;
      p = strtok(buf," ");
      do{
        args[num_args] = p;
        num_args++;
      }while(p = strtok(NULL," "));
      command = args[0];
      argsct = num_args;
      // *********
      if (strcmp(args[0], "pwd") == 0) {
        printf("\nRunning built in command pwd\n");
        cwd = getcwd(NULL, 0);
        printf("CWD = [%s]\n", cwd);
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
      else if(strcmp(args[0],"exit") == 0){
        printf("\n");
        printf("Leaving Shell City \n");
        printf("\n");
        go = 0;
        free(prompt);
        free(args);
        freepathlist(pathlist);
        exit(0);
      }
      else if(strcmp(args[0],"which") == 0){
        printf("\nRunning built in command which\n");
        cwd = getcwd(NULL, 0);
        if (argsct < 2){
          printf("%s", "which: Too few arguments.\n");
        }else {
          count = 1;
          while (args[count] != NULL) {
            which(args[count], pathlist);
            count++;
          }
        }
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
      else if(strcmp(args[0],"where") == 0){
        printf("\nRunning built in command where\n");
        cwd = getcwd(NULL, 0);
        if (argsct < 2) {
          printf("%s", "where: Too few arguments.\n");
        }
        count = 1;
        while (args[count] != NULL) {
          where(args[count], pathlist);
          count++;
        }
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
      else if(strcmp(args[0],"kill") == 0){
        printf("\nRunning built in command kill\n");
        if(argsct < 2) {
  		fprintf(stderr, "kill: Too few arguments.\n");
	} else if(argsct < 3 || strchr(args[1], '-') == NULL) {

  		int i = 1;
  		while(args[i] != NULL) {
    			kill_process(args[i]);
    			i++;
  		}
	} else {
  /* kill with the given signal number */
  		char *signal;
		signal = strtok(args[1], "-");
  		i = 2;
  		while(args[i] != NULL) {
    		kill_process_signal(args[i], signal);
    			i++;
  		}
	}	

	cwd = getcwd(NULL, 0);
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
      else if(strcmp(args[0],"cd") == 0){
        printf("\nRunning built in command cd\n");
        if(argsct < 2){
          chdir(getenv("HOME"));
          if ( (cwd = getcwd(NULL, 0)) == NULL ){
            perror("getcwd");
            exit(2);
          }
          printf("%s[%s]>>>", promptPrefix, cwd);
          free(cwd);
        }
        else if(strcmp(args[1],"-") == 0){
          chdir("..");
          if ((cwd = getcwd(NULL, 0)) == NULL ){
            perror("getcwd");
            exit(2);
          }
          printf("%s[%s]>>>", promptPrefix, cwd);
          free(cwd);
        }
        else{
          chdir(args[1]);
          if ( (cwd = getcwd(NULL, 0)) == NULL ){
            perror("getcwd");
            exit(2);
          }
          printf("%s[%s]>>>", promptPrefix, cwd);
          free(cwd);
        }
      }
      else if(strcmp(args[0],"list") == 0){
        printf("\nRunning built in command list\n");
        cwd = getcwd(NULL,0);
        count = 1;
        if (argsct < 2) {
          list(cwd);
        }else{
	  while(count < argsct){
	    list(args[count]);
	    printf("\n");
	    count++;
	  }
	
	}
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
      else if(strcmp(args[0],"pid") == 0){
        printf("\nRunning built in command pid\n");
        cwd = getcwd(NULL,0);
        pid = getPID();
	printf("The process id is %d\n", pid);
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
      else if (strcmp(args[0],"prompt") == 0){
        printf("\nRunning built in command prompt\n");
        cwd = getcwd(NULL,0);
        //free(promptPrefix);
        promptPrefix = newPrompt(prompt, promptPrefix, cwd, args[1]);
        printf("\n");
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }

      else if(strcmp(args[0],"printenv") == 0){
        printf("\nRunning built in command printenv\n");
        cwd = getcwd(NULL, 0);
        if(argsct < 2){
          printenv(envp);
        }
        else if (argsct == 2){
          print_env_variable(args[1]);
        }
        else{
          fprintf(stderr, "Incorrect number of arguments for %s function.\n", args[0]);
        }
        printf("\n");
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
      else if(strcmp(args[0],"setenv") == 0){
        printf("\nRunning built in command setenv\n");
        cwd = getcwd(NULL, 0);
        if(argsct < 2) {
          printenv(envp);
        }
        else if(argsct == 2) {
          set_env(args[1], "");
          if(strcmp(args[1],"PATH") == 0){
            freepathlist(pathlist);
            pathlist = get_path();
          }
        }
        else if (argsct == 3){
          set_env(args[1],args[2]);
          if(strcmp(args[1],"PATH") == 0){
            freepathlist(pathlist);
            pathlist = get_path();
          }
        }
        else{
          errno = E2BIG;
          perror("setenv");
        }
        printf("\n");
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
      else {                           /* external command */
        cwd = getcwd(NULL,0);
        //int argsct;
        int found = 0;
        int status;
	pid_t cpid;
        glob_t globbuf;
        globbuf.gl_offs = 0;
	int globreturn;
	char **p;
        char* toexec = malloc(MAX_CANON*sizeof(char));
        if(access(args[0], X_OK) == 0){
          found = 1;
          strcpy(toexec, args[0]);
        }
        else{
          struct pathelement* curr = pathlist;
          char *tmp = malloc(MAX_CANON*sizeof(char));

          while(curr!=NULL & !found){
            snprintf(tmp,MAX_CANON,"%s/%s", curr->element, args[0]);
            if(access(tmp, X_OK)==0){
              toexec = tmp;
              found = 1;
            }
            curr=curr->next;
          }
        }

        // If the command if found in the path, execute it as a child process
        if(found){

          printf("Executing %s\n", toexec);

          // Create a child process
          cpid = fork();
	  //printf("forked worked");
          if (cpid < 0) {
            printf("fork error\n");
            exit(1);
          }
          else if(cpid == 0){
            // Child process executes command
            int i = 0;
	   glob(args[0],GLOB_NOCHECK,NULL,&globbuf); 
            for(i = 1; i < argsct; i++){
                glob(args[i],GLOB_NOCHECK | GLOB_APPEND,NULL,&globbuf);
		//execve(toexec, args, envp);
              }
	
  	    execve(toexec, &globbuf.gl_pathv[0], envp);
	    
          }
          else {

            // Parent process (shell) waits for child process to terminate
            waitpid(cpid, &status, 0);


            // Return non-normal exit status from child
            if(WIFEXITED(status)){
              if(WEXITSTATUS(status) != 0){
                printf("child exited with status: %d\n", WEXITSTATUS(status));
              }
            }
          }
        }
        printf("%s[%s]>>>", promptPrefix, cwd);
        free(cwd);
      }
    }
    //free(cwd);
    //exit(0);
  }
  free(prompt);
  free(args);
  free(cwd);
  return 0;
}

char *which(char *command, struct pathelement *pathlist )
{
  /* loop through pathlist until finding command and return it.  Return
  NULL when not found. */
  struct pathelement *curr = pathlist;
  char *path = malloc(MAX_CANON*(sizeof(char)));
  int found = 0;
  if (strcmp(command,"which") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"where") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"list") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"pwd") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"prompt") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"kill") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"pid") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"cd") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"printenv") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  else if (strcmp(command,"setenv") == 0){
    printf("%s: This is a built-in command\n", command);
    return NULL;
  }
  //A buffer for concatting strings
  /* loop through pathlist until finding command and return it.  Return
  NULL when not found. */
  else{
	while(curr != NULL && !found){
        strcpy(path,curr->element);
        path[strlen(path)+1] = '\0';
        path[strlen(path)] = '/';
        strcat(path,command);
        if(access(path,F_OK) == 0){
                printf("%s\n",path);
                found = 1;
		return NULL;
        }
        curr = curr->next;
    }

	if (!found){
                fprintf(stderr, "%s: command not found\n", command);
                return NULL;
        }
  }//end of else
} /* which() */

// list all commands withing where
char *where(char *command, struct pathelement *pathlist) {
  struct pathelement *curr = pathlist;
  char *path = malloc(MAX_CANON*(sizeof(char)));
  int found = 0;
  if (strcmp(command,"which") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"where") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"list") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"pwd") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"prompt") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"kill") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"pid") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"cd") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"printenv") == 0){
    printf("%s: This is a built-in command\n", command);
  }
  else if (strcmp(command,"setenv") == 0){
    printf("%s: This is a built-in command\n", command);
  }


  else{
    while(curr != NULL && !found){
	strcpy(path,curr->element);
	path[strlen(path)+1] = '\0';
	path[strlen(path)] = '/';
	strcat(path,command);	
	if(access(path,F_OK) == 0){
		printf("%s\n",path);
		found = 1;
	}
	curr = curr->next;
    }
    if (!found){
                fprintf(stderr, "%s: command not found\n", command);
                return NULL;
        }
  }
   return NULL;
} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
  DIR *directoryListing;
  struct dirent *ent;
  //char *zwd;
  directoryListing = opendir(dir);
  if(directoryListing == NULL){
    perror(dir);
  }
  else{
    //print the directory we're listing
    printf("%s\n",dir);
    while ((ent = readdir(directoryListing)) != NULL) {
      printf("\n");
      printf("%s\n", ent->d_name);
     }
 }
  closedir(directoryListing);
} /* list() */

void printWorking (char *dwd) {
  // getcwd(dwd, sizeof(dwd));
  dwd = getcwd(NULL,0);
  printf("\nDir: %s\n", dwd);
  free(dwd);
}

int getPID()
{
  pid_t pid, pid2;
  gid_t gid;
  if ((pid = getpid()) < 0) {
    perror("unable to get pid");
    return 0;
  } else {
    return pid;
  }
}

void killPID(char *pid, char * signalNumber) {
  if (signalNumber == NULL ) {

    if (kill(atoi(pid), SIGTERM ) < 0) {
      perror("unable to kill process");
    } else {
      printf("The process killed \n");
    }
  }
  else {
    if (kill(atoi(pid), atoi(signalNumber)) < 0) {
      perror("unable to kill process");
    } else {
      printf("The process killed \n");
    }
  }

}


char *newPrompt(char * currentPrompt, char * currentHeader, char * currentDIR, char * promptName) {
  if (promptName == NULL) {
    printf("Input prompt prefix: ");
    char *commandlinePrompt = calloc(MAX_CANON, sizeof(char));
    fgets(commandlinePrompt, MAX_CANON, stdin);
    commandlinePrompt[strlen(commandlinePrompt)-1] = '\0';
    currentHeader = commandlinePrompt;
    snprintf(currentPrompt, PROMPTMAX, "%s%s%s%s", currentHeader , " [", currentDIR, "]>>>");
    free(commandlinePrompt);
    free(currentPrompt);
    return currentHeader;
  }
  else {
    currentHeader = promptName;
    snprintf(currentPrompt, PROMPTMAX, "%s%s%s%s", currentHeader, " [", currentDIR, "]>>>");
    return currentHeader;
  }
  free(currentHeader);
}

void printenv(char **envp){
  char **env;
  env = envp;
  while(*env){
    printf("%s\n", *env);
    env++;
  }
}

void print_env_variable(char *env_var){
  char *env = getenv(env_var);
  if(env != NULL){
    printf("%s\n", env);
  }
  else {
    printf("Enviornment does not exist.\n");
  }
}


void set_env(char *envname, char *envval){
  setenv(envname,envval,1);
}

void freepathlist(struct pathelement *head){
  struct pathelement *current = head;
  struct pathelement *temp;
  temp = current;
  free(temp->element);
  while(current != NULL){
    temp = current;
    current = current->next;
    free(temp);
  }
  free(current);
}

void handle_sigchild(int sig) {
  while (waitpid((pid_t) (-1), 0, WNOHANG) > 0) {

  }
}

void kill_process_signal(char* process, char *signal) {
  int i = 0;
  if(atoi(process) && atoi(signal)) {
    if((i = kill(atoi(process), atoi(signal))) < 0){
        perror("unable to kill process");
    }else{
      printf("The process was killd");
    }
  } else {
    fprintf(stderr, "kill: Arguments should be jobs or process id's.\n");
  }
}

void kill_process(char *process) {
  int i = 0;
  if(atoi(process)) {
    if((i = kill(atoi(process),SIGTERM)) < 0){
      perror("unable to kill process");
    } else{
      printf("The process was killed \n");
    }
  } else {
    fprintf(stderr, "kill: Arguments should be jobs or process id's.\n");
  }
}

