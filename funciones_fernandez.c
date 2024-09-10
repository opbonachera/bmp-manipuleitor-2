#include "funciones_fernandez.h"
#include "funciones_bonachera.h"

void cambiarTonalidad(FILE* img, char* nombreNuevoArchivo, unsigned char color, char* modificador)
{
    float numModificador = (float)atoi(modificador);
    float multiplicador = 1 + (numModificador / 100);

    FILE* imgNueva = fopen(nombreNuevoArchivo, "wb");

    t_metadata cabecera;
    leerCabecera(img,&cabecera);

    escribirCabecera(img, imgNueva, cabecera);

    int tamFila = (cabecera.ancho * cabecera.profundidad / 8 + 3) & ~3; //tamaño de fila con padding.

    t_pixel imgOriginal[tamFila][cabecera.alto];
    t_pixel nueva[tamFila][cabecera.alto];

    fread(imgOriginal, sizeof(imgOriginal), 1, img);

    for(int i=0; i<tamFila; i++)
    {
        for(int j=0; j<cabecera.alto; j++)
        {
            nueva[i][j].pixel[0] = imgOriginal[i][j].pixel[0];
            nueva[i][j].pixel[1] = imgOriginal[i][j].pixel[1];
            nueva[i][j].pixel[2] = imgOriginal[i][j].pixel[2];
            nueva[i][j].pixel[color] = (imgOriginal[i][j].pixel[color] * multiplicador) > 255 ? 255 : (unsigned char)(imgOriginal[i][j].pixel[color] * multiplicador);
        }
    }

    fwrite(nueva, sizeof(nueva), 1, imgNueva);
    fclose(imgNueva);
}


void escribirCabecera(FILE* img, FILE* nueva, t_metadata cabeceraOriginal)
{
    unsigned char byte[cabeceraOriginal.comienzoImagen];

    fread(&byte, sizeof(byte), 1, img);
    fwrite(&byte, sizeof(byte), 1, nueva);
}


void procesarArgumentos(int argc, char* argv[])
{
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
            printf("\nComando procesado: %s", argv[i]);
        }
        else if(esNombreDeArchivo(argv[i]))
        {
            nombresDeArchivos[cantidadArchivos] = argv[i];
            cantidadArchivos++;
            printf("\nNombre de archivo encontrado: %s", argv[i]);
        }
    }

    for(int i = 0; i < cantidadComandos; i++)
    {
        ejecutarComando(comandosProcesados[i], nombresDeArchivos);
    }
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
    char* cad = (char *)malloc(strlen(parametro) + 1);

    strcpy(cad, parametro);

    cad += 2;
    *cad = '\0';

    cad -= 2;

    int resultado = strcmp(cad, "--");

    if(!resultado)
    {
        free(cad);
        printf("\nComando encontrado: %s", parametro);
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
        i++;
    }

    if(!resultado)
    {
        printf("\nComando duplicado: %s", comandosProcesados[i]);
        return true;
    }

    return false;
}

void ejecutarComando(char* comando, char* nombresDeArchivos[])
{
    FILE* img;
    img = fopen(nombresDeArchivos[0],"rb");

    char* modificador = strrchr(comando, '=');
    if(modificador)
    {
        *modificador = '\0';
        modificador++;
    }

    char* nombreNuevoArchivo = generarNombreArchivo(comando, nombresDeArchivos);
    printf("\nNombreNuevoArchivo: %s", nombreNuevoArchivo);

    if(!strcmp(comando, "--tonalidad_roja"))
        cambiarTonalidad(img, nombreNuevoArchivo, 2 , modificador);
    if(!strcmp(comando, "--tonalidad_azul"))
        cambiarTonalidad(img, nombreNuevoArchivo, 0 , modificador);
    if(!strcmp(comando, "--tonalidad_verde"))
        cambiarTonalidad(img, nombreNuevoArchivo, 1 , modificador);
    if(!strcmp(comando, "--rotar-izquierda"))
        rotarImagenIzquierda(img, nombreNuevoArchivo);
    if(!strcmp(comando, "--rotar-derecha"))
        rotarImagenDerecha(img, nombreNuevoArchivo);
    if(!strcmp(comando, "--espejar-vertical"))
        espejarImagenVertical(img, nombreNuevoArchivo);
    if(!strcmp(comando, "--espejar-horizontal"))
        espejarImagenHorizontal(img, nombreNuevoArchivo);

    free(nombreNuevoArchivo);
    fclose(img);
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
