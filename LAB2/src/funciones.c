#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../incl/funciones.h"



void argumentos(char *imagenEntrada, char *archivoSalida, int niveles, int bins){
	printf("Archivo de entrada: %s\n", imagenEntrada);
	printf("Archivo de salida: %s\n", archivoSalida);
	printf("Cantidad de niveles: %d\n", niveles);
	printf("Cantidad de bins: %d\n", bins);
}

int validacionEntradas(char *imagenEntrada, int niveles, int bins){
	if(imagenValida(imagenEntrada) && niveles >= 0 && niveles <= 8 && binValido(bins))
		return TRUE;
	return FALSE;
}

int imagenValida(char *imagenEntrada){	
	FILE *img = fopen(imagenEntrada, "r");
	if(!img)
		return FALSE;
	fclose(img);
	return TRUE;
}

int binValido(int bin){
	int flag = TRUE;

	if(bin<1 || bin>256)
		return FALSE;

	while(flag){
		if(bin%2 == 0)
			bin = bin/2;
		else if(bin == 1)
			return TRUE;
		else
			flag = FALSE;
	}
	return FALSE;
}

void *prueba(void * data){

	struct thread_data *myData = (struct thread_data *) data;
	int nivel = myData->nivel;

	if(nivel < nivelMax)
		printf("\nHay que divirid mas\n");
	else
		printf("tamo bien aqui\n");

	printf("\nPrueba de funcion de un proceso %d.\n\n", nivel);
	pthread_exit(0);
}