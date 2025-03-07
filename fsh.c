#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT 2

char *readline(char *line);
char **tokline(char *str);
int execline(char **to_exec);

int ntok = 0;

int main(void) {

	FILE *fptr;
	char *line, **parsed;
	int c;
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
	parsed = tokline(readline(line));
	execline(parsed);
	free(*parsed);
	free(parsed);
	return 0;

}

char *readline(char *line) {

	size_t bufsize = 0;
	if (getline(&line, &bufsize, stdin) == -1) {
		perror("getline err");
		exit(EXIT_FAILURE);
	}
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
	*(toks + ntok) = NULL;
	return toks;

}

int execline(char **args) {

	pid_t pid, wpid;
	int check_s;
	pid = fork();
	if (pid == 0) {		//COMMANDS FOR CHILD
		if (execvp(*args, args) == -1) {
			perror("exec err");
			exit(EXIT_FAILURE);
		}
	} else if (pid == -1) {
		perror("fork err");
		exit(EXIT_FAILURE);
	} else {	//COMMANDS FOR PARENT
		do {
			wpid = waitpid(pid, &check_s, WUNTRACED);
		} while (!WIFEXITED(check_s) && WIFSIGNALED(check_s)); 
	}
	return PROMPT;
}
