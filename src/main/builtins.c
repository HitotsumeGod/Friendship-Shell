#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fsh.h>

char *builtins[] = {"cd", "exit", "help", "AND", "expo", "dj"};
int num_of_builtins = 6;

int cd(int argc, char **args) {

	int status;
	if (argc != 2) {
		perror("cd command requires a single target!");
		return PROG_ERR;
	}
	if ((status = chdir(*(args + 1))) == -1) {
		perror("Error changing directories. Ensure target validity before attempting again.");
		return PROG_ERR;
	}
	printf("%s%s%s\n", "Changed to directory '", *(args + 1), "'.");
	return PROG_SUCCESS;

}
void exitsh(void) {

	printf("%s\n", "Exiting the shell.");
	exit(PROG_FIN);

}

void helpsh(void) {

	printf("| %s\n| %s\n| %s\n", "To leave the shell, use the 'exit' command.", "To get a reminder about the next World Expo, use the 'expo' command.", "To listen to some music, use the 'dj' command.");

}
int expo(int argc, char **args);
int dj(int argc, char **args);

