#include "funciones_risso.h"
#include "funciones_bonachera.h"
#include "funciones_fernandez.h"


void asignarAltoyAnchoParaVerOHori(int* alto, int* ancho, t_metadata* cabecera1, t_metadata* cabecera2, char concatenar){
    if(concatenar=='V'){
        *alto = (int)cabecera1->alto + (int)cabecera2->alto;
        *ancho = (int)cabecera1->ancho > (int)cabecera2->ancho ? (int)cabecera1->ancho : (int)cabecera2->ancho;
    }
    else if(concatenar=='H'){
        *alto = (int)cabecera1->alto > (int)cabecera2->alto ? (int)cabecera1->alto : (int)cabecera2->alto;
        *ancho = (int)cabecera1->ancho + (int)cabecera2->ancho;
    }
}

void rellenarMatrizDeRojo(int alto,int ancho, t_pixel** matriz){
    for (int i = 0; i < alto; i++)
    {
        for (int j = 0; j < ancho; j++)
        {
            matriz[i][j].pixel[0] = 0;
            matriz[i][j].pixel[1] = 0;
            matriz[i][j].pixel[2] = 255;
        }
    }
}

void concatenarVeryHor(t_metadata* cabecera1, t_metadata* cabecera2, t_pixel** matNueva, t_pixel** matImagenUno, t_pixel** matImagenDos, char concatenar){

    for (int i = 0; i < ((int)cabecera1->alto); i++)
    {
        for (int j = 0; j < ((int)cabecera1->ancho); j++)
        {
            matNueva[i][j] = matImagenUno[i][j];
        }
    }


    for (int i = 0; i < ((int)cabecera2->alto); i++)
    {
        for (int j = 0; j < ((int)cabecera2->ancho); j++)
        {
            if(concatenar=='V')
            matNueva[i + (int)cabecera1->alto][j]= matImagenDos[i][j];

            else if(concatenar=='H'){
            matNueva[i][j + ((int)cabecera1->ancho)] = matImagenDos[i][j];}
        }
    }
}

int aumentarContraste(FILE* img, char* nombreNuevoArchivo, int parametro){

    FILE* imgNueva = fopen(nombreNuevoArchivo, "wb");

    if(!imgNueva)
    {
        printf("\nError al crear la nueva imagen.");
        return ERROR_CREACION_ARCHIVO;
    }

    t_metadata cabecera;
    leerCabecera(img,&cabecera);

    escribirCabecera(img, imgNueva, &cabecera);

    int tamFila = (cabecera.ancho * cabecera.profundidad / 8 + 3) & ~3;

    t_pixel** matImgOriginal = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, tamFila);
    t_pixel** matImgNueva = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, tamFila);

    cargarMatriz(img, matImgOriginal, cabecera.alto, tamFila);

    float porcentaje= ((float)parametro/100)+1;

    int newRed, newGreen, newBlue;

    for(int i=0; i<cabecera.alto; i++)
    {
            for(int j=0; j<tamFila; j++)
            {
                newRed = 128 + (matImgOriginal[i][j].pixel[2]-128)* porcentaje;
                newGreen = 128 + (matImgOriginal[i][j].pixel[1]-128)* porcentaje;
                newBlue = 128 + (matImgOriginal[i][j].pixel[0]-128)* porcentaje;

                matImgNueva[i][j].pixel[2] = (newRed > 255) ? 255 : (newRed < 0) ? 0 : newRed;
                matImgNueva[i][j].pixel[1] = (newGreen > 255) ? 255 : (newGreen < 0) ? 0 : newGreen;
                matImgNueva[i][j].pixel[0] = (newBlue > 255) ? 255 : (newBlue < 0) ? 0 : newBlue;

            }
    }

    escribirArchivo(imgNueva, matImgNueva, cabecera.alto, tamFila);
    matrizDestruir((void**)matImgOriginal, cabecera.alto);
    matrizDestruir((void**)matImgNueva, cabecera.alto);
    fclose(imgNueva);

    return OK;
}

