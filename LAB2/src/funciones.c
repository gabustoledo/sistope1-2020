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

void *principal(void *input){
	struct thread_data *myData = (struct thread_data *) input;
	int nivelActual = myData->nivel;
	int iActual = myData->i;
	int jActual = myData->j;
	int anchoActual = myData->ancho;

	if(nivelActual < nivelMax){ // Debe crear 4 hebras de un nivel mayor
		pthread_t tid[4];
		pthread_attr_t attr;
		struct thread_data inputHebras[4];

		// Crear 4 hebras hijas
		// struct thread_data hebra1;
		inputHebras[0].nivel = nivelActual + 1;
		inputHebras[0].i = iActual;
		inputHebras[0].j = jActual;
		inputHebras[0].ancho = anchoActual/2;

		// struct thread_data hebra2;
		inputHebras[1].nivel = nivelActual + 1;
		inputHebras[1].i = iActual;
		inputHebras[1].j = jActual + anchoActual/2;
		inputHebras[1].ancho = anchoActual/2;

		// struct thread_data hebra3;
		inputHebras[2].nivel = nivelActual + 1;
		inputHebras[2].i = iActual + anchoActual/2;
		inputHebras[2].j = jActual;
		inputHebras[2].ancho = anchoActual/2;

		// struct thread_data hebra4;
		inputHebras[3].nivel = nivelActual + 1;
		inputHebras[3].i = iActual + anchoActual/2;
		inputHebras[3].j = jActual + anchoActual/2;
		inputHebras[3].ancho = anchoActual/2;

		pthread_attr_init(&attr);
		for (int i = 0; i < 4; i++)
			pthread_create(&(tid[i]), &attr, principal, (void *) &inputHebras[i]);

		// Esperar 4 hebras hijas
		int *outputHebra1 = (int *) malloc (bins * sizeof(int));
		int *outputHebra2 = (int *) malloc (bins * sizeof(int));
		int *outputHebra3 = (int *) malloc (bins * sizeof(int));
		int *outputHebra4 = (int *) malloc (bins * sizeof(int));

		pthread_join(tid[0], (void *)&outputHebra1);
		pthread_join(tid[1], (void *)&outputHebra2);
		pthread_join(tid[2], (void *)&outputHebra3);
		pthread_join(tid[3], (void *)&outputHebra4);

		// Sumas 4 arreglos devueltos
		int *output = (int *) malloc (bins * sizeof(int));
		for (int i = 0; i < bins; i++){
			output[i] = outputHebra1[i] + outputHebra2[i] + outputHebra3[i] + outputHebra4[i];
		}	

		// Retornar la suman de los arreglo
		pthread_exit((void *)output);

	}else if(nivelActual == nivelMax){ // Debe calcular el histograma y devolverlo a la hebra madre
		// Pasar a gris parte de la imagen
		int **gris = (int **) malloc (anchoActual * sizeof(int *));
		for (int i=0 ; i < anchoActual ; i++)
    	gris[i] = (int *) malloc (anchoActual * sizeof(int));
		
		gris = RGBtoGris(iActual, jActual, anchoActual);

		// Calcular histograma de esa parte
		int *histo = (int *) malloc (bins * sizeof(int));
		histo = calculoHistograma(gris, anchoActual);

		// retornar arreglo de histograma
		pthread_exit((void *)histo);
	}else{
		printf("Error en la ejecucion, nivel maximo sobre pasado.\n\n");
		exit(0);
	}
	pthread_exit(0);
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

	if(!archivo){
		printf("No se pudo abrir el archivo %s.\n",nombre);
		exit(0);
	}

	fseek(archivo, cabeceraIMG.offset, SEEK_SET);
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

int **RGBtoGris(int iActual, int jActual, int ancho){
	int **gris = (int **) malloc (ancho * sizeof(int *));
	for (int i=0 ; i < ancho ; i++)
    gris[i] = (int *) malloc (ancho * sizeof(int));

	int l = 0;
	int k = 0;
	for (int i = iActual; i < iActual+ancho; i++){
		for (int j = jActual; j < jActual+ancho; j++){
			gris[l][k] = R[i][j]*0.3 + G[i][j]*0.59 + B[i][j]*0.11;
			k++;
		}
		k = 0;
		l++;
	}
	return gris;
}

int *calculoHistograma(int **img, int ancho){
	int *histo = (int *) malloc (bins * sizeof(int));
	int rango = 256/bins;

	for (int i = 0; i < bins; i++)
		histo[i] = 0;	

	int l = 0;
	for (int k = 0; k < 256; k+=rango){
		for (int i = 0; i < ancho ; i++){
			for (int j = 0; j < ancho; j++){
				if(img[i][j]>=k && img[i][j]<=k+rango-1)
					histo[l]++;
			}
		}
		l++;
	}
	
	return histo;
}