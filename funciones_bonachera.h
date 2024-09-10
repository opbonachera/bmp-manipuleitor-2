#ifndef FUNCIONES_BONACHERA_H_INCLUDED
#define FUNCIONES_BONACHERA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "estructuras.h"

int  leerCabecera(FILE* file, t_metadata *cabecera);
int  obtenerParametro(char* argumento);
bool validarRango(int limiteInferior, int limiteSuperior, int valor);
int  rotarImagenIzquierda(FILE* img, char* nombreNuevoArchivo);

#endif // FUNCIONES_BONACHERA_H_INCLUDED
