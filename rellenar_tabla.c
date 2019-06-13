/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla.h"
#include "errores.h"

/*
 *
 */

void pedirNombreUsuario(texto *user)
{/*Funcion que permite introducir un nombre de usuario*/
    char introducir[INTRODUCIR];    //String para almacenar los nombres de las carpetas

    printf("Introduce tu nombre de usuario: ");
    gets(introducir);

    user->longitud = strlen(introducir);
    user->nombre = (char *) malloc(user->longitud * sizeof(char));
    for(int j = 0; j < user->longitud; j++)
        user->nombre[j] = introducir[j];
}


void pedirNumeroCC(int *numero, char *cadena, int minimo)
{/*Funcion para pedir el numero de carpetas*/
    do{
        printf("\nIntroduce el numero que deseas %s: ", cadena);
        fflush(stdin);
        scanf("%d", numero);

        if(*numero < minimo)
            errorPosicionCC(numero);
    }while(*numero < minimo);
}


void pedirNombreCarpetas(info_carpetas *carp, int *long_carp, int num_carp, int inicio)
{/*Funcion para darle nombre a las carpetas*/
    char introducir[INTRODUCIR];    //String para almacenar los nombres de las carpetas
    int i = 0;

    for (i = inicio; i < num_carp; i++){/*Bucle que sigue hasta llegar al numero de carpetas*/
        printf("Introduce el nombre de la carpeta %d: ", i);
        fflush(stdin);
        gets(introducir);

        carp[i].contenido.longitud = strlen(introducir);
        carp[i].contenido.nombre = (char *) malloc(carp[i].contenido.longitud * sizeof(char));
        for(int j = 0; j < carp[i].contenido.longitud; j++)//Permite copiar el contenido del string al puntero
            carp[i].contenido.nombre[j] = introducir[j];

        if(carp[i].contenido.longitud > *long_carp)  //1.A Entra si el nuevo nombre de la carpeta, tiene una longitud mas larga
            *long_carp = carp[i].contenido.longitud;

        carp[i].tipo = i;
        carp[i].numero_correos = 0;
        carp[i].eliminar = 'N';
    }
}


void pedirNombreCorreos(info_correos *corr, int *long_corr, int max_corr, int inicio)
{/*Funcion que nos permite introducir el nombre de los correos*/
    char introducir[2 * INTRODUCIR];    //String para introducir los nombres de los correos
    int  i = 0, j = 0;

    for(i = inicio; i < max_corr; i++){
        do{
            printf("\nIntroduce el nombre del correo %d: ", i);
            fflush(stdin);
            gets(introducir);

            corr[i].contenido.longitud = strlen(introducir);
            corr[i].contenido.nombre = (char *) malloc(corr[i].contenido.longitud * sizeof(char));
            for(j = 0; j < corr[i].contenido.longitud; j++) //Permite copiar el contenido del string al puntero
                corr[i].contenido.nombre[j] = introducir[j];

            printf("Este correo esta leido? [ S / N ]: ");
            fflush(stdin);
            scanf("%c", &(corr[i].leido));

            switch (corr[i].leido)
            {
                case 'S':   //1.A En caso de que el correo se marque leido
                case 's':
                    corr[i].leido = ' ';
                    break;

                case 'N':   //1.B En caso de que el correo NO se marque leido
                case 'n':
                    corr[i].leido = 'X';
                    break;

                default:
                    errorComandoDesconocido();
            }
        }while(corr[i].leido != ' ' && corr[i].leido != 'X');
        if(corr[i].contenido.longitud > *long_corr)//2. En caso de que la longitud del nuevo nombre es mayor que el mayor actual
            *long_corr = corr[i].contenido.longitud;
        corr[i].eliminar = 'N';
    }
}


void meterCorreosACarpetas(info_carpetas *carp, info_correos *corr, int max_carp, int max_corr, int inicio_corr)
{/*Funcion que nos permite codificar los correos, para que pertenezcan a la carpeta con el mismo numero*/
    int i = 0, j = 0;

    printf("Introduce los correos en carpetas:\n");
    carpetasExistentes(carp, &max_carp);

    for(i = inicio_corr; i < max_corr; i++){
        do{
            printf("\nCorreo [ ");
            for(j = 0; j < corr[i].contenido.longitud; j++)
                printf("%c", corr[i].contenido.nombre[j]);
            printf(" ]: ");
            fflush(stdin);
            scanf("%d", &(corr[i].tipo_carpeta));

            if(corr[i].tipo_carpeta < 0 || corr[i].tipo_carpeta >= max_carp)
                errorPosicionCC(&(corr[i].tipo_carpeta));
        }while(corr[i].tipo_carpeta < 0 || corr[i].tipo_carpeta >= max_carp);

        carp[corr[i].tipo_carpeta].numero_correos++;
    }
}


void liberar(info_carpetas *carp, int num_carp, info_correos *corr, int total_corr)
{/*Funcion que permite liberar, al final del programa, el espacio reservado por malloc, y evita fallos �\_(:D)_/�*/
    int i = 0;

    for(i = 0; i < total_corr; i++)
        free(corr[i].contenido.nombre);
    for (i = 0; i < num_carp; i++)
        free(carp[i].contenido.nombre);
    //free(user);
    free(corr);
    free(carp);
}