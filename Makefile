CC=gcc
DEPS = fsh.h
OBJ = fsh.o builtins.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -g

fsh: $(OBJ)
	$(CC) -o $@ $^ 
