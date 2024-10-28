#include "funciones_bonachera.h"
#include "funciones_fernandez.h"

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
            return 0; // Parametro invalido
        }

        return num;
    }else
    {
        return SIN_PARAMETROS;
    }
}



int leerCabecera(FILE* img, t_metadata *cabecera)
{
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

    for (int i = 0; i < cabeceraOriginal.alto; i++)
    {
        for (int j = 0; j < cabeceraOriginal.ancho; j++)
        {
             matNuevaImagen[cabeceraOriginal.ancho - j - 1][i] = matImagen[i][j];
        }
    }

    escribirArchivo(nuevaImagen, matNuevaImagen, cabeceraOriginal.ancho, cabeceraOriginal.alto);
    matrizDestruir((void**)matImagen, cabeceraOriginal.alto);
    matrizDestruir((void**)matNuevaImagen, cabeceraOriginal.alto);

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

    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    for(int i = 0; i < cabeceraOriginal.alto; i++) {
        for(int j = 0; j < cabeceraOriginal.ancho; j++) {
            matNuevaImagen[j][cabeceraOriginal.alto - i - 1] = matImagen[i][j];
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

    for (int i = 0; i < cabeceraOriginal.alto; i++) {
        for (int j = 0; j < cabeceraOriginal.ancho; j++) {
            matNuevaImagen[i][j] = matImagen[i][cabeceraOriginal.ancho - 1 - j];
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

    for (int i = 0; i < cabeceraOriginal.alto; i++) {
        for (int j = 0; j < cabeceraOriginal.ancho; j++) {
            matNuevaImagen[i][j] = matImagen[cabeceraOriginal.alto - 1 - i][j];
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

    if (!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    t_metadata cabeceraOriginal;

    leerCabecera(imagenOriginal, &cabeceraOriginal);
    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);


    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);
    t_pixel** matNuevaImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);


    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    int n = 4;

    for(int i=0; i<cabeceraOriginal.alto; i+=n)
    {
        for(int j=0; j<cabeceraOriginal.ancho; j+=n)
        {
            unsigned int sumaRojo=0, sumaVerde=0, sumaAzul=0;

            for(int k=0; k<n; k++)
            {
                for(int l=0; l<n; l++)
                {
                    sumaRojo  += matImagen[k+i][l+j].pixel[0];
                    sumaVerde += matImagen[k+i][l+j].pixel[1];
                    sumaAzul  += matImagen[k+i][l+j].pixel[2];
                }
            }

            unsigned char prmRojo =  MIN(sumaRojo / (n * n), 255);
            unsigned char prmVerde = MIN(sumaVerde / (n * n), 255);
            unsigned char prmAzul =  MIN(sumaAzul / (n * n), 255);

            for(int k=0; k<n; k++)
            {
                for(int l=0; l<n; l++)
                {
                    matNuevaImagen[k+i][l+j].pixel[0] = prmRojo;
                    matNuevaImagen[k+i][l+j].pixel[1] = prmVerde;
                    matNuevaImagen[k+i][l+j].pixel[2] = prmAzul;
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

int achicarImagen(FILE* imagenOriginal, char* nombreNuevoArchivo, int parametro)
{
    FILE* nuevaImagen;
    t_metadata cabeceraOriginal;

    nuevaImagen = fopen(nombreNuevoArchivo, "wb");
    if (!nuevaImagen)
        return ERROR_CREACION_ARCHIVO;

    leerCabecera(imagenOriginal, &cabeceraOriginal);
    escribirCabecera(imagenOriginal, nuevaImagen, &cabeceraOriginal);


    int nuevoAlto = cabeceraOriginal.alto * parametro / 100;
    int nuevoAncho = cabeceraOriginal.ancho * parametro / 100;

    t_pixel** matImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), cabeceraOriginal.alto, cabeceraOriginal.ancho);
    t_pixel** matNuevaImagen = (t_pixel**)matrizCrear(sizeof(t_pixel), nuevoAlto, nuevoAncho);

    fseek(imagenOriginal, cabeceraOriginal.comienzoImagen, SEEK_SET);
    cargarMatriz(imagenOriginal, matImagen, cabeceraOriginal.alto, cabeceraOriginal.ancho);


    float alRatio = (float)cabeceraOriginal.alto / nuevoAlto;
    float anRatio = (float)cabeceraOriginal.ancho / nuevoAncho;

    for (int i = 0; i < nuevoAlto; i++) {
        for (int j = 0; j < nuevoAncho; j++) {

            int nI = (int)(i * alRatio);
            int nJ = (int)(j * anRatio);

            matNuevaImagen[i][j] = matImagen[nI][nJ];
        }
    }

    escribirArchivo(nuevaImagen, matNuevaImagen, nuevoAlto, nuevoAncho);
    modificarDimensiones(nuevaImagen, nuevoAncho, nuevoAlto);

    matrizDestruir((void**)matImagen, cabeceraOriginal.alto);
    matrizDestruir((void**)matNuevaImagen, nuevoAlto);

    fclose(nuevaImagen);

    return OK;
}
