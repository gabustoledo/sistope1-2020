#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../incl/funciones.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char **argv){

	// Argumentos de entrada.
	char archivoEntrada[50] = {};
	int inicio;
	int numeroLineas;
	char cadena[50] = {};
	char id[10] = {};

	int *encontrado;
	int largoCadena;
	char archivoSalida[100];

	// Es leido lo que escribe el padre en el pipe
	char leido[100];
	read(STDIN_FILENO, leido, 50*sizeof(char));
	strcpy(archivoEntrada, leido);

	read(STDIN_FILENO, leido, 20*sizeof(char));
	inicio = atof(leido);

	read(STDIN_FILENO, leido, 50*sizeof(char));
	numeroLineas = atof(leido);
	encontrado = (int *)malloc(numeroLineas * sizeof(int));

	read(STDIN_FILENO, leido, 10*sizeof(char));
	strcpy(id, leido);

	read(STDIN_FILENO, leido, 50*sizeof(char));
	strcpy(cadena, leido);
	largoCadena = strlen(cadena);

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