#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "fsh.h"

#define PROMPT 2

int ntok;	//NUMBER OF TOKENS IN EXPRESSION

char *readline(char *line);
arglist *tokline(char *str);
int execline(arglist *argbox);
int execbuiltin(arglist *argbox);
void sigint_handler(int signum);

int main(void) {

	struct sigaction intact;
	char *line, **parsed;
	int c, status;

	intact.sa_handler = sigint_handler;
	printf("%s\n", "Welcome to the only shell for true friends! Ban Paku Banzai!");
	printf("\n\n                                                        .\n                                                     :@@@@@\n                                                     @@  :@@\n                                                     @:   @@\n                                        @=          .@    @@\n                                        @@          .@    @@      %%\n                              @:        @@+  .=*#%%@@@@    @@     %%@.\n                              @@.     -%%@@@@@@@@@@@@@@    @@@@# #@@\n                              -@@*@@@@@@@@*.        @@.   @@@@@@@@@\n                    @        .@@@@@@*         .@@@@ %%@-   @@    =@@@@@     @%% \n                    %%@-   =@@@@@=       @@@@@@@+  @@@@#   @@+       #@@@@+@@\n                     %%@@@@@@#        .-@+   @@@    -@@@   *@%%          @@@@%%\n                    .@@@@%%        *@@@@@-    =@%%    .@@   .@@            #@@@\n          .       -@@@@           @*   =@*     @%%     %%    @@              *@@@    #@\n          @%%    *@@@%%             @@     @%%                -@+               *@@@@@@*\n           @@@@@@@:               -@:       -=       =@     @@                 *@@@@      .@\n     .=%%@@@@@@@*                   @@       .@@@@@@@%%@*     @@                   *@@@@    @%%\n    @@@@@@@@:                      @@  @@=@@@*      .@@#  @.@@                     .@@@@@@@.\n     #@@@@#                        *@  @@@.  *=    #@. +@@# @@                         *@@@@@@@-\n        @@@@@                      =@@@-    %%    .   +*  :@@@@                           #@@@@@\n          :@@@@                    +@+     %%  .=  :*  @    :@@                        =@@@@@#\n            .@@@@                  #@@=    #= :+  .%%  @    %%@@                      @@@@@-\n              #@@@#                #@:@@=   @.  -=   #-  =@@@@                    @@@@@\n             %%@@@@@@               #@:  @@*  =@:  .@@  .@@ #@@                  @@@@@@@@\n            @@@-  @@@+             .@#    @@@#.     +@@@@  @@#                %%@@@-   .@@.\n           *@.     +@@@             @@*  .=   %%@@@@@-    * @@-              -@@@-       -#\n                     %%@@@            @@=         @        +@@             .@@@@\n                     #@@@@@           @@%%                .@@:           -@@@@@@@\n                    @@@  @@@@:         @@@.              @@%%          #@@@*   @@@\n                   *@      *@@@@+    #@@@@@@@@@@@@@@@@@@@@@@@@     #@@@@-      .@\n                             :@@@@@@@@@@+                  @@@ =@@@@@@@\n                             @@:  -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:  @@+\n                            :@:     @@@     ....     .:.     @@@      @@\n                             -      @@@                      @@@       .\n                                    -.                       .:\n\n");
	do {
		ntok = 0;
		printf("%s", "> ");
		if ((line = readline(line)) == NULL) {
			status = 1;
			free(line);
		} else {
			parsed = tokline(line);
			status = execline(parsed);
			free(*parsed);
			free(parsed);
		}
		if (sigaction(SIGINT, &intact, NULL) == -1) {
			perror("");
			exit(EXIT_FAILURE);
		}
	} while (status);
	return 0;

}

char *readline(char *line) {

	size_t bufsize = 2;
	int c, n = 0;
	if ((line = malloc(sizeof(char) * bufsize)) == NULL) {
		perror("malloc err");
		exit(EXIT_FAILURE);
	}
	while ((c = fgetc(stdin)) != EOF && c != '\n' && c != '&') {
		*(line + n) = c;
		n++;	
	}
	*(line + n) = '\0';
	if (strlen(line) < 2)
		return NULL;
	return line;

}

arglist *tokline(char *str) {

	char **toks, **dummy;
	char *current_tok;
	arglist *azg;
	size_t mn = 2;
	if ((toks = malloc(sizeof(char *) * mn)) == NULL || azg = malloc(sizeof(arglist)) == NULL) {
		perror("malloc err");
		exit(EXIT_FAILURE);
	}
	if ((current_tok = strtok(str, " ")) == NULL) {
		perror("strtok err");
		exit(EXIT_FAILURE);
	}
	*(toks + ntok) = current_tok;	//WILL ALWAYS BE PROGRAM TOKEN
	ntok++;
	while ((current_tok = strtok(NULL, " ")) != NULL) {
		if (ntok > mn) {
			mn *= 2;
			dummy = realloc(toks, sizeof(char *) * mn);
			if (dummy == NULL) {
				perror("realloc err");
				exit(EXIT_FAILURE);
			}
			toks = dummy;	
		}
		*(toks + ntok) = current_tok;
		ntok++;
	}
	*(toks + ntok) = NULL;		//remove latr
	azg -> progname = *toks;
	azg -> argbox = toks;
	return azg;

}

int execline(arglist *azg) {

	pid_t pid, wpid;
	int check_s, n;

	n = 0;
	while (*(azg + (n++)) != NULL) {		//FOR EACH COMMAND IN THE BOX, EXECUTE
		for (int i = 0; i < num_of_builtins; i++)	//IF BUILTIN COMMAND, EXECUTE AS BUILTIN
			if (strcmp(*args, builtins[i]) == 0)
				return execbuiltin((azg + n) -> argbox);
		pid = fork();
		if (pid == 0) {		//COMMANDS FOR CHILD
			if (execvp((azg + n) -> progname, (azg + n) -> argbox) == -1) {
				printf("%s\n", "Not a valid program or FSH command.");
				exit(EXIT_FAILURE);
			}
			exit(0);
		} else if (pid == -1) {
			perror("fork err");
			exit(EXIT_FAILURE);
		} else {	//COMMANDS FOR PARENT
			do {
				wpid = waitpid(pid, &check_s, WUNTRACED);
			} while (!WIFEXITED(check_s) && !WIFSIGNALED(check_s)); 
		}
	}
	return 0;
}

int execbuiltin(char **args) {

	if (strcmp(builtins[0], *args) == 0)	//CD
		return cd(ntok, args);
	if (strcmp(builtins[1], *args) == 0)	//EXIT
		exitsh();
	if (strcmp(builtins[2], *args) == 0)	//HELP
		helpsh();
	return PROG_ERR;

}

void sigint_handler(int s) {

	printf("\n");

}
