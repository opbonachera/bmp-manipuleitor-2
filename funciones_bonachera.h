#ifndef FUNCIONES_BONACHERA_H_INCLUDED
#define FUNCIONES_BONACHERA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define OK 0
#define ERROR_APERTURA_ARCHIVO 1
#define ERROR_CREACION_ARCHIVO 2
#define FUERA_DE_RANGO -1
#define SIN_PARAMETROS 4
#define FALTA_IMAGEN 5

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct
{
    unsigned char pixel[3];
}t_pixel;

typedef struct
{
    unsigned int tamArchivo;
    unsigned int tamEncabezado;
    unsigned int comienzoImagen;
    unsigned int ancho;
    unsigned int alto;
    unsigned short profundidad;
}t_metadata;

bool validarRango(int limiteInferior, int limiteSuperior, int valor);
int  leerCabecera(FILE* file, t_metadata *cabecera);
int  obtenerParametro(char* argumento);
int  rotarImagenIzquierda(FILE* img, char* nombreNuevoArchivo);
int  rotarImagenDerecha(FILE* img, char* nombreNuevoArchivo);
int  espejarImagenHorizontal(FILE* imagenOriginal, char* nombreNuevoArchivo);
int  espejarImagenVertical(FILE* imagenOriginal, char* nombreNuevoArchivo);
int  invertirImagen(FILE* imagenOriginal, char* nombreNuevoArchivo);
int  recortarImagen(FILE* imagenOriginal, char* nombreNuevoArchivo, int parametro);
int  achicarImagen(FILE* imagenOriginal, char* nombreNuevoArchivo, int parametro);
int  pixelearImagen(FILE* imagenOriginal, char* nombreNuevoArchivo);

void modificarDimensiones(FILE* img, int nuevoX, int nuevoY);

#endif // FUNCIONES_BONACHERA_H_INCLUDED
