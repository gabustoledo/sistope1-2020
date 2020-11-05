#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../incl/funciones.h"

/*
 * Input: Cadena de caracteres a analizar, cadena que se desea buscar, largo de la cadena a buscar.
 * Output: 1 -> en caso de encontrar la cadena, 0 -> en caso de no encontrar la cadena.
 * Function: Toma una porcion de la linea que sea del mismo largo de la cadena que se busca,
 *           la compara con la cadena buscada, si son iguales termina de inmediato su
 *           operacion y retorna 1. De no encontrarse en ningun momento la cadena retorna 0.
*/
int detectar(char linea[], char *cadena, int largoCadena){
	// Arreglo auxiliar para almacenar una porcion de la linea
	char *cadenaComp = (char *)malloc(largoCadena * sizeof(char));

	// Se recorre toda la linea
	for (int i = 0; i <= (strlen(linea) - largoCadena); i++){
		// Se crea la mini cadena que se compara con la cadena buscada.
		for (int j = 0; j < largoCadena; j++){
			cadenaComp[j] = linea[i + j];
		}
		// Es comparada si son iguales
		if (strcmp(cadenaComp, cadena) == 0)
			return 1; // Encontrada
	}
	return 0; // No encontrada
}

/*
 * Input: Nombre del archivo, linea de inicio, cantidad de lineas a leer, largo de la cadena buscada,
 *        cadena que se busca, arreglo de enteros que indica si se ha encontrado o no la cadena.
 * Output: Vacio.
 * Function: Funcion para el proceso comparador, avanza el puntero hasta la linea de partida, ahi comienza a leer la cantidad de lineas indicadas,
 *           cada linea es analizada con la funcion "detectar()", y su retorno es almacenado en "encontrado".
*/
void openFileRP(char nameFile[], int inicio, int lineas, int largoCadena, char cadena[], int *encontrado){
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
	// Por cada caracter
	for (int i = 0; i < strlen(cadena); i++){
		// Solo permite los caracteres A C G T
		if(cadena[i] != 'A' && cadena[i] != 'C' && cadena[i] != 'G' && cadena[i] != 'T')
			return FALSE;
	}
	return TRUE;
}

/*
 * Input: Nombre del archivo.
 * Output: TRUE o FALSE.
 * Function: Se trata de abrir el archivo ingresado, si es posible abrirlo, lo cierra y retorna TRUE, 
 *           de no poder abrirse retorna FALSE.
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
 * Function: Funcion para escribir el resultado parcial, el contenido de "encontrado" es escrito en el archivo, con un SI o NO.
*/
void writeFileRP(char *nombreArchivoSalida, char nombreArchivoEntrada[], int *encontrado, int lineas, int lineaInicio){
	FILE *archivoEntrada;
	FILE *archivoSalida;
	char buffer[200] = {};

	archivoSalida = fopen(nombreArchivoSalida, "w+");
	archivoEntrada = fopen(nombreArchivoEntrada, "r");

	if (archivoSalida == NULL || archivoEntrada == NULL){
		printf("\nError de apertura del archivo escritura %s. \n\n", nombreArchivoSalida);
		exit(0);
	}else{
		// Se avanza hasta la linea de inicio.
		for (int i = 0; i < lineaInicio; i++){
			fscanf(archivoEntrada, "%s", buffer);
		}

		// Por cada linea a leer.
		for (int i = 0; i < lineas; i++){

			// Se lee el contenido y es copiado al archivo de salida.
			fscanf(archivoEntrada, "%s", buffer);
			fprintf(archivoSalida, "%s", buffer);

			// Se le agrega un SI o NO.
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
 * Input: Nombre del archivo de salida, numero de lineas de archivo, bandera debug, cadena buscada, cantidad de procesos, arreglo con pid de procesos hijos.
 * Output: Vacio.
 * Function: Funcion para el proceso coordinador, lee cada archivo de respuesta parcial, y copia todo su contenido en el nuevo archivo, 
 *           asi con todos los archivo parciales para formar el resultado completo.
*/
void writeFileRC(char *nameFileOutput, int numeroLineas, int flagD, char *cadena, int procesos, int *pidArray){
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

		// Por cada proceso creado.
		for (int i = 0; i < procesos; i++){
			
			// Se genera el nombre del archivo parcial a leer, y se abre.
			strcpy(nombreArchivoEntrada, "rp/rp_");
			strcat(nombreArchivoEntrada, cadena);
			strcat(nombreArchivoEntrada, "_");
			sprintf(idStr, "%d", pidArray[i]);
			strcat(nombreArchivoEntrada, idStr);
			strcat(nombreArchivoEntrada, ".txt");
			archivoInput = fopen(nombreArchivoEntrada, "r");

			// Por cada linea en el archivo.
			for (int j = 0; j < lineasPorProceso; j++){
				// Caracteres del ADN, son leido y copiado totalmente.
				fscanf(archivoInput, "%s", buffer);
				fprintf(archivoOutput, "%s", buffer);

				if(flagD == 1)
					printf("%s",buffer);

				// Es copiada la respuesta SI o NO
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