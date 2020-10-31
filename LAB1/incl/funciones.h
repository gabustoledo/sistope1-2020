#ifndef FUNCIONES_H
#define FUNCIONES_H

#define TRUE 1
#define FALSE 0

int detectar(char linea[], char *cadena, int largoCadena);
void openFile(char *nameFile, int inicio, int lineas, int largoCadena, char *cadena, int *encontrado);
void writeFile(char *nombre, int *encontrado, int lineas);
void readRP(char *nameFile, int *resultados, int lineaActual, int lineasLeer);
void writeRF(char *nameFileInput, char *nameFileOutput, int numeroLineas, int *resultados, int flagD);
int cadenaValida(char *cadena);
int archivoValido(char *nombreArchivo);
int validacionEntradas(char *nombreArchivo, int procesos, int lineas, char *cadena);

#endif