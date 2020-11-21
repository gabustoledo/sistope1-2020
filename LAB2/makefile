CC=gcc
CFLAGS=-Wall -g
SRC_DIR=src
INCL_DIR=incl
OBJ_DIR=obj

main: hist

hist: $(OBJ_DIR)/hist.o $(OBJ_DIR)/funciones.o
	@$(CC) $(CFLAGS) -o lab2 $(SRC_DIR)/hist.c $(OBJ_DIR)/funciones.o

$(OBJ_DIR)/funciones.o: $(SRC_DIR)/funciones.c $(INCL_DIR)/funciones.h
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c  $(SRC_DIR)/funciones.c -o $(OBJ_DIR)/funciones.o

$(OBJ_DIR)/hist.o: $(SRC_DIR)/hist.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/hist.c -o $(OBJ_DIR)/hist.o

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) lab2