#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../incl/funciones.h"

/*
 * Input: Cadena de caracteres a analizar, cadena que se desea buscar, largo de la cadena a buscar.
 * Output: 1 -> en caso de encontrar la cadena, 0 -> en caso de no encontrar la cadena.
 * Function: Toma una porcion de la linea del mismo largo de la cadena que se busca,
 *           la compara con la cadena buscada, si son iguales termina de inmediato su
 *           operacion y retorna 1. De no encontrarse en ningun momento la cadena retorna 0.
*/
int detectar(char linea[], char *cadena, int largoCadena){
	char *cadenaComp = (char *)malloc(largoCadena * sizeof(char));

	for (int i = 0; i <= (strlen(linea) - largoCadena); i++){
		for (int j = 0; j < largoCadena; j++){
			cadenaComp[j] = linea[i + j];
		}
		if (strcmp(cadenaComp, cadena) == 0)
			return 1;
	}
	return 0;
}

/*
 * Input: Nombre del archivo, linea de partida, cantidad de lineas a leer, largo de la cadena buscada,
 *        cadena que se busca, arreglo de enteros que indica si se ha encontrado o no la cadena.
 * Output: Vacio.
 * Function: Avanza el puntero hasta la linea de partida, ahi comienza a leer la cantidad de lineas indicadas,
 *           cada linea es analizada con la funcion "detectar()", y su retorno es almacenado en "encontrado".
*/
void openFile(char *nameFile, int inicio, int lineas, int largoCadena, char *cadena, int *encontrado){
	FILE *archivo;
	char buffer[200] = {};

	archivo = fopen(nameFile, "r");

	if (archivo == NULL){
		printf("\nError de apertura del archivo lectura %s. \n\n", nameFile);
		exit(0);
	}else{
		// Seccion para avanzar el puntero hasta donde hay que comenzar.
		for (int i = 0; i < inicio; i++)
			fscanf(archivo, "%s", buffer);

		// Seccion para leer las lineas correspondientes al proceso.
		for (int i = inicio; i < (inicio + lineas); i++){
			fscanf(archivo, "%s", buffer);
			encontrado[i - inicio] = detectar(buffer, cadena, largoCadena);
		}
		fclose(archivo);
	}
}

/*
 * Input: Nombre del archivo, arreglo de enteros que indica si se ha encontrado o no la cadena en cada linea,
 *        cantidad de lineas que fueron leidas.
 * Output: Vacio.
 * Function: El contenido de "encontrado" es escrito en el archivo, son solo 0 y 1, que indican si se encontro
 *           la cadena en cada linea.
*/
void writeFile(char *nombre, int *encontrado, int lineas){
	FILE *archivo;

	archivo = fopen(nombre, "w+");

	if (archivo == NULL){
		printf("\nError de apertura del archivo escritura %s. \n\n", nombre);
		exit(0);
	}else{
		for (int i = 0; i < lineas; i++){
			fprintf(archivo, "%d", encontrado[i]);
			fprintf(archivo, "%s", "\n");
		}
		fclose(archivo);
	}
}

//COMPARADOR
//------------------
// COORDINADOR

/*
 * Input: Nombre del archivo, arreglo para almacenar los resultados, linea que sera leida, cantidad de lineas a leer.
 * Output: Vacio.
 * Function: El contenido del archivo son 0 y 1, lo que indica si fue encontrada la cadena en ciertas lineas,
 *           el contenido del archivo el pasado completamente al arreglo "resultados" en las posiciones
 *           indicadas con "lineaActual".
*/
void readRP(char *nameFile, int *resultados, int lineaActual, int lineasLeer){

	FILE *archivo;
	char buffer[1];
	char salto[1];
	int result = 0;

	archivo = fopen(nameFile, "r");

	if (archivo == NULL){
		printf("\nError de apertura del archivo lectura %s. \n\n", nameFile);
		exit(0);
	}
	else{
		// cada linea es leida, y su contenido es almacenado en "resultados".
		for (int i = 0; i < lineasLeer; i++){
			fread(buffer, sizeof(char), 1, archivo); // Contenido (0-1)
			fread(salto, sizeof(char), 1, archivo);	 // Salto de linea

			result = atof(buffer);
			resultados[lineaActual + i] = result;
		}
		fclose(archivo);
	}
}

/*
 * Input: Nombre del archivo de entrada, nombre del archivo a escribir los resultados,
 *        cantidad de lineas del archivo de entrada, arreglo con los resultados finales.
 * Output: Vacio.
 * Function: Lee cada linea del archivo de entrada, la copia directamente al archivo de salida,
 *           antes es pasar a leer la siguiente linea, comprueba el contenido de "resultados",
 *           si hay un 0 escribe un NO, si hay un 1 escribe un Si al final de la linea, y pasa
 *           a leer la siguiente linea.
*/
void writeRF(char *nameFileInput, char *nameFileOutput, int numeroLineas, int *resultados, int flagD){

	FILE *archivoInput;
	FILE *archivoOutput;
	char buffer[200] = {};

	archivoInput = fopen(nameFileInput, "r");
	archivoOutput = fopen(nameFileOutput, "w+");

	if (archivoInput == NULL || archivoOutput == NULL){
		printf("\nError de apertura del archivo lectura y escritura %s %s. \n\n", nameFileInput, nameFileOutput);
		exit(0);
	}
	else{
		// Cada linea es leida y escrita en el archivo de salida, antes de pasar a la siguiente linea
		// se comprueba el contenido de "resutlados" para poner Si o NO.
		for (int i = 0; i < numeroLineas; i++){
			fscanf(archivoInput, "%s", buffer);

			fprintf(archivoOutput, "%s", buffer);
			if (flagD == 1)
				printf("%s", buffer);
			if (resultados[i] == 0){
				fprintf(archivoOutput, "%s", "    NO\n");
				if (flagD == 1)
					printf("    NO\n");
			}else{
				fprintf(archivoOutput, "%s", "    SI\n");
				if (flagD == 1)
					printf("    SI\n");
			}
		}

		fclose(archivoInput);
		fclose(archivoOutput);
	}
}