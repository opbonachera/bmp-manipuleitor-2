#include "funciones_fernandez.h"
#include "funciones_bonachera.h"
#include "funciones_risso.h"

int cambiarTonalidad(FILE* img, char* nombreNuevoArchivo, unsigned char color, int parametro)
{
    float multiplicador = 1 + ((float)parametro / 100);

    FILE* imgNueva = fopen(nombreNuevoArchivo, "wb");

    if(!imgNueva)
    {
        printf("\nError al crear la nueva imagen.");
        return ERROR_CREACION_ARCHIVO;
    }

    t_metadata cabecera;
    leerCabecera(img,&cabecera);

    escribirCabecera(img, imgNueva, &cabecera);


    t_pixel** matImgOriginal = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, cabecera.ancho);
    t_pixel** matImgNueva = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, cabecera.ancho);

    cargarMatriz(img, matImgOriginal, cabecera.alto, cabecera.ancho);

    for(int i=0; i<cabecera.alto; i++)
    {
        for(int j=0; j<cabecera.ancho; j++)
        {
            matImgNueva[i][j].pixel[0] = matImgOriginal[i][j].pixel[0];
            matImgNueva[i][j].pixel[1] = matImgOriginal[i][j].pixel[1];
            matImgNueva[i][j].pixel[2] = matImgOriginal[i][j].pixel[2];
            matImgNueva[i][j].pixel[color] = (matImgOriginal[i][j].pixel[color] * multiplicador) > 255 ? 255 : (unsigned char)(matImgOriginal[i][j].pixel[color] * multiplicador);
        }
    }

    escribirArchivo(imgNueva, matImgNueva, cabecera.alto, cabecera.ancho);

    matrizDestruir((void**)matImgOriginal, cabecera.alto);
    matrizDestruir((void**)matImgNueva, cabecera.alto);
    fclose(imgNueva);

    return OK;
}


int escalaDeGrises(FILE* img, char* nombreNuevoArchivo)
{
    FILE* imgNueva = fopen(nombreNuevoArchivo, "wb");

    if(!imgNueva)
    {
        printf("\nError al crear la nueva imagen.");
        return ERROR_CREACION_ARCHIVO;
    }

    t_metadata cabecera;
    leerCabecera(img,&cabecera);

    escribirCabecera(img, imgNueva, &cabecera);

    t_pixel** matImgOriginal = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, cabecera.ancho);
    t_pixel** matImgNueva = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, cabecera.ancho);

    cargarMatriz(img, matImgOriginal, cabecera.alto, cabecera.ancho);

    unsigned char promedio = 0;

    for(int i=0; i<cabecera.alto; i++)
    {
        for(int j=0; j<cabecera.ancho; j++)
        {
            promedio = (matImgOriginal[i][j].pixel[0] + matImgOriginal[i][j].pixel[1] + matImgOriginal[i][j].pixel[2]) / 3;
            matImgNueva[i][j].pixel[0] = matImgNueva[i][j].pixel[1] = matImgNueva[i][j].pixel[2] = promedio;
        }
    }

    escribirArchivo(imgNueva, matImgNueva, cabecera.alto, cabecera.ancho);

    matrizDestruir((void**)matImgOriginal, cabecera.alto);
    matrizDestruir((void**)matImgNueva, cabecera.alto);
    fclose(imgNueva);

    return OK;
}


void escribirCabecera(FILE* img, FILE* nueva, t_metadata* cabeceraOriginal)
{
    char bytes[cabeceraOriginal->comienzoImagen];

    fread(&bytes, sizeof(bytes), 1, img);
    fwrite(&bytes, sizeof(bytes), 1, nueva);
}


int procesarArgumentos(int argc, char* argv[])
{
    int resultado = OK;

    char* nombresDeArchivos[2] = {NULL, NULL};
    char* comandosProcesados[argc];

    for(int i = 0; i < argc; i++)
    {
        comandosProcesados[i] = NULL;
    }

    int cantidadArchivos = 0;
    int cantidadComandos = 0;

    for(int i = 1; i < argc; i++)
    {
        if(esComando(argv[i]) && !comandoFueProcesado(argv[i], comandosProcesados))
        {
            comandosProcesados[cantidadComandos] = argv[i];
            cantidadComandos++;
        }
        else if(esNombreDeArchivo(argv[i]) && cantidadArchivos <= 2)
        {
            nombresDeArchivos[cantidadArchivos] = argv[i];
            cantidadArchivos++;
        }
    }

    if(cantidadArchivos == 0)
    {
        printf("\nDebe informar el/los archivo/s a procesar.");
        return ARCHIVO_NO_INFORMADO;
    }

    if(cantidadArchivos > 2)
    {
        printf("\nSolo puede ingresar hasta 2 archivos a procesar.");
        return EXCEDE_CANT_ARCHIVOS;
    }

    for(int i = 0; i < cantidadComandos && !resultado; i++)
    {
        resultado = ejecutarComando(comandosProcesados[i], nombresDeArchivos);
    }

    return resultado;
}

bool esNombreDeArchivo(char* parametro)
{
    char* pos;
    pos = strrchr(parametro, '.');

    if(!pos)
        return false;

    return true;
}

bool esComando(char* parametro)
{
    char* cad = malloc(strlen(parametro) + 1);

    strcpy(cad, parametro);

    cad += 2;
    *cad = '\0';

    cad -= 2;

    int resultado = strcmp(cad, "--");

    if(!resultado)
    {
        free(cad);
        return true;
    }

    free(cad);
    return false;
}

bool comandoFueProcesado(char* parametro, char* comandosProcesados[])
{
    int i = 0;
    int resultado = 1;

    while(comandosProcesados[i] && strlen(comandosProcesados[i]) > 0)
    {
        resultado = strcmp(parametro, comandosProcesados[i]);

        if(!resultado)
        {
            printf("\nEl comando \"%s\" esta duplicado.", comandosProcesados[i]);
            return true;
        }

        i++;
    }

    return false;
}

int ejecutarComando(char* comando, char* nombresDeArchivos[])
{
    FILE* img1;
    FILE* img2;
    img1 = fopen(nombresDeArchivos[0],"rb");

    if(!img1)
    {
        printf("\nNo se encontro el archivo 1.");
        return ERROR_APERTURA_ARCHIVO;
    }

    if(nombresDeArchivos[1])
    {
        img2 = fopen(nombresDeArchivos[1],"rb");
        if(!img2)
        {
            printf("\nNo se encontro el archivo 2.");
            return ERROR_APERTURA_ARCHIVO;
        }
    }

    int parametro = obtenerParametro(comando);
    char* nombreNuevoArchivo = generarNombreArchivo(comando, nombresDeArchivos);

    if(!strcmp(comando, "--tonalidad-roja") && parametro)
        cambiarTonalidad(img1, nombreNuevoArchivo, 2 , parametro);
    else if(!strcmp(comando, "--tonalidad-azul") && parametro)
        cambiarTonalidad(img1, nombreNuevoArchivo, 0 , parametro);
    else if(!strcmp(comando, "--tonalidad-verde") && parametro)
        cambiarTonalidad(img1, nombreNuevoArchivo, 1 , parametro);
    else if(!strcmp(comando, "--reducir-contraste") && parametro)
        reducirContraste(img1, nombreNuevoArchivo,parametro);
    else if(!strcmp(comando, "--aumentar-contraste") && parametro)
        aumentarContraste(img1, nombreNuevoArchivo,parametro);
    else if(!strcmp(comando, "--rotar-izquierda"))
        rotarImagenIzquierda(img1, nombreNuevoArchivo);
    else if(!strcmp(comando, "--rotar-derecha"))
        rotarImagenDerecha(img1, nombreNuevoArchivo);
    else if(!strcmp(comando, "--espejar-vertical"))
        espejarImagenVertical(img1, nombreNuevoArchivo);
    else if(!strcmp(comando, "--espejar-horizontal"))
        espejarImagenHorizontal(img1, nombreNuevoArchivo);
    else if(!strcmp(comando, "--escala-de-grises"))
        escalaDeGrises(img1, nombreNuevoArchivo);
    else if(!strcmp(comando, "--concatenar-horizontal"))
        concatenarImagenesVertioHori(img1, img2, nombreNuevoArchivo,HORIZONTAL);
    else if(!strcmp(comando, "--concatenar-vertical"))
        concatenarImagenesVertioHori(img1, img2, nombreNuevoArchivo,VERTICAL);
    else if(!strcmp(comando, "--recortar") && parametro)
        recortarImagen(img1,nombreNuevoArchivo,parametro);
    else if(!strcmp(comando, "--achicar") && parametro)
        achicarImagen(img1,nombreNuevoArchivo, parametro);
    else if(!strcmp(comando, "--negativo"))
        invertirImagen(img1,nombreNuevoArchivo);
    else if(!strcmp(comando, "--comodin"))
        pixelearImagen(img1,nombreNuevoArchivo);
    else
    {
        printf("\nError: El comando \"%s\" no existe o se informo un parametro fuera de rango.\n", comando);
    }

    free(nombreNuevoArchivo);
    fclose(img1);

    return OK;
}

char* generarNombreArchivo(const char* comando, char* nombresDeArchivos[])
{
    char* nombreGrupo = "MEMORIA_";
    char* nombreNuevoArchivo = malloc(strlen(nombreGrupo) + strlen(comando + 2) + strlen(nombresDeArchivos[0]) + 2);

    if (!nombreNuevoArchivo) {
        return NULL;
    }

    strcpy(nombreNuevoArchivo, nombreGrupo);
    strcat(nombreNuevoArchivo, comando + 2);
    strcat(nombreNuevoArchivo, "_");
    strcat(nombreNuevoArchivo, nombresDeArchivos[0]);

    return nombreNuevoArchivo;
}

void** matrizCrear(size_t tamElem, size_t filas, size_t columnas)
{
    void** mat = malloc(sizeof(void*) * filas);

    if(!mat)
    {
        printf("\nError al crear la matriz.");
        return NULL;
    }

    void** ult = mat + filas - 1;

    for(void** i = mat; i <= ult; i++)
    {
        *i = malloc(tamElem * columnas);

        if(!*i)
        {
            matrizDestruir(mat, i - mat);
            printf("\nError al crear la matriz.");
            return NULL;
        }
    }

    return mat;
}

void matrizDestruir(void** mat, int filas)
{
    void** ult = mat + filas - 1;

    for(void** i = mat; i <= ult; i++)
    {
        free(*i);
    }

    free(mat);
}


void cargarMatriz(FILE* archivo, t_pixel** mat, size_t filas, size_t columnas)
{
    for(int i=0; i<filas; i++)
    {
        for(int j=0; j<columnas; j++)
        {
            fread(mat[i][j].pixel, sizeof(t_pixel), 1, archivo);
        }
    }
}

void escribirArchivo(FILE* archivo, t_pixel** mat, size_t filas, size_t columnas)
{
    for(int i=0; i<filas; i++)
    {
        for(int j=0; j<columnas; j++)
        {
            fwrite(mat[i][j].pixel, sizeof(t_pixel), 1, archivo);
        }
    }
}
