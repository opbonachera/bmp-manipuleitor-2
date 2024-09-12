#ifndef FUNCIONES_BONACHERA_H_INCLUDED
#define FUNCIONES_BONACHERA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "estructuras.h"

bool validarRango(int limiteInferior, int limiteSuperior, int valor);
int  leerCabecera(FILE* file, t_metadata *cabecera);
int  obtenerParametro(char* argumento);
int  rotarImagenIzquierda(FILE* img, char* nombreNuevoArchivo);
int  rotarImagenDerecha(FILE* img, char* nombreNuevoArchivo);
int  espejarImagenHorizontal(FILE* imagenOriginal, char* nombreNuevoArchivo);
int  espejarImagenVertical(FILE* imagenOriginal, char* nombreNuevoArchivo);
void modificarDimensiones(FILE* img, int nuevoX, int nuevoY);

#endif // FUNCIONES_BONACHERA_H_INCLUDED
