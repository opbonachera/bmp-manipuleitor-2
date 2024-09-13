#include "funciones_bonachera.h"
#include "funciones_fernandez.h"

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
    char* pos = strrchr(argumento, '=');

    int num;

    if(pos)
    {
        *pos = '\0';
        pos++;


        sscanf(pos, "%d", &num);

        if(!validarRango(0,101,num))
        {
            printf("El parametro no esta dentro del rango aceptado.\n");
            return 0;
        }

        return num;
    }else
    {
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

int rotarImagenDerecha(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");


    if(!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    if(!imagenOriginal)
        return ERROR_APERTURA_ARCHIVO;

    t_metadata cabeceraOriginal;

    leerCabecera(imagenOriginal, &cabeceraOriginal);
    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);

    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);
    t_pixel** matNuevaImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.ancho, cabeceraOriginal.alto);

    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    for(int y = 0; y < cabeceraOriginal.ancho; y++) {
        for(int x = 0; x < cabeceraOriginal.alto; x++) {
            fwrite(&matImagen[x][y], sizeof(unsigned char), 3, nuevaImagen);
        }
    }

    matrizDestruir((void**)matImagen, cabeceraOriginal.alto);

    modificarDimensiones(nuevaImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    fclose(nuevaImagen);

    return OK;

}

int rotarImagenIzquierda(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");

    if (!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    if (!imagenOriginal)
        return ERROR_APERTURA_ARCHIVO;

    t_metadata cabeceraOriginal;

    leerCabecera(imagenOriginal, &cabeceraOriginal);
    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);


    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);
    t_pixel** matNuevaImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.ancho, cabeceraOriginal.alto);

    for(int y = 0; y < cabeceraOriginal.alto; y++) {
        for(int x = 0; x < cabeceraOriginal.ancho; x++) {
            fread(&matImagen[y][x], sizeof(unsigned char), 3, imagenOriginal);
            matNuevaImagen[x][cabeceraOriginal.alto - 1 - y] = matImagen[y][x];
        }
    }

    escribirArchivo(nuevaImagen, matNuevaImagen, cabeceraOriginal.ancho, cabeceraOriginal.alto);

    matrizDestruir((void**)matImagen, cabeceraOriginal.alto);
    matrizDestruir((void**)matNuevaImagen, cabeceraOriginal.ancho);

    modificarDimensiones(nuevaImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    fclose(nuevaImagen);


    return OK;
}


int espejarImagenHorizontal(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");

    if (!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    if (!imagenOriginal)
        return ERROR_APERTURA_ARCHIVO;

    t_metadata cabeceraOriginal;

    leerCabecera(imagenOriginal, &cabeceraOriginal);
    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);


    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);
    t_pixel** matNuevaImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);


    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    for (int y = 0; y < cabeceraOriginal.alto; y++) {
        for (int x = 0; x < cabeceraOriginal.ancho; x++) {
            matNuevaImagen[y][x] = matImagen[y][cabeceraOriginal.ancho - 1 - x];
        }
    }


    escribirArchivo(nuevaImagen, matNuevaImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);


    matrizDestruir((void**)matImagen, cabeceraOriginal.alto);
    matrizDestruir((void**)matNuevaImagen, cabeceraOriginal.alto);


    fclose(nuevaImagen);

    return OK;
}

int espejarImagenVertical(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");

    if (!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    if (!imagenOriginal)
        return ERROR_APERTURA_ARCHIVO;

    t_metadata cabeceraOriginal;

    leerCabecera(imagenOriginal, &cabeceraOriginal);
    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);


    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);
    t_pixel** matNuevaImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);


    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    for (int y = 0; y < cabeceraOriginal.alto; y++) {
        for (int x = 0; x < cabeceraOriginal.ancho; x++) {
            matNuevaImagen[y][x] = matImagen[cabeceraOriginal.alto - 1 - y][x];
        }
    }


    escribirArchivo(nuevaImagen, matNuevaImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);


    matrizDestruir((void**)matImagen, cabeceraOriginal.alto);
    matrizDestruir((void**)matNuevaImagen, cabeceraOriginal.alto);

    fclose(nuevaImagen);

    return OK;
}
