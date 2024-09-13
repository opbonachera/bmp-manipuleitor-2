/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
*/

#include "funciones_grupo.h"

#define ERR_CANT_ARG 1

int solucion(int argc, char* argv[])
{
    if(argc==1)
    {
        printf("\nNo se ingresaron argumentos.");
        return ERR_CANT_ARG;
    }

    if(argc==2)
    {
        printf("\nDebe ingresar nombre de archivo y comando deseado.");
        return ERR_CANT_ARG;
    }

    return procesarArgumentos(argc, argv);;
}
