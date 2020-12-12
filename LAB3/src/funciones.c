#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../incl/funciones.h"

void mostrarEntrada(char *entrada, char *salida, int ancho, int discos, int tamanoBuffer, int debug){
	printf("El archivo de entrada es: %s\n", entrada);
	printf("El archivo de salida es: %s\n", salida);
	printf("El ancho de los discos es: %d\n", ancho);
	printf("La cantidad de discos es: %d\n", discos);
	printf("El tamano del buffer es: %d\n", tamanoBuffer);
	printf("El modo debug es: %d\n", debug);
}

int validarParametros(char *entrada, int ancho, int discos, int tamanoBuffer){
	if(entradaValida(entrada) && ancho>0 && discos>0 && tamanoBuffer>0)
		return TRUE;
	return FALSE;
}

int entradaValida(char *entrada){
	FILE *input = fopen(entrada, "r");
	if(input==NULL)
		return FALSE;
	return TRUE;
}