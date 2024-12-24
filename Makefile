OBJ_DIR = obj
BIN_DIR = bin
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)
EXEC = $(BIN_DIR)/shell
INCLUDES = -Iinclude  # Add the path to header files here (e.g., src/ or include/)

CC = gcc
CFLAGS = -Wall -g $(INCLUDES)

# Targets and rules for compiling and linking

$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(EXEC)

# Rule to compile .c files into .o files
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean $(EXEC)

