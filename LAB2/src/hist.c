#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../incl/funciones.h"




int main(int argc, char **argv){

	// Argumentos de entrada
	char *imagenEntrada = NULL;
	char *archivoSalida = NULL;
	int niveles = 0;
	int bins = 0;

	// Variables para crear hebras
	pthread_t tid;
  pthread_attr_t attr;

	// Para leer argumentos de entrada
	int c;

	if(argc != 9){
		printf("\nArgumentos ingresados no son la cantidad adecuada.\n\n");
		return 0;
	}

	while (((c = getopt(argc, argv, "i:o:L:B:")) != -1)){
		switch (c){
		case 'i':
			imagenEntrada = optarg;
			break;
		case 'o':
			archivoSalida = optarg;
			break;
		case 'L':
			nivelMax = atof(optarg);
			break;
		case 'B':
			bins = atof(optarg);
			break;
		}
	}

	argumentos(imagenEntrada, archivoSalida, nivelMax, bins);

	if(!validacionEntradas(imagenEntrada, niveles, bins)){
		printf("\nParametros ingresados no son validos.\n");
		return 0;
	}

	printf("\nParametros validos.\n");

	// PRUEBAAAAAS
	struct thread_data thread_data_array;
	thread_data_array.nivel = 0;

	pthread_attr_init(&attr);
  pthread_create(&tid, &attr, prueba, (void *) &thread_data_array);
  pthread_join(tid, NULL);

	return 0;
}

// ./lab2 -i inputFiles/imagen_1.bmp -o histograma.txt -L 2 -B 256