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

	// Entradas principales
	char *archivoEntrada = NULL;
	int procesos = 0;
	int numeroLineas = 0;
	char *cadena = NULL;
	int flagD = 0;

	int c;
	int lineasPorProceso = 0;

	// Se comprueba que se ingresaron argumentos suficientes.
	if(argc < 9 || argc > 10){
		printf("\n\nArgumentos ingresados no son la cantidad adecuada.\n\n");
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
	char lineasStr[50];
	sprintf(lineasStr, "%d", lineasPorProceso);

	// Contenido del path para funcion execv
	char *path[2] = {"./comparador", NULL};

	// Inicio para cada proceso
	int inicioAux = 0;

	// Pid y array de pid para guardar el id de cada proceso
	int status;
	pid_t pid;
	pid_t *pidarray = (pid_t *)malloc(sizeof(pid_t) * procesos);

	for (int i = 0; i < procesos; i++){

		// Es creado el pipe de comunicacion
		int *pipes = (int*)malloc(sizeof(int)*2);
		pipe(pipes);

		// Se crea el proceso hijo
		pid = fork();

		if(pid > 0){ // En caso de ser el padre, para escribir
			close(pipes[LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor

			// Linea de inicio
			char inicioStr[20];
			sprintf(inicioStr, "%d", inicioAux);

			// Id para el hijo
			char idStr[10];
			sprintf(idStr, "%d", i);
			
			// Son escritos los parametros para el proceso hijo
			write(pipes[ESCRITURA], archivoEntrada, 50*sizeof(char));
			write(pipes[ESCRITURA], inicioStr, 20*sizeof(char));
			write(pipes[ESCRITURA], lineasStr, 50*sizeof(char));
			write(pipes[ESCRITURA], idStr, 10*sizeof(char));
			write(pipes[ESCRITURA], cadena, 50*sizeof(char));

		}else if (pid == 0){ // En caso de ser le hijo para ejecutar el comparador
			
			close(pipes[ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
			dup2(pipes[LECTURA], STDIN_FILENO);

			execv(path[0], path);
		}
		pidarray[i] = pid;               // Los pid son guardados para esperarlos en el futuro.
		inicioAux += lineasPorProceso;   // La linea de inicio aumenta para cada proceso.
	}

	// Proceso padre espera a todos sus procesos hijos.
	if (flagD == 1)
		printf("\n\n");
	for (int i = 0; i < procesos; i++){
		waitpid(pidarray[i], &status, 0);
		if (flagD == 1)
			printf("Proceso %d ha terminado su operacion\n", pidarray[i]);
	}
	if (flagD == 1)
		printf("\n\n");

	// Se crea el nombre del archivo de salida.
	char archivoSalida[50];
	strcpy(archivoSalida, "rc_");
	strcat(archivoSalida, cadena);
	strcat(archivoSalida, ".txt");

	// El archivo de salida es escrito con el resultado encontrado.
	writeFileRC(archivoSalida, numeroLineas, flagD, cadena, procesos);

	return 0;
}

/*
  ./sucesion -i entrada.txt -n 5 -c 20 -p AAAA -d
*/