CC=gcc
SRC=src
BUILD=build
DEPS = $(SRC)/headers/fsh.h
OBJ = $(BUILD)/fsh.o $(BUILD)/builtins.o

$(BUILD)/%.o: $(SRC)/main/%.c $(DEPS) 
	$(CC) -c -o $@ $< -I$(SRC)/headers

fsh: $(OBJ)
	$(CC) -o $@ $^ 
