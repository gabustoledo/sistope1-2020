#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../incl/funciones.h"

int main(int argc, char **argv){

	char *archivoEntrada;
	int procesos;
	int numeroLineas;
	char *cadena;
	int flagD = 0;
	int c;
	int lineasPorProceso = 0;
	int *resultados;

	// Los argumentos de entrada son leidos y almacenados en su correspondinetes variables.
	while (((c = getopt(argc, argv, "i:n:c:p:d")) != -1)){
		switch (c){
		case 'i':
			archivoEntrada = optarg;

			// Se valida que el archivo ingresado si existe.
			FILE *archivo;
			archivo = fopen(archivoEntrada, "r");
			if (archivo == NULL){
				printf("\nError de apertura del archivo %s. \n\n", archivoEntrada);
				exit(0);
			}else
				fclose(archivo);

			break;

		case 'n':
			procesos = atof(optarg);
			if(procesos <= 0){
				printf("Numero de procesos ingresado es invalido.\n");
				return 0;
			}
			break;

		case 'c':
			numeroLineas = atof(optarg);
			if(numeroLineas <= 0){
				printf("Numero de lineas ingresado es invalido.\n");
				return 0;
			}
			break;

		case 'p':
			cadena = optarg;
			if(cadenaValida(cadena) == 0)				
				return 0;
			break;

		case 'd':
			flagD = 1;
			break;
		}
	}

	// Cantidad de lineas que debe leer cada proceso, suponiendo que siempre da un valor entero.
	lineasPorProceso = numeroLineas / procesos;

	// Contenido para funcion execv
	char *path[7] = {"./comparador", "-i", "-n", "-c", "-p", "-d", NULL};

	// Nombre de archivo para cada proceso
	char envNombreArchivo[50];
	strcpy(envNombreArchivo, "-i");
	strcat(envNombreArchivo, archivoEntrada);
	path[1] = envNombreArchivo;

	// Lineas por proceso
	char envLineas[50];
	char lineasStr[10];
	strcpy(envLineas, "-c");
	sprintf(lineasStr, "%d", lineasPorProceso);
	strcat(envLineas, lineasStr);
	path[3] = envLineas;

	// Cadena para cada proceso
	char envCadena[50];
	strcpy(envCadena, "-p");
	strcat(envCadena, cadena);
	path[4] = envCadena;

	// Inicio para cada proceso
	int inicioAux = 0;
	char envInicio[50];
	strcpy(envInicio, "-n");

	// id para cada proceso
	char envId[50];
	strcpy(envId, "-d");

	pid_t pid;
	pid_t *pidarray = (pid_t *)malloc(sizeof(pid_t) * procesos);
	for (int i = 0; i < procesos; i++){
		if ((pid = fork()) == 0){
			// Id para cada comparador
			char idStr[10];
			sprintf(idStr, "%d", i);
			strcat(envId, idStr);
			path[5] = envId;

			// Inicio para cada comparador
			char inicioStr[10];
			sprintf(inicioStr, "%d", inicioAux);
			strcat(envInicio, inicioStr);
			path[2] = envInicio;

			execv(path[0], path);
		}
		pidarray[i] = pid;
		inicioAux += lineasPorProceso;
	}

	// Proceso padre espera a todos sus procesos hijos.
	if (flagD == 1)
		printf("\n\n");
	for (int i = 0; i < procesos; i++){
		waitpid(pidarray[i], 0, 0);
		if (flagD == 1)
			printf("Proceso %d ha terminado su operacion\n", pidarray[i]);
	}
	if (flagD == 1)
		printf("\n\n");

	// Los resultados son leido en todos los archivos, y son escritos en "resultados".
	resultados = (int *)malloc(sizeof(int) * numeroLineas);
	char fileRP[50];
	// Por cada proceso que fue creado.
	for (int i = 0; i < procesos; i++){

		// Se elige el id a revisar.
		char idStr[10];
		sprintf(idStr, "%d", i);

		// Se genera el nombre de archivo que escribio cada proceso.
		strcpy(fileRP, "rp/rp_");
		strcat(fileRP, cadena);
		strcat(fileRP, "_");
		strcat(fileRP, idStr);
		strcat(fileRP, ".txt");

		// El archivo es leido, y su contenido queda en "resultados".
		readRP(fileRP, resultados, i * lineasPorProceso, lineasPorProceso);
	}

	// Se genera el nombre de archivo para la salida.
	char archivoSalida[100];
	strcpy(archivoSalida, "rc_");
	strcat(archivoSalida, cadena);
	strcat(archivoSalida, ".txt");

	// El archivo de salida es escrito con el resultado encontrado.
	writeRF(archivoEntrada, archivoSalida, numeroLineas, resultados, flagD);

	return 0;
}

/*
  ./sucesion -i entrada.txt -n 5 -c 20 -p AAAA -d
*/

/*
  al final hacer le modo debug

	preguntar si es solo que sera para ver los resultados finales o para ver lo que va haciendo
*/