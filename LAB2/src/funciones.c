#include <stdio.h>
#include "../incl/funciones.h"

void argumentos(char *imagenEntrada, char *archivoSalida, int niveles, int bins){
	printf("Archivo de entrada: %s\n", imagenEntrada);
	printf("Archivo de salida: %s\n", archivoSalida);
	printf("Cantidad de niveles: %d\n", niveles);
	printf("Cantidad de bins: %d\n", bins);
}