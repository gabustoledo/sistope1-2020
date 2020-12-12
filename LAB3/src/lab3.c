#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../incl/funciones.h"

int main(int argc, char **argv){

	// Parametros de entrada
	char *archivoEntrada;
	char *archivoSalida;
	int ancho;
	int discos;
	int tamanoBuffer;
	int debug = 0;

	int c;

	if(argc != 11 && argc != 12){
		printf("Los parametros ingresados no son validos.\n");
		exit(0);
	}

	while (((c = getopt(argc, argv, "i:o:d:n:s:b")) != -1)){
		switch (c){
			case 'i':
				archivoEntrada = optarg;
				break;
			case 'o':
				archivoSalida = optarg;
				break;
			case 'd':
				ancho = atof(optarg);
				break;
			case 'n':
				discos = atof(optarg);
				break;
			case 's':
				tamanoBuffer = atof(optarg);
				break;
			case 'b':
				debug = 1;
				break;
		}
	}

	if(!validarParametros(archivoEntrada, ancho, discos, tamanoBuffer)){
		printf("Los parametros ingresados no son validos.\n");
		exit(0);
	}

	mostrarEntrada(archivoEntrada, archivoSalida, ancho, discos, tamanoBuffer, debug);

	return 0;
}

// ./lab3 -i entrada/prueba2.csv -o salida.txt -d 100 -n 4 -s 20