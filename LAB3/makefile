CC=gcc
CFLAGS=-Wall -g
SRC_DIR=src
INCL_DIR=incl
OBJ_DIR=obj

main: lab3

lab3: $(OBJ_DIR)/lab3.o $(OBJ_DIR)/funciones.o
	@$(CC) $(CFLAGS) -pthread -o lab3 $(SRC_DIR)/lab3.c $(OBJ_DIR)/funciones.o

$(OBJ_DIR)/funciones.o: $(SRC_DIR)/funciones.c $(INCL_DIR)/funciones.h
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c  $(SRC_DIR)/funciones.c -o $(OBJ_DIR)/funciones.o

$(OBJ_DIR)/lab3.o: $(SRC_DIR)/lab3.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/lab3.c -o $(OBJ_DIR)/lab3.o

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) lab3