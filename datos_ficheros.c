#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla.h"
#include "errores.h"
#define EXTENSION 5     //Espacio para guardar la extension ".txt\0"


void guardarCarpetas(info_carpetas *carp, int num_carp)
{/*Funcion que nos permite crear ficheros ".txt" con los nombres de los correos*/
    FILE *archiv;
    char auxiliar[INTRODUCIR + EXTENSION];
    int i = 0, j = 0;

    for(i = 0; i < num_carp; i++)
    {
        for(j = 0; j < carp[i].contenido.longitud; j++)
            auxiliar[j] = carp[i].contenido.nombre[j];
        auxiliar[j] = '\0';
        strcat(auxiliar, ".txt");

        carp[i].archivo.longitud = strlen(auxiliar);
        carp[i].archivo.nombre = (char *) malloc((carp[i].archivo.longitud + 1) * sizeof(char));    //El +1 esta para poder almacenar el '\0', y asi usar el nombre
        for(int j = 0; j <= carp[i].archivo.longitud; j++)    //Permite copiar el contenido del string al puntero
            carp[i].archivo.nombre[j] = auxiliar[j];
        /*Parte que permite poner el nombre de la carpeta al fichero*/

        archiv = fopen(carp[i].archivo.nombre, "r");    //Lee archivos ".txt"
        if(archiv == NULL){                             //En caso de no existir el archivo, lo crea
            archiv = fopen(carp[i].archivo.nombre, "w");

            for(j = 0; j < carp[i].contenido.longitud; j++)
                fprintf(archiv, "%c", carp[i].contenido.nombre[j]);
            fprintf(archiv, "\n%d\n", carp[i].contenido.longitud);
            fprintf(archiv, "%d\n", carp[i].numero_correos);
            fprintf(archiv, "%d\n", carp[i].tipo);

            fclose(archiv);
            archiv = fopen(carp[i].archivo.nombre, "r");
        }
        fclose(archiv);
        /*Parte para verificar que el archivo este creado, de no ser asi, lo crea*/
    }
}


void guardarCorreos(info_carpetas *carp, info_correos *corr, int num_carp, int total_corr)
{
    FILE *archiv;
    char auxiliar[2 * INTRODUCIR + EXTENSION];
    int i = 0, j = 0;

    for(i = 0; i < num_carp; i++)
    {
        archiv = fopen(carp[i].archivo.nombre, "a");
        if(archiv != NULL){ //1.A En caso de que se pueda acceder a la carpeta

            for(j = 0; j < total_corr; j++){

                if(carp[i].tipo == corr[j].tipo_carpeta){
                    for(j = 0; j < corr[i].contenido.longitud; j++)
                        fprintf(archiv, "%c", corr[i].contenido.nombre[j]);
                    fprintf(archiv, "\n%d\n", corr[j].contenido.longitud);
                    fprintf(archiv, "%c\n", corr[j].leido);
                    fprintf(archiv, "%d\n", corr[j].tipo_carpeta);
                }
            }

        } else {            //1.B En caso de que NO se pueda acceder a la carpeta

            errorArchivo(carp[i].archivo.nombre);
        }
        fclose(archiv);
    }
}


info_carpetas * eliminarCarpetas(info_carpetas *carp, int *num_carp)
{/*Borra las carpetas de los correos borrados, y edita la estructura dinamica de las carpetas*/
    info_carpetas *auxiliar;
    int borrar = 0;
    int eliminar = 0;
    int i = 0, j = 0, k = 0;

    do{
        printf("\nIntroduce cuantas carpetas quieres %s: ", cadena);
        scanf("%d", &borrar);
        if(borrar < 0 || borrar >= (*num_carp)) //1.A En caso de que la carpeta seleccionada no exista
            errorTamanyo(&borrar);
        else{


        }
    }while(borrar < 0 || borrar >= (*num_carp));

    auxiliar = (info_carpetas *)malloc(((*num_carp) - borrar) * sizeof(info_carpetas));

    for(i = 0; i < borrar; i++){
        do{
            printf("\nIntroduce el numero de la %d carpeta: ", i);
            scanf("%d", &eliminar);
            if(eliminar < 0 || eliminar >= (*num_carp)) //2.A En caso de que la carpeta seleccionada no exista
                errorAccederCarp(&eliminar);
            else
                carp[eliminar].eliminar = 'S';
        }while(eliminar < 0 || eliminar >= (*num_carp));
    }

    if(borrar > 0)
    {
        for(i = 0; i < (*num_carp); i++){

            if(carp[i].eliminar == 'N')         //3.A Carpetas que hay que conservar
            {
                auxiliar[k].archivo.nombre = carp[i].archivo.nombre;
                auxiliar[k].archivo.longitud = carp[i].archivo.longitud;
                auxiliar[k].contenido.nombre = carp[i].contenido.nombre;
                auxiliar[k].contenido.longitud = carp[i].contenido.longitud;
                auxiliar[k].numero_correos = carp[i].numero_correos;
                auxiliar[k].tipo = k;
                k++;
            }

            else if(carp[i].eliminar == 'S')    //3.B Carpetas que hay que borrar
            {
                remove(carp[i].archivo.nombre);
                for(j = 0; j < carp[eliminar].numero_correos; j++){
                    free(carp[i].contenido.nombre);
                    free(carp[i].archivo.nombre);
                }
            }
        }
        *num_carp -= borrar;
        free(carp);
        return auxiliar;    //Hay 2 return, ya que si no se entra dentro de este bucle for, no habria
    }                       //modificaciones para la estructura dinamica, por lo que habria que devolver
    return carp;            //la estructura dinamica tal y como estaba
}







/*info_correos * eliminarCorreos(info_carpetas *carp, info_correos *corr, int num_carp, int total_corr)
{
    FILE *archiv;
    info_correos *auxiliar;
    int i = 0;

    for(i = 0; i < total_corr; i++)
    {


    }

    return auxiliar;
}*/