int reducirContraste(FILE* img, char* nombreNuevoArchivo, int parametro){

    FILE* imgNueva = fopen(nombreNuevoArchivo, "wb");

    if(!imgNueva)
    {
        printf("\nError al crear la nueva imagen.");
        return ERROR_CREACION_ARCHIVO;
    }

    t_metadata cabecera;
    leerCabecera(img,&cabecera);

    escribirCabecera(img, imgNueva, &cabecera);

    int tamFila = (cabecera.ancho * cabecera.profundidad / 8 + 3) & ~3; //tama�o de fila con padding.

    t_pixel** matImgOriginal = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, tamFila);
    t_pixel** matImgNueva = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera.alto, tamFila);

    cargarMatriz(img, matImgOriginal, cabecera.alto, tamFila);

    int promedio;
    float porcentaje= ((float)parametro/100)+1;
    for(int i=0; i<cabecera.alto; i++)
    {
        for(int j=0; j<tamFila; j++)
        {
            promedio=(matImgNueva[i][j].pixel[0]+matImgNueva[i][j].pixel[1]+matImgNueva[i][j].pixel[2])/3;
            if(promedio<=127){
            matImgNueva[i][j].pixel[0] = (matImgOriginal[i][j].pixel[0] * porcentaje) > 255 ? 255 : (unsigned char)(matImgOriginal[i][j].pixel[0] * porcentaje);
            matImgNueva[i][j].pixel[1] = (matImgOriginal[i][j].pixel[1] * porcentaje) > 255 ? 255 : (unsigned char)(matImgOriginal[i][j].pixel[1] * porcentaje);
            matImgNueva[i][j].pixel[2] = (matImgOriginal[i][j].pixel[2] * porcentaje) > 255 ? 255 : (unsigned char)(matImgOriginal[i][j].pixel[2] * porcentaje);
            }
            else{
            matImgNueva[i][j].pixel[0] = matImgOriginal[i][j].pixel[0];
            matImgNueva[i][j].pixel[1] = matImgOriginal[i][j].pixel[1];
            matImgNueva[i][j].pixel[2] = matImgOriginal[i][j].pixel[2];
            }
        }
    }

    escribirArchivo(imgNueva, matImgNueva, cabecera.alto, tamFila);
    matrizDestruir((void**)matImgOriginal, cabecera.alto);
    matrizDestruir((void**)matImgNueva, cabecera.alto);
    fclose(imgNueva);

    return OK;
}

int concatenarImagenesVertioHori(FILE* img1, FILE* img2, char* nombreNuevoArchivo, char concatenar){

    if(!img1 || img2)
    {
        printf("Error: para concatenar imagenes deben enviarse dos archivos .bmp \n");
        return FALTA_IMAGEN;
    }

    FILE* imagenNueva = fopen(nombreNuevoArchivo, "wb");

    if (!imagenNueva)
        return ERROR_CREACION_ARCHIVO;


    t_metadata cabecera1;
    t_metadata cabecera2;

    leerCabecera(img1, &cabecera1);
    leerCabecera(img2, &cabecera2);

    escribirCabecera(img1, imagenNueva, &cabecera1);

    int alto, ancho;

    asignarAltoyAnchoParaVerOHori(&alto, &ancho, &cabecera1, &cabecera2, concatenar);

    t_pixel** matImagenUno = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera1.alto, cabecera1.ancho);
    t_pixel** matImagenDos = (t_pixel**)matrizCrear(sizeof(t_pixel), cabecera2.alto, cabecera2.ancho);
    t_pixel** matimagenNueva = (t_pixel**)matrizCrear(sizeof(t_pixel), alto, ancho);

    rellenarMatrizDeRojo(alto,ancho, matimagenNueva);

    fseek(img2, cabecera2.comienzoImagen,SEEK_SET);
    cargarMatriz(img1, matImagenUno, cabecera1.alto, cabecera1.ancho);
    cargarMatriz(img2, matImagenDos, cabecera2.alto, cabecera2.ancho);

    concatenarVeryHor(&cabecera1,&cabecera2, matimagenNueva, matImagenUno, matImagenDos, concatenar);

    escribirArchivo(imagenNueva, matimagenNueva, alto, ancho);

    modificarDimensiones(imagenNueva, ancho, alto);

    matrizDestruir((void**)matImagenUno, cabecera1.alto);
    matrizDestruir((void**)matImagenDos, cabecera2.alto);
    matrizDestruir((void**)matimagenNueva, alto);

    fclose(imagenNueva);

    return OK;
}
