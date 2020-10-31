#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../incl/funciones.h"

int main(int argc, char **argv){

	// Entradas principales
	char *archivoEntrada = NULL;
	int procesos = 0;
	int numeroLineas = 0;
	char *cadena = NULL;
	int flagD = 0;

	int c;
	int lineasPorProceso = 0;
	int *resultados;

	if(argc < 9){
		printf("Argumentos ingresados son insuficientes.\n");
		return 0;
	}

	// Los argumentos de entrada son leidos y almacenados en su correspondinetes variables.
	while (((c = getopt(argc, argv, "i:n:c:p:d")) != -1)){
		switch (c){
		case 'i':
			archivoEntrada = optarg;
			break;
		case 'n':
			procesos = atof(optarg);
			break;
		case 'c':
			numeroLineas = atof(optarg);
			break;
		case 'p':
			cadena = optarg;
			break;
		case 'd':
			flagD = 1;
			break;
		}
	}

	// Se validan las entradas ingresadas.
	if(!validacionEntradas(archivoEntrada, procesos, numeroLineas, cadena))
		return 0;

	// Cantidad de lineas que debe leer cada proceso, suponiendo que siempre da un valor entero.
	lineasPorProceso = numeroLineas / procesos;

	// Lineas por proceso a string para path
	char lineasStr[10];
	sprintf(lineasStr, "%d", lineasPorProceso);

	// Contenido para funcion execv
	char *path[11] = {};
	path[0] = "./comparador";
	path[1] = "-i";
	path[2] = archivoEntrada;
	path[3] = "-n";
	path[5] = "-c";
	path[6] = lineasStr;
	path[7] = "-p";
	path[8] = cadena;
	path[9] = "-d";
	path[11] = NULL;

	// Inicio para cada proceso
	int inicioAux = 0;

	pid_t pid;
	pid_t *pidarray = (pid_t *)malloc(sizeof(pid_t) * procesos);

	for (int i = 0; i < procesos; i++){

		if ((pid = fork()) == 0){
			// Inicio para cada comparador
			char inicioStr[10];
			sprintf(inicioStr, "%d", inicioAux);
			path[4] = inicioStr;
	
			// Id para cada comparador
			char idStr[10];
			sprintf(idStr, "%d", i);
			path[10] = idStr;

			execv(path[0], path);
		}
		pidarray[i] = pid;               // Los pid son guardados para esperarlos en el futuro.
		inicioAux += lineasPorProceso;   // La linea de inicio aumenta para cada proceso.
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