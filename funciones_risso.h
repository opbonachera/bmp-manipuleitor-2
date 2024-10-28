#ifndef FUNCIONES_RISSO_H_INCLUDED
#define FUNCIONES_RISSO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "funciones_bonachera.h"

#define VERTICAL 'V'
#define HORIZONTAL 'H'

int  aumentarContraste(FILE* img, char* nombreNuevoArchivo, int parametro);
int reducirContraste(FILE* img, char* nombreNuevoArchivo, int parametro);
void rellenarMatrizDeRojo(int altura,int ancho, t_pixel** matriz);
void concatenarVeryHor(t_metadata* cabecera1, t_metadata* cabecera2, t_pixel** matNueva, t_pixel** matImagenUno, t_pixel** matImagenDos, char concatenar);
void asignarAltoyAnchoParaVerOHori(int* alto, int* ancho, t_metadata* cabecera1, t_metadata* cabecera2, char concatenar);
int concatenarImagenesVertioHori(FILE* img1, FILE* img2, char* nombreNuevoArchivo, char concatenar);

#endif // FUNCIONES_RISSO_H_INCLUDED
