#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../incl/funciones.h"



void argumentos(char *imagenEntrada, char *archivoSalida, int niveles, int bins){
	printf("Archivo de entrada: %s\n", imagenEntrada);
	printf("Archivo de salida: %s\n", archivoSalida);
	printf("Cantidad de niveles: %d\n", niveles);
	printf("Cantidad de bins: %d\n", bins);
}

int validacionEntradas(char *imagenEntrada, int niveles, int bins){
	if(imagenValida(imagenEntrada) && niveles >= 0 && niveles <= 8 && binValido(bins))
		return TRUE;
	return FALSE;
}

int imagenValida(char *imagenEntrada){	
	FILE *img = fopen(imagenEntrada, "r");
	if(!img)
		return FALSE;
	fclose(img);
	return TRUE;
}

int binValido(int bin){
	int flag = TRUE;

	if(bin<1 || bin>256)
		return FALSE;

	while(flag){
		if(bin%2 == 0)
			bin = bin/2;
		else if(bin == 1)
			return TRUE;
		else
			flag = FALSE;
	}
	return FALSE;
}

void *prueba(void * data){

	int *output = (int *) malloc (bins * sizeof(int));
	for (int i = 0; i < bins; i++){
		output[i] = i;
	}	

	struct thread_data *myData = (struct thread_data *) data;
	int nivel = myData->nivel;

	if(nivel < nivelMax)
		printf("\nHay que divirid mas\n");
	else
		printf("tamo bien aqui\n");

	printf("\nPrueba de funcion de un proceso %d.\n\n", nivel);
	pthread_exit((void *)output);
}

cabeceraBMP lecturaCabecera(char *nombre){

	FILE *archivo = fopen(nombre, "r");
	cabeceraBMP cabecera;

	if(!archivo){
		printf("No se pudo abrir el archivo %s.\n",nombre);
		exit(0);
	}

	fseek(archivo,0, SEEK_SET);
	fread(&cabecera.bm,sizeof(char),2,archivo);
	fread(&cabecera.tamano,sizeof(int),1,archivo);
	fread(&cabecera.reservado,sizeof(int),1,archivo);
	fread(&cabecera.offset,sizeof(int),1,archivo);
	fread(&cabecera.tamanoMetadatos,sizeof(int),1,archivo);
	fread(&cabecera.alto,sizeof(int),1,archivo);
	fread(&cabecera.ancho,sizeof(int),1,archivo);
	fread(&cabecera.numeroPlanos,sizeof(short int),1,archivo);
	fread(&cabecera.profundidadColor,sizeof(short int),1,archivo);
	fread(&cabecera.tipoCompresion,sizeof(int),1,archivo);
	fread(&cabecera.tamanoEstructura,sizeof(int),1,archivo);
	fread(&cabecera.pxmh,sizeof(int),1,archivo);
	fread(&cabecera.pxmv,sizeof(int),1,archivo);
	fread(&cabecera.coloresUsados,sizeof(int),1,archivo);
	fread(&cabecera.coloresImportantes,sizeof(int),1,archivo);

	fclose(archivo);
	return cabecera;
}

void memoriaRGB(){
	R = (int **) malloc (cabeceraIMG.ancho * sizeof(int *));
	G = (int **) malloc (cabeceraIMG.ancho * sizeof(int *));
	B = (int **) malloc (cabeceraIMG.ancho * sizeof(int *));

	for (int i=0 ; i < cabeceraIMG.ancho ; i++) {
    R[i] = (int *) malloc (cabeceraIMG.ancho * sizeof(int));
		G[i] = (int *) malloc (cabeceraIMG.ancho * sizeof(int));
		B[i] = (int *) malloc (cabeceraIMG.ancho * sizeof(int));
  }
}

void lecturaImagen(char *nombre){

	FILE *archivo = fopen(nombre, "r");
	cabeceraBMP cabecera;

	if(!archivo){
		printf("No se pudo abrir el archivo %s.\n",nombre);
		exit(0);
	}

	fseek(archivo,0, SEEK_SET);
	fread(&cabecera.bm, sizeof(char),2,archivo);
	fread(&cabecera.tamano, sizeof(int),1,archivo);
	fread(&cabecera.reservado, sizeof(int),1,archivo);
	fread(&cabecera.offset, sizeof(int),1,archivo);
	fread(&cabecera.tamanoMetadatos, sizeof(int),1,archivo);
	fread(&cabecera.alto, sizeof(int),1,archivo);
	fread(&cabecera.ancho, sizeof(int),1,archivo);
	fread(&cabecera.numeroPlanos, sizeof(short int),1,archivo);
	fread(&cabecera.profundidadColor,sizeof(short int),1,archivo);
	fread(&cabecera.tipoCompresion,sizeof(int),1,archivo);
	fread(&cabecera.tamanoEstructura,sizeof(int),1,archivo);
	fread(&cabecera.pxmh,sizeof(int),1,archivo);
	fread(&cabecera.pxmv,sizeof(int),1,archivo);
	fread(&cabecera.coloresUsados,sizeof(int),1,archivo);
	fread(&cabecera.coloresImportantes,sizeof(int),1,archivo);

	fseek(archivo, cabecera.offset, SEEK_SET);
	for (int i = 0; i < cabeceraIMG.ancho; i++){
		for (int j = 0; j < cabeceraIMG.ancho; j++){
			fread(&B[i][j], sizeof(char), 1, archivo);
			fread(&G[i][j], sizeof(char), 1, archivo);
			fread(&R[i][j], sizeof(char), 1, archivo);
		}
	}

	fclose(archivo);
}

void escrituraArchivo(char *nombre, int *histograma){
	FILE *archivo = fopen(nombre, "w");
	int update = 256/bins;

	if(!archivo){
		printf("No se pudo crear el archivo de salido %s.\n",nombre);
		exit(0);
	}

	int j = 0;
	for (int i = 0; i < 256; i+=update){
		fprintf(archivo,"[%d,\t%d]\t%d\n",i,i+update-1,histograma[j]);
		j++;
	}		
	fclose(archivo);
}