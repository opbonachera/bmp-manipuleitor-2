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


int pixelearImagen(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");
    printf("el nombre es (%s)", nombreNuevoArchivo);

    if (!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    t_metadata cabeceraOriginal;

    leerCabecera(imagenOriginal, &cabeceraOriginal);
    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);


    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);
    t_pixel** matNuevaImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);


    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

     int n = 8;
        for (int i = 0; i < cabeceraOriginal.alto; i += n) {
            for (int j = 0; j < cabeceraOriginal.ancho; j += n) {
                unsigned long sumaR = 0, sumaG = 0, sumaB = 0;
                int acumuladorPixeles = 0;

                for (int k = 0; k < n && (i + k) < cabeceraOriginal.alto; k++) {
                    for (int l = 0; l < n && (j + l) < cabeceraOriginal.ancho; l++) {
                        sumaR += matImagen[i + k][j + l].pixel[0];
                        sumaG += matImagen[i + k][j + l].pixel[1];
                        sumaB += matImagen[i + k][j + l].pixel[2];
                        acumuladorPixeles++;
                    }
                }

                unsigned char avgR = MIN(sumaR / acumuladorPixeles, 255);
                unsigned char avgG = MIN(sumaG / acumuladorPixeles, 255);
                unsigned char avgB = MIN(sumaB / acumuladorPixeles, 255);

                for (int k = 0; k < n && (i + k) < cabeceraOriginal.alto; k++) {
                    for (int l = 0; l < n && (j + l) < cabeceraOriginal.ancho; l++) {
                        matNuevaImagen[i + k][j + l].pixel[0] = avgR;
                        matNuevaImagen[i + k][j + l].pixel[1] = avgG;
                        matNuevaImagen[i + k][j + l].pixel[2] = avgB;
                    }
                }
            }
        }

    escribirArchivo(nuevaImagen, matNuevaImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    matrizDestruir((void**)matImagen, cabeceraOriginal.alto);
    matrizDestruir((void**)matNuevaImagen, cabeceraOriginal.alto);

    fclose(nuevaImagen);

    return OK;
}

int invertirImagen(FILE* imagenOriginal, char* nombreNuevoArchivo)
{
    FILE* nuevaImagen = fopen(nombreNuevoArchivo, "wb");

    if(!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    t_metadata cabeceraOriginal;

    leerCabecera(imagenOriginal, &cabeceraOriginal);
    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);


    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);

    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    for(int i=0; i<cabeceraOriginal.alto; i++)
    {
        for(int j=0; j<cabeceraOriginal.ancho; j++)
        {
            matImagen[i][j].pixel[0] = ~matImagen[i][j].pixel[0];
            matImagen[i][j].pixel[1] = ~matImagen[i][j].pixel[1];
            matImagen[i][j].pixel[2] = ~matImagen[i][j].pixel[2];
        }
    }

    escribirArchivo(nuevaImagen, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    matrizDestruir((void**)matImagen, cabeceraOriginal.alto);
    return OK;
}

int recortarImagen(FILE* imagenOriginal, char* nombreNuevoArchivo, int parametro)
{
    FILE* nuevaImagen;
    int nuevoX, nuevoY;
    t_metadata cabeceraOriginal;

    nuevaImagen = fopen(nombreNuevoArchivo, "wb");
    if (!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    leerCabecera(imagenOriginal, &cabeceraOriginal);

    nuevoX = cabeceraOriginal.ancho * parametro / 100;
    nuevoY = cabeceraOriginal.alto * parametro / 100;

    cabeceraOriginal.ancho = nuevoX;
    cabeceraOriginal.alto = nuevoY;

    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);


    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto * 100 / parametro, cabeceraOriginal.ancho * 100 / parametro);
    t_pixel** matNuevaImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), nuevoY, nuevoX);


    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto * 100 / parametro, cabeceraOriginal.ancho * 100 / parametro);


    for (int y = 0; y < nuevoY; y++)
    {
        for (int x = 0; x < nuevoX; x++)
        {
            matNuevaImagen[y][x] = matImagen[y][x];
        }
    }

    escribirArchivo(nuevaImagen, matNuevaImagen, nuevoY, nuevoX);

    matrizDestruir((void**)matImagen, cabeceraOriginal.alto * 100 / parametro);
    matrizDestruir((void**)matNuevaImagen, nuevoY);

    modificarDimensiones(nuevaImagen, nuevoX, nuevoY);

    fclose(nuevaImagen);

    return OK;
}
