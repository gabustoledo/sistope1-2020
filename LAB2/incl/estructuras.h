#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct thread_data{
  int	nivel;  // Nivel actual de la hebra
  int i;      // Posicion i donde debe comenzar a leer
  int j;      // Posicion j donde cebe comenzar a leer
  int ancho;  // Ancho de la porcion de la imagen que debe leer
}thread_data;

typedef struct cabeceraBMP{
  char bm[2];                  //(2 Bytes) BM (Tipo de archivo)
  int tamano;                  //(4 Bytes) Tamaño del archivo en bytes
  int reservado;               //(4 Bytes) Reservado
  int offset;                  //(4 Bytes) offset, distancia en bytes entre la img y los píxeles
  int tamanoMetadatos;         //(4 Bytes) Tamaño de Metadatos (tamaño de esta estructura = 40)
  int alto;                    //(4 Bytes) Ancho (numero de píxeles horizontales)
  int ancho;                   //(4 Bytes) Alto (numero de pixeles verticales)
  short int numeroPlanos;      //(2 Bytes) Numero de planos de color
  short int profundidadColor;  //(2 Bytes) Profundidad de color (debe ser 24 para nuestro caso)
  int tipoCompresion;          //(4 Bytes) Tipo de compresión (Vale 0, ya que el bmp es descomprimido)
  int tamanoEstructura;        //(4 Bytes) Tamaño de la estructura Imagen (Paleta)
  int pxmh;                    //(4 Bytes) Píxeles por metro horizontal
  int pxmv;                    //(4 Bytes) Píxeles por metro vertical
  int coloresUsados;           //(4 Bytes) Cantidad de colores usados 
  int coloresImportantes;      //(4 Bytes) Cantidad de colores importantes
}cabeceraBMP;

#endif