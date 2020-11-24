#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../incl/funciones.h"

int main(int argc, char **argv){

	// Argumentos de entrada
	char *imagenEntrada = NULL;
	char *archivoSalida = NULL;

	// Variables para crear hebra 0
	pthread_t tid;
  pthread_attr_t attr;
	thread_data thread_data_array;

	// Para leer argumentos de entrada
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
			nivelMax = atof(optarg);
			break;
		case 'B':
			bins = atof(optarg);
			break;
		}
	}

	if(!validacionEntradas(imagenEntrada, nivelMax, bins)){
		printf("\nParametros ingresados no son validos.\n");
		return 0;
	}
	
	// Falta leer la imagen y dejarla de manera global
	cabeceraIMG = lecturaCabecera(imagenEntrada);

	// Se reseva espacio para las matrices de RGB
	memoriaRGB();

	// El contenido de la imagen es leido
	lecturaImagen(imagenEntrada);

	// Respuesta del nivel 0
	int *histograma = (int *) malloc (bins * sizeof(int));

	// Creacion de la hebra de nivel 0 
	thread_data_array.nivel = 0;
	thread_data_array.i = 0;
	thread_data_array.j = 0;
	thread_data_array.ancho = cabeceraIMG.ancho;

	pthread_attr_init(&attr);
  pthread_create(&tid, &attr, principal, (void *) &thread_data_array);	
  pthread_join(tid, (void *)&histograma);

	// Se escribe la respuesta en el archivo
	escrituraArchivo(archivoSalida, histograma);

	return 0;
}

// ./lab2 -i inputFiles/imagen_1.bmp -o histograma.txt -L 2 -B 256