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
void openFileRP(char *nameFile, int inicio, int lineas, int largoCadena, char *cadena, int *encontrado){
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
 * Input: Cadena buscada.
 * Output: TRUE o FALSE.
 * Function: Compara cada caracter para encontrar un caracter no deseado.
*/
int cadenaValida(char *cadena){
	for (int i = 0; i < strlen(cadena); i++){
		if(cadena[i] != 'A' && cadena[i] != 'C' && cadena[i] != 'G' && cadena[i] != 'T')
			return FALSE;
	}
	return TRUE;
}

/*
 * Input: Nombre del archivo.
 * Output: TRUE o FALSE.
 * Function: Comprueba que el archivo ingresado si exista.
*/
int archivoValido(char *nombreArchivo){
	FILE *archivo;
	archivo = fopen(nombreArchivo, "r");
	if (archivo == NULL)
		return FALSE;
	else
		fclose(archivo);
	return TRUE;
}

/*
 * Input: Nombre del archivo, cantidad de procesos, numero de lineas, cadena buscada.
 * Output: TRUE o FALSE.
 * Function: Comprueba que los argumentos ingresados sean validos.
*/
int validacionEntradas(char *nombreArchivo, int procesos, int lineas, char *cadena){
	if(archivoValido(nombreArchivo) && cadenaValida(cadena) && procesos > 0 && lineas > 0)
		return TRUE;
	printf("\n\nEntradas invalidas, vuelva a intentar con valores correctos.\n\n");
	return FALSE;
}

/*
 * Input: Nombre del archivo de escritura, nombre del archivo de entrada, arreglo de encontrados, cantidad de lineas, linea de inicio.
 * Output: Vacio.
 * Function: El contenido de "encontrado" es escrito en el archivo, con un SI o NO.
*/
void writeFileRP(char *nombreArchivoSalida, char *nombreArchivoEntrada, int *encontrado, int lineas, int lineaInicio){
	FILE *archivoEntrada;
	FILE *archivoSalida;
	char buffer[200] = {};

	archivoSalida = fopen(nombreArchivoSalida, "w+");
	archivoEntrada = fopen(nombreArchivoEntrada, "r");

	if (archivoSalida == NULL || archivoEntrada == NULL){
		printf("\nError de apertura del archivo escritura %s. \n\n", nombreArchivoSalida);
		exit(0);
	}else{

		for (int i = 0; i < lineaInicio; i++){
			fscanf(archivoEntrada, "%s", buffer);
		}

		for (int i = 0; i < lineas; i++){
			fscanf(archivoEntrada, "%s", buffer);
			fprintf(archivoSalida, "%s", buffer);

			if (encontrado[i] == 0)
				fprintf(archivoSalida, "%s", "    NO\n");
			else
				fprintf(archivoSalida, "%s", "    SI\n");
		}
		fclose(archivoEntrada);
		fclose(archivoSalida);
		
	}
}

/*
 * Input: Nombre del archivo de salida, numero de lineas de archivo, bandera debug, cadena buscada, cantidad de procesos.
 * Output: Vacio.
 * Function: Lee cada archivo de respuesta parcial, y copia todo su contenido en el nuevo archivo, 
 *           asi con todos los archivo parciales para formar el resultado completo.
*/
void writeFileRC(char *nameFileOutput, int numeroLineas, int flagD, char *cadena, int procesos){
	FILE *archivoInput;
	FILE *archivoOutput;
	char buffer[200] = {};
	char nombreArchivoEntrada[100];
	char idStr[10];
	int lineasPorProceso = numeroLineas/procesos;

	archivoOutput = fopen(nameFileOutput, "w+");

	if (archivoOutput == NULL){
		printf("\nError de apertura del archivo lectura y escritura %s. \n\n", nameFileOutput);
		exit(0);
	}else{

		for (int i = 0; i < procesos; i++){
			
			strcpy(nombreArchivoEntrada, "rp/rp_");
			strcat(nombreArchivoEntrada, cadena);
			strcat(nombreArchivoEntrada, "_");
			sprintf(idStr, "%d", i);
			strcat(nombreArchivoEntrada, idStr);
			strcat(nombreArchivoEntrada, ".txt");

			archivoInput = fopen(nombreArchivoEntrada, "r");

			for (int j = 0; j < lineasPorProceso; j++){
				// Caracteres del ADN
				fscanf(archivoInput, "%s", buffer);
				fprintf(archivoOutput, "%s", buffer);

				if(flagD == 1)
					printf("%s",buffer);

				// Si o No
				fscanf(archivoInput, "%s", buffer);
				fprintf(archivoOutput, "    %s\n", buffer);	

				if(flagD == 1)
					printf("    %s\n",buffer);
			}		

			fclose(archivoInput);
		}		
		fclose(archivoOutput);
	}
}