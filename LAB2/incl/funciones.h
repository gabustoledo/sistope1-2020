#include "estructuras.h"

#ifndef FUNCIONES_H
#define FUNCIONES_H

int validacionEntradas(char *imagenEntrada, int niveles, int bins);
int imagenValida(char *imagenEntrada);
int binValido(int bin);
void *principal(void *input);
cabeceraBMP lecturaCabecera(char *nombre);
void memoriaRGB();
void lecturaImagen(char *nombre);
void escrituraArchivo(char *nombre, int *histograma);
int **RGBtoGris(int iActual, int jActual, int ancho);
int *calculoHistograma(int **img, int ancho);
int esBMP(char *nombre);

#endif