CC=gcc
SRC=src/main
DEPS=src/headers
SRS=$(SRC)/fsh.c $(SRC)/builtins.c

fsh: $(SRS)
	$(CC) -o $@ $^ -I $(DEPS)
clean: fsh
	rm -f $^
