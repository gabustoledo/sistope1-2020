CC=gcc
CFLAGS=-Wall -g
SRC_DIR=src
INCL_DIR=incl
OBJ_DIR=obj
RP_DIR=rp

main: comparador coordinador

comparador: $(OBJ_DIR)/comparador.o $(OBJ_DIR)/funciones.o
	@$(CC) $(CFLAGS) -o comparador $(SRC_DIR)/comparador.c $(OBJ_DIR)/funciones.o

coordinador: $(OBJ_DIR)/coordinador.o $(OBJ_DIR)/funciones.o
	@$(CC) $(CFLAGS) -o sucesion $(SRC_DIR)/coordinador.c $(OBJ_DIR)/funciones.o

$(OBJ_DIR)/funciones.o: $(SRC_DIR)/funciones.c $(INCL_DIR)/funciones.h
	@mkdir -p $(OBJ_DIR) $(RP_DIR)
	@$(CC) $(CFLAGS) -c  $(SRC_DIR)/funciones.c -o $(OBJ_DIR)/funciones.o

$(OBJ_DIR)/comparador.o: $(SRC_DIR)/comparador.c
	@mkdir -p $(OBJ_DIR) $(RP_DIR)
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/comparador.c -o $(OBJ_DIR)/comparador.o

$(OBJ_DIR)/coordinador.o: $(SRC_DIR)/coordinador.c
	@mkdir -p $(OBJ_DIR) $(RP_DIR)
	@$(CC) $(CFLAGS) -c $(SRC_DIR)/coordinador.c -o $(OBJ_DIR)/coordinador.o

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) $(RP_DIR) comparador sucesion