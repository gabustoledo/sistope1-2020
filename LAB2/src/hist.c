#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../incl/funciones.h"

int main(int argc, char **argv){

	char *imagenEntrada = NULL;
	char *archivoSalida = NULL;
	int niveles = 0;
	int bins = 0;

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
			niveles = atof(optarg);
			break;
		case 'B':
			bins = atof(optarg);
			break;
		}
	}

	argumentos(imagenEntrada, archivoSalida, niveles, bins);

	return 0;
}