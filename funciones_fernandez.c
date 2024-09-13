#include "funciones_fernandez.h"
#include "funciones_bonachera.h"

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

    int tamFila = (cabecera.ancho * cabecera.profundidad / 8 + 3) & ~3; //tamaño de fila con padding.

    t_pixel** matImgOriginal = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, tamFila);
    t_pixel** matImgNueva = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, tamFila);

    cargarMatriz(img, matImgOriginal, cabecera.alto, tamFila);

    for(int i=0; i<cabecera.alto; i++)
    {
        for(int j=0; j<tamFila; j++)
        {
            matImgNueva[i][j].pixel[0] = matImgOriginal[i][j].pixel[0];
            matImgNueva[i][j].pixel[1] = matImgOriginal[i][j].pixel[1];
            matImgNueva[i][j].pixel[2] = matImgOriginal[i][j].pixel[2];
            matImgNueva[i][j].pixel[color] = (matImgOriginal[i][j].pixel[color] * multiplicador) > 255 ? 255 : (unsigned char)(matImgOriginal[i][j].pixel[color] * multiplicador);
        }
    }

    escribirArchivo(imgNueva, matImgNueva, cabecera.alto, tamFila);

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

    int tamFila = (cabecera.ancho * cabecera.profundidad / 8 + 3) & ~3; //tamaño de fila con padding.

    t_pixel** matImgOriginal = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, tamFila);
    t_pixel** matImgNueva = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, tamFila);

    cargarMatriz(img, matImgOriginal, cabecera.alto, tamFila);

    unsigned char promedio = 0;

    for(int i=0; i<cabecera.alto; i++)
    {
        for(int j=0; j<tamFila; j++)
        {
            promedio = (matImgOriginal[i][j].pixel[0] + matImgOriginal[i][j].pixel[1] + matImgOriginal[i][j].pixel[2]) / 3;
            matImgNueva[i][j].pixel[0] = matImgNueva[i][j].pixel[1] = matImgNueva[i][j].pixel[2] = promedio;
        }
    }

    escribirArchivo(imgNueva, matImgNueva, cabecera.alto, tamFila);

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
    img1 = fopen(nombresDeArchivos[0],"rb");

    if(!img1)
    {
        printf("\nNo se encontro el archivo 1.");
        return ERROR_APERTURA_ARCHIVO;
    }

    if(nombresDeArchivos[1])
    {
        FILE* img2;
        img2 = fopen(nombresDeArchivos[1],"rb");
        if(!img2)
        {
            printf("\nNo se encontro el archivo 2.");
            return ERROR_APERTURA_ARCHIVO;
        }
    }

    int parametro = obtenerParametro(comando);

    char* nombreNuevoArchivo = generarNombreArchivo(comando, nombresDeArchivos);

    int resultado = OK;

    if(!strcmp(comando, "--tonalidad_roja"))
        resultado = cambiarTonalidad(img1, nombreNuevoArchivo, 2 , parametro);
    if(!strcmp(comando, "--tonalidad_azul"))
        resultado = cambiarTonalidad(img1, nombreNuevoArchivo, 0 , parametro);
    if(!strcmp(comando, "--tonalidad_verde"))
        resultado = cambiarTonalidad(img1, nombreNuevoArchivo, 1 , parametro);
    if(!strcmp(comando, "--rotar-izquierda"))
        resultado = rotarImagenIzquierda(img1, nombreNuevoArchivo);
    if(!strcmp(comando, "--rotar-derecha"))
        resultado = rotarImagenDerecha(img1, nombreNuevoArchivo);
    if(!strcmp(comando, "--espejar-vertical"))
        resultado = espejarImagenVertical(img1, nombreNuevoArchivo);
    if(!strcmp(comando, "--espejar-horizontal"))
        resultado = espejarImagenHorizontal(img1, nombreNuevoArchivo);
    if(!strcmp(comando, "--escala-de-grises"))
        resultado = escalaDeGrises(img1, nombreNuevoArchivo);


    free(nombreNuevoArchivo);
    fclose(img1);

    return resultado;
}

char* generarNombreArchivo(char* comando, char* nombresDeArchivos[])
{
    char* nombreGrupo = "MEMORIA_";
    char* nombreNuevoArchivo = malloc(strlen(nombreGrupo) + strlen(comando+2) + strlen(nombresDeArchivos[0]));

    strcpy(nombreNuevoArchivo, nombreGrupo);
    strcat(nombreNuevoArchivo, comando+2);
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
