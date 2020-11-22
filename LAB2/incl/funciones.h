#ifndef FUNCIONES_H
#define FUNCIONES_H

#define TRUE 1
#define FALSE 0

struct thread_data{
  int	nivel;
};

int nivelMax;

void argumentos(char *imagenEntrada, char *archivoSalida, int niveles, int bins);
int validacionEntradas(char *imagenEntrada, int niveles, int bins);
int imagenValida(char *imagenEntrada);
int binValido(int bin);
void *prueba( void * n);

#endif