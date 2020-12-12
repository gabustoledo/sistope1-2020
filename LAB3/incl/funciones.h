#ifndef FUNCIONES_H
#define FUNCIONES_H

#define TRUE 1
#define FALSE 0

void mostrarEntrada(char *entrada, char *salida, int ancho, int discos, int tamanoBuffer, int debug);
int validarParametros(char *entrada, int ancho, int discos, int tamanoBuffer);
int entradaValida(char *entrada);

#endif