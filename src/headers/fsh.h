#ifndef __FSH_C__
#define __FSH_C__

#define PROG_FIN 0
#define PROG_ERR 1
#define PROG_SUCCESS 2

struct arglist {
	char *progname;		//NAME OF PROGRAM/BUILTIN
	char **args;		//NULL-TERMINATED VECTOR OF ARGUMENTS FOR EXECVP
	struct arglist next;
};

typedef struct arglist arglist;

extern char *builtins[];
extern int num_of_builtins;

extern int cd(int argc, char *args[]);
extern void exitsh(void);
extern void helpsh(void);
extern int expo(int argc, char *args[]);
extern int dj(int argc, char*args[]);

#endif //FSH_C
