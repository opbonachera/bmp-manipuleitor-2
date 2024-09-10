#ifndef FUNCIONES_FERNANDEZ_H_INCLUDED
#define FUNCIONES_FERNANDEZ_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "estructuras.h"

void cambiarTonalidad(FILE* img, char* imgNueva, unsigned char color,  char* modificador);
void escribirCabecera(FILE* img, FILE* nueva, t_metadata cabeceraOriginal);
void procesarArgumentos(int argc, char* argv[]);
void procesarArgumentos(int argc, char* argv[]);
bool esNombreDeArchivo(char* parametro);
bool esComando(char* parametro);
bool comandoFueProcesado(char* parametro, char* comandosProcesados[]);
void ejecutarComando(char* comando, char* nombresDeArchivos[]);
char* generarNombreArchivo(char* comando, char* nombresDeArchivos[]);

#endif // FUNCIONES_FERNANDEZ_H_INCLUDED
