#ifndef FUNCIONES_FERNANDEZ_H_INCLUDED
#define FUNCIONES_FERNANDEZ_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "estructuras.h"

#define ARCHIVO_NO_INFORMADO 5
#define EXCEDE_CANT_ARCHIVOS 6
#define COMANDO_INCORRECTO 7

int cambiarTonalidad(FILE* img, char* imgNueva, unsigned char color,  int parametro);
void escribirCabecera(FILE* img, FILE* nueva, t_metadata* cabeceraOriginal);
int procesarArgumentos(int argc, char* argv[]);
bool esNombreDeArchivo(char* parametro);
bool esComando(char* parametro);
bool comandoFueProcesado(char* parametro, char* comandosProcesados[]);
int ejecutarComando(char* comando, char* nombresDeArchivos[]);
char* generarNombreArchivo(char* comando, char* nombresDeArchivos[]);
void** matrizCrear(size_t tamElem, size_t filas, size_t columnas);
void matrizDestruir(void** mat, int filas);
void cargarMatriz(FILE* archivo, t_pixel** mat, size_t filas, size_t columnas);
void escribirArchivo(FILE* archivo, t_pixel** mat, size_t filas, size_t columnas);

#endif // FUNCIONES_FERNANDEZ_H_INCLUDED
