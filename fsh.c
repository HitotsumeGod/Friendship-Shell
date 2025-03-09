#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "fsh.h"

#define PROMPT 2

int ntok;	//NUMBER OF TOKENS IN EXPRESSION

char *readline(char *line);
char **tokline(char *str);
int execline(char **to_exec);
int execbuiltin(char **args);

int main(void) {

	FILE *fptr;
	char *line, **parsed;
	int c, status;
	printf("%s\n", "Welcome to the only shell for true friends! Ban Paku Banzai!");
	if ((fptr = fopen("FRIEND", "r")) == NULL) {
		perror("fopen err");
		exit(EXIT_FAILURE);
	}
	while ((c = fgetc(fptr)) != EOF)
		printf("%c", c);
	if (fclose(fptr) == -1) {
		perror("fclose err");
		exit(EXIT_FAILURE);
	}
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

char **tokline(char *str) {

	char **toks, **dummy;
	char *current_tok;
	size_t mn = 2;
	if ((toks = malloc(sizeof(char *) * mn)) == NULL) {
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
	return toks;

}

int execline(char **args) {

	pid_t pid, wpid;
	int check_s;
	for (int i = 0; i < num_of_builtins; i++)	//IF BUILTIN COMMAND, EXECUTE AS BUILTIN
		if (strcmp(*args, builtins[i]) == 0)
			return execbuiltin(args);
	pid = fork();
	if (pid == 0) {		//COMMANDS FOR CHILD
		if (execvp(*args, args) == -1) {
			perror("exec err");
			exit(EXIT_FAILURE);
		}
		exit(1);
	} else if (pid == -1) {
		perror("fork err");
		exit(EXIT_FAILURE);
	} else {	//COMMANDS FOR PARENT
		do {
			wpid = waitpid(pid, &check_s, WUNTRACED);
		} while (!WIFEXITED(check_s) && !WIFSIGNALED(check_s)); 
	}
	return 1;
}

int execbuiltin(char **args) {

	if (strcmp(builtins[0], *args) == 0)
		return cd(ntok, args);
	if (strcmp(builtins[1], *args) == 0)
		exitsh();
	return PROG_ERR;

}
