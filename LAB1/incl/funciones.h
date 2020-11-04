#ifndef FUNCIONES_H
#define FUNCIONES_H

#define TRUE 1
#define FALSE 0

int detectar(char linea[], char *cadena, int largoCadena);
void openFileRP(char nameFile[], int inicio, int lineas, int largoCadena, char cadena[], int *encontrado);
int cadenaValida(char *cadena);
int archivoValido(char *nombreArchivo);
int validacionEntradas(char *nombreArchivo, int procesos, int lineas, char *cadena);
void writeFileRP(char *archivoSalida, char archivoEntrada[], int *encontrado, int lineas, int lineaInicio);
void writeFileRC(char *nameFileOutput, int numeroLineas, int flagD, char *cadena, int procesos, int *pidArray);

#endif