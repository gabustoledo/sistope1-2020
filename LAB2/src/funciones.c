#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../incl/funciones.h"
#include "../incl/globales.h"
#include "../incl/estructuras.h"

/* Entrada: Nombre de la imagen, cantidad de niveles, cantidad de bins.
 * Salida: TRUE -> 1, FALSE -> 0.
 * Funcion: Valida que la imagen exista, comprueba si niveles esta en el rango deseado,
 *          valida si el bins es potencia de 2.
*/
int validacionEntradas(char *imagenEntrada, int niveles, int bins){
	if(imagenValida(imagenEntrada) && niveles >= 0 && niveles <= 8 && binValido(bins))
		return TRUE;
	return FALSE;
}

/* Entrada: Nombre de la imagen.
 * Salida: TRUE -> 1, FALSE -> 0.
 * Funcion: Comprueba que la imagen exista, lo hace tratando de abrirla, si falla es FALSE, de lo contrario TRUE.
*/
int imagenValida(char *imagenEntrada){
	if(esBMP(imagenEntrada)){
		FILE *img = fopen(imagenEntrada, "r");
		if(!img)
			return FALSE;
		fclose(img);
		return TRUE;
	}
	return FALSE;
}

/* Entrada: Cantidad de bin.
 * Salida: TRUE -> 1, FALSE -> 0.
 * Funcion: Divide el numero para comprobar que es potencia de 2.
*/
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

/* Entrada: Estructura para pasar informacion a hebra hija.
 * Salida: Retorno a la hebra madre el histograma calculado.
 * Funcion: Si el nivel de la hebra es menor al maximo, genera 4 hebras e indica la seccion de la imagen que deben leer,
 *          las espera y suma las respuesta que le enviaron. Si la hebra es de nivel maximo, pasa a gris su parte de la imagen,
 *          con esa respuesta calcula el histograma parcial y lo devuelve a la hebra madre.
*/
void *principal(void *input){
	thread_data *myData = (thread_data *) input;
	int nivelActual = myData->nivel;
	int iActual = myData->i;
	int jActual = myData->j;
	int anchoActual = myData->ancho;

	if(nivelActual < nivelMax){ // Debe crear 4 hebras de un nivel mayor
		pthread_t tid[4];
		pthread_attr_t attr;
		thread_data inputHebras[4];

		// Crear 4 input para hebras hijas
		inputHebras[0].nivel = nivelActual + 1;
		inputHebras[0].i = iActual;
		inputHebras[0].j = jActual;
		inputHebras[0].ancho = anchoActual/2;

		inputHebras[1].nivel = nivelActual + 1;
		inputHebras[1].i = iActual;
		inputHebras[1].j = jActual + anchoActual/2;
		inputHebras[1].ancho = anchoActual/2;

		inputHebras[2].nivel = nivelActual + 1;
		inputHebras[2].i = iActual + anchoActual/2;
		inputHebras[2].j = jActual;
		inputHebras[2].ancho = anchoActual/2;

		inputHebras[3].nivel = nivelActual + 1;
		inputHebras[3].i = iActual + anchoActual/2;
		inputHebras[3].j = jActual + anchoActual/2;
		inputHebras[3].ancho = anchoActual/2;

		// Se crean las 4 hebras con su respectivo input
		pthread_attr_init(&attr);
		for (int i = 0; i < 4; i++)
			pthread_create(&(tid[i]), &attr, principal, (void *) &inputHebras[i]);

		// Se reserva memoria para la respuesta de las hebras
		int *outputHebra1 = (int *) malloc (bins * sizeof(int));
		int *outputHebra2 = (int *) malloc (bins * sizeof(int));
		int *outputHebra3 = (int *) malloc (bins * sizeof(int));
		int *outputHebra4 = (int *) malloc (bins * sizeof(int));

		// Se esperan las 4 hebras
		pthread_join(tid[0], (void *)&outputHebra1);
		pthread_join(tid[1], (void *)&outputHebra2);
		pthread_join(tid[2], (void *)&outputHebra3);
		pthread_join(tid[3], (void *)&outputHebra4);

		// Se suma los 4 resultados recibidos
		int *output = (int *) malloc (bins * sizeof(int));
		for (int i = 0; i < bins; i++)
			output[i] = outputHebra1[i] + outputHebra2[i] + outputHebra3[i] + outputHebra4[i];

		// Liberacion de memoria del output de las hebras.
		free(outputHebra1);
		free(outputHebra2);
		free(outputHebra3);
		free(outputHebra4);

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

		// Se libera memoria de gris
		for (int i=0 ; i < anchoActual ; i++)
    	free(gris[i]);
		free(gris);

		// retornar arreglo de histograma
		pthread_exit((void *)histo);
	}else{
		printf("Error en la ejecucion, nivel maximo sobre pasado.\n\n");
		exit(0);
	}
	pthread_exit(0);
}

/* Entrada: Nombre de la imagen.
 * Salida: Estructura con los datos de la cabecera de la imagen.
 * Funcion: Abre la imagen, y comienza a leer cada dato de la cabecera, guardandolo en la estructura para la cabecera.
*/
cabeceraBMP lecturaCabecera(char *nombre){

	FILE *archivo = fopen(nombre, "r");
	cabeceraBMP cabecera;

	if(!archivo){
		printf("No se pudo abrir el archivo %s.\n",nombre);
		exit(0);
	}

	// Cada elemento de la cabcera de la imagen es leido.
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

/* Entrada:
 * Salida:
 * Funcion: Reserva memoria para la matrices globales de RGB, segun el ancho de la imagen en la cabecera.
*/
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

/* Entrada: Nombre de la imagen.
 * Salida: 
 * Funcion: Abre la imagen, posiciona el cursor en el inicio de los datos de la imagen, 
 *          cada bytes leido es asignado a su matriz en el orden BGR.
*/
void lecturaImagen(char *nombre){

	FILE *archivo = fopen(nombre, "r");

	if(!archivo){
		printf("No se pudo abrir el archivo %s.\n",nombre);
		exit(0);
	}

	// Se avanza hasta donde comienzan los datos de los pixeles.
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

/* Entrada: Nombre del archivo de salida, arreglo con los resultados del histograma.
 * Salida:
 * Funcion: Abre o crea el archivo de salida, por cada elemento del histograma, lo escribe en el archivo,
 *          mientras el rango va variando segun los bins deseados.
*/
void escrituraArchivo(char *nombre, int *histograma){
	FILE *archivo = fopen(nombre, "w");
	int rango = 256/bins;

	if(!archivo){
		printf("No se pudo crear el archivo de salido %s.\n",nombre);
		exit(0);
	}

	int j = 0;
	for (int i = 0; i < 256; i+=rango){
		fprintf(archivo,"[%d,\t%d]\t%d\n",i,i+rango-1,histograma[j]);
		j++;
	}
	fclose(archivo);
}

/* Entrada: I inicial, J inicial, ancho de la seccion.
 * Salida: Matriz con los valores de la imagen en gris.
 * Funcion: Crea una matriz del ancho indicado, para cada elemento de esta matriz calcula el valor
 *          gris segun lo que contengan las matrices RGB.
*/
int **RGBtoGris(int iActual, int jActual, int ancho){
	int **gris = (int **) malloc (ancho * sizeof(int *));
	for (int i=0 ; i < ancho ; i++)
    gris[i] = (int *) malloc (ancho * sizeof(int));

	int l = 0;
	int k = 0;
	for (int i = iActual; i < iActual+ancho; i++){
		for (int j = jActual; j < jActual+ancho; j++){
			// Se utiliza la formula proporcionada
			gris[l][k] = R[i][j]*0.3 + G[i][j]*0.59 + B[i][j]*0.11;
			k++;
		}
		k = 0;
		l++;
	}
	return gris;
}

/* Entrada: Seccion de la imagen en gris, ancho de la imagen.
 * Salida: Arreglo con el histograma calculado segun la cantidad de bins.
 * Funcion: Para cada rango segun los bins, comprueba en todos los elementos de la imagen
 *          si el valor leido se encuentra en el rango buscado, de ser ciero le suma uno a esa
 *          posicion del histograma.
*/
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

/* Entrada: Nombre del archivo de la imagen.
 * Salida: TRUE -> 1, FALSE -> 0.
 * Funcion: Comprueba que la parte final del string sea ".bmp", para evitar que se entre otro tipo de archivo.
*/
int esBMP(char *nombre){
	int largo = strlen(nombre);
	if(nombre[largo-4]=='.' && nombre[largo-3]=='b' && nombre[largo-2]=='m' && nombre[largo-1]=='p'){
		return TRUE;
	}
	return FALSE;
}
