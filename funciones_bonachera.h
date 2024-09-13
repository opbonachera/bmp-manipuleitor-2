#ifndef FUNCIONES_BONACHERA_H_INCLUDED
#define FUNCIONES_BONACHERA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "estructuras.h"

#define OK 0
#define ERROR_APERTURA_ARCHIVO 1
#define ERROR_CREACION_ARCHIVO 2
#define FUERA_DE_RANGO -1
#define SIN_PARAMETROS 4

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

bool validarRango(int limiteInferior, int limiteSuperior, int valor);
int  leerCabecera(FILE* file, t_metadata *cabecera);
int  obtenerParametro(char* argumento);
int  rotarImagenIzquierda(FILE* img, char* nombreNuevoArchivo);
int  rotarImagenDerecha(FILE* img, char* nombreNuevoArchivo);
int  espejarImagenHorizontal(FILE* imagenOriginal, char* nombreNuevoArchivo);
int  espejarImagenVertical(FILE* imagenOriginal, char* nombreNuevoArchivo);
void modificarDimensiones(FILE* img, int nuevoX, int nuevoY);

#endif // FUNCIONES_BONACHERA_H_INCLUDED
