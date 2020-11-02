#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../incl/funciones.h"

int main(int argc, char **argv){

	// Argumentos de entrada.
	char *archivoEntrada;
	int inicio;
	int numeroLineas;
	char *cadena;
	char *id;

	int *encontrado;
	int largoCadena;
	char archivoSalida[100];
	char *inicioStr;
	int c;

	// Son obtenidos los argumentos de entrada.
	while (((c = getopt(argc, argv, "i:n:c:p:d:")) != -1)){
		switch (c){
		case 'i':
			archivoEntrada = optarg;
			break;
		case 'n':
			inicioStr = optarg;
			inicio = atof(inicioStr);
			break;
		case 'c':
			numeroLineas = atof(optarg);
			encontrado = (int *)malloc(numeroLineas * sizeof(int));
			break;
		case 'p':
			cadena = optarg;
			largoCadena = strlen(cadena);
			break;
		case 'd':
			id = optarg;
			break;
		}
	}

	// Se lee el archivo de entrada y se hacen las comprobaciones si la cadena se encuentra.
	openFileRP(archivoEntrada, inicio, numeroLineas, largoCadena, cadena, encontrado);

	// Es creado el nombre del archivo de salida.
	strcpy(archivoSalida, "rp/rp_");
	strcat(archivoSalida, cadena);
	strcat(archivoSalida, "_");
	strcat(archivoSalida, id);
	strcat(archivoSalida, ".txt");

	// Se escribe el archivo se salida con los resultados parciales.
	writeFileRP(archivoSalida, archivoEntrada, encontrado, numeroLineas, inicio);

	return 0;
}