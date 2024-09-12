#include "funciones_bonachera.h"
#include "funciones_fernandez.h"

#define OK 0
#define ERROR_APERTURA_ARCHIVO 1
#define ERROR_CREACION_ARCHIVO 2
#define FUERA_DE_RANGO 3
#define SIN_PARAMETROS 4

// Helpers
bool validarRango(int limiteInferior, int limiteSuperior, int valor)
{
    return (valor>limiteInferior && valor<limiteSuperior);
}

void modificarDimensiones(FILE* img, int nuevoX, int nuevoY)
{
    fseek(img, 18, SEEK_SET);
    fwrite(&nuevoX, sizeof(unsigned int), 1, img);
    fwrite(&nuevoY, sizeof(unsigned int), 1, img);
}

int obtenerParametro(char* argumento)
{
    char* pos = strrchr(argumento, '=') + 1;
    int num;

    if(pos)
    {
        sscanf(pos, "%d", &num);

        if(!validarRango(0,101,num))
        {
            printf("El parametro no esta dentro del rango aceptado.\n");
            return FUERA_DE_RANGO;
        }

        return num;
    }else
    {
        printf("No se encontro ningun parametro.\n");
        return SIN_PARAMETROS;
    }
}


int leerCabecera(FILE* img, t_metadata *cabecera)
{
    printf("\n\n--------");

    fseek(img, 2, SEEK_SET);
    fread(&cabecera->tamArchivo, sizeof(unsigned int), 1, img);

    fseek(img, 14, SEEK_SET);
    fread(&cabecera->tamEncabezado, sizeof(unsigned int), 1, img);

    fseek(img, 10, SEEK_SET);
    fread(&cabecera->comienzoImagen, sizeof(unsigned int), 1, img);

    fseek(img, 18, SEEK_SET);
    fread(&cabecera->ancho, sizeof(unsigned int), 1, img);

    fseek(img, 22, SEEK_SET);
    fread(&cabecera->alto, sizeof(unsigned int), 1, img);

    fseek(img, 28, SEEK_SET);
    fread(&cabecera->profundidad, sizeof(unsigned short), 1, img);

    printf("Tamaño de archivo: %u bytes\n",  cabecera->tamArchivo);
    printf("Tamaño de cabecera: %u bytes\n", cabecera->tamEncabezado);
    printf("Alto: %u bytes\n",               cabecera->alto);
    printf("Ancho: %u bytes\n",              cabecera->ancho);
    printf("Comienzo de imagen: byte %u\n",  cabecera->comienzoImagen);
    printf("Profundidad: %u bits\n",         cabecera->profundidad);

    fseek(img, 0, SEEK_SET);
    return 0;
}

// Creacion de imagenes
int rotarImagenIzquierda(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");

    if(!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    t_metadata cabeceraOriginal, cabeceraNueva;

    leerCabecera(imagenOriginal, &cabeceraOriginal);

    escribirCabecera(imagenOriginal, nuevaImagen, cabeceraOriginal);

    int anchoOriginal = cabeceraOriginal.ancho;
    int altoOriginal = cabeceraOriginal.alto;

    t_pixel imagen[240][360];
    t_pixel imagenNueva[360][240];

    for(int y = 0; y < altoOriginal; y++) {
        for(int x = 0; x < anchoOriginal; x++) {
            fread(&imagen[y][x], sizeof(unsigned char), 3, imagenOriginal);
            imagenNueva[x][altoOriginal - 1 - y] = imagen[y][x];
        }
    }

    fwrite(&imagenNueva, sizeof(imagenNueva), 1, nuevaImagen);

    modificarDimensiones(nuevaImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    fclose(nuevaImagen);
    leerCabecera(nuevaImagen, &cabeceraNueva);

    return OK;
}

int rotarImagenDerecha(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");

    if(!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    t_metadata cabeceraOriginal, cabeceraNueva;

    leerCabecera(imagenOriginal, &cabeceraOriginal);

    escribirCabecera(imagenOriginal, nuevaImagen, cabeceraOriginal);

    int anchoOriginal = cabeceraOriginal.ancho;
    int altoOriginal = cabeceraOriginal.alto;

    t_pixel imagen[240][360];

    fread(imagen, sizeof(imagen), 1, imagenOriginal);

    for(int y = 0; y < anchoOriginal; y++) {
        for(int x = 0; x < altoOriginal; x++) {
            fwrite(&imagen[x][y], sizeof(unsigned char), 3, nuevaImagen);
        }
    }

    modificarDimensiones(nuevaImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    fclose(nuevaImagen);
    leerCabecera(nuevaImagen, &cabeceraNueva);

    return OK;
}


int espejarImagenHorizontal(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");

    if(!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    t_metadata cabeceraOriginal, cabeceraNueva;

    leerCabecera(imagenOriginal, &cabeceraOriginal);

    escribirCabecera(imagenOriginal, nuevaImagen, cabeceraOriginal);

    int anchoOriginal = cabeceraOriginal.ancho;
    int altoOriginal = cabeceraOriginal.alto;

    t_pixel matrizOriginal[anchoOriginal][altoOriginal];
    t_pixel matrizEspejada[anchoOriginal][altoOriginal];

   for (int y = 0; y < cabeceraOriginal.alto; y++) {
        for (int x = 0; x < cabeceraOriginal.ancho; x++) {
            fread(matrizOriginal[x][y].pixel, sizeof(unsigned char), 3, imagenOriginal);
        }
    }

    for (int y = 0; y < cabeceraOriginal.alto; y++) {
        for (int x = 0; x < cabeceraOriginal.ancho; x++) {
            matrizEspejada[x][y] = matrizOriginal[cabeceraOriginal.ancho - 1 - x][y];
        }
    }

    for (int y = 0; y < cabeceraOriginal.alto; y++) {
        for (int x = 0; x < cabeceraOriginal.ancho; x++) {
            fwrite(matrizEspejada[x][y].pixel, sizeof(unsigned char), 3, nuevaImagen);
        }
    }
    fclose(nuevaImagen);
    leerCabecera(nuevaImagen, &cabeceraNueva);

    return OK;
}

int espejarImagenVertical(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");

    if(!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    t_metadata cabeceraOriginal, cabeceraNueva;

    leerCabecera(imagenOriginal, &cabeceraOriginal);

    escribirCabecera(imagenOriginal, nuevaImagen, cabeceraOriginal);

    int anchoOriginal = cabeceraOriginal.ancho;
    int altoOriginal = cabeceraOriginal.alto;

    t_pixel matrizOriginal[anchoOriginal][altoOriginal];
    t_pixel matrizEspejada[anchoOriginal][altoOriginal];


    for (int y = 0; y < cabeceraOriginal.alto; y++) {
        for (int x = 0; x < cabeceraOriginal.ancho; x++) {
            fread(matrizOriginal[x][y].pixel, sizeof(unsigned char), 3, imagenOriginal);
        }
    }

    for (int y = 0; y < cabeceraOriginal.alto; y++) {
        for (int x = 0; x < cabeceraOriginal.ancho; x++) {
            matrizEspejada[x][y] = matrizOriginal[x][cabeceraOriginal.alto - y -1];
        }
    }

    for (int y = 0; y < cabeceraOriginal.alto; y++) {
        for (int x = 0; x < cabeceraOriginal.ancho; x++) {
            fwrite(matrizEspejada[x][y].pixel, sizeof(unsigned char), 3, nuevaImagen);
        }
    }

    fclose(nuevaImagen);
    leerCabecera(nuevaImagen, &cabeceraNueva);

    return OK;
}

