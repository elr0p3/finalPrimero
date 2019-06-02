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
#define EXTENSION 3     //Espacio para guardar leer mas datos de los ficheros


int obtenerCarpExistentes(int exist_carp)
{
    FILE *carpet = fopen("carpetas.txt", "r");
    char datos[INTRODUCIR];
    exist_carp = -1;     //Ya que en el bucle que determina este valor, simpre nos hara un loop extra
    
    if (carpet == NULL){    //1.A En caso de que no existan datos de carpetas
        carpet = fopen("carpetas.txt", "w");
        exist_carp = 0;
    } else {                //1.B En caso de que haya datos de carpetas
        while(!feof(carpet)){
            fgets(datos, INTRODUCIR, carpet);
            exist_carp++;  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }   //Bucle para detectar cuantas carpetas hay almacenados
    }
                
    fclose(carpet);
    return exist_carp;
}


int obtenerCorrExistentes(int exist_corr)
{
    FILE *carpet = fopen("correos.txt", "r");
    char datos[INTRODUCIR];
    exist_corr = -1;     //Ya que en el bucle que determina este valor, simpre nos hara un loop extra
    
    if (carpet == NULL){    //1.A En caso de que no existan datos de carpetas
        carpet = fopen("correos.txt", "w");
        exist_corr = 0;
    } else {                //1.B En caso de que haya datos de carpetas
        while(!feof(carpet)){
            fgets(datos, INTRODUCIR, carpet);
            exist_corr++;  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }   //Bucle para detectar cuantas carpetas hay almacenados
    }
    
    return exist_corr;
}


info_carpetas * cargarDatosCarpetas(info_carpetas *carp, int num_carp, int *long_carp)
{
    FILE *carpet = fopen("carpetas.txt", "r");
    char datos[INTRODUCIR];
    int i = 0, j = 0;
    
    if (carpet == NULL){    //1.A En caso de que no existan datos de carpetas
        carpet = fopen("carpetas.txt", "w");
    } else {                //1.B En caso de que haya datos de carpetas
        
        carp = (info_carpetas *)malloc(num_carp * sizeof(info_carpetas));
        
        for(i = 0; i < num_carp; i++){
            
            fgets(datos, INTRODUCIR, carpet);
            datos[strcspn(datos, "\n")] = '\0';
            carp[i].contenido.longitud = strlen(datos);
            carp[i].contenido.nombre = (char *)malloc(carp[i].contenido.longitud * sizeof(char));
            for(j = 0; j < carp[i].contenido.longitud; j++)
                carp[i].contenido.nombre[j] = datos[j];
            carp[i].numero_correos = 0;
            carp[i].tipo = i;
            carp[i].eliminar = 'N';
            
            if(carp[i].contenido.longitud > *long_carp)  //2.A Entra si el nuevo nombre de la carpeta, tiene una longitud mas larga
                *long_carp = carp[i].contenido.longitud;
        }   //Bucle para llenar la estructura dinamica con los datos almacenados
    }
    
    fclose(carpet);
    return carp;
}

     
info_correos * cargarDatosCorreos(info_carpetas *carp, info_correos *corr, int num_corr, int *long_corr)
{
    FILE *corre = fopen("correos.txt", "r");
    char datos[2 * INTRODUCIR + EXTENSION];
    int tip_carp = 0;
    int i = 0, j = 0;
    
    if (corre == NULL){ //1.A En caso de que NO existan datos de correos
        corre = fopen("correos.txt", "w");
    } else {            //1.B En caso de que exitan datos de correos
        
        corr = (info_correos *)malloc(num_corr * sizeof(info_correos));
        
        for(i = 0; i < num_corr; i++){
            
            fscanf(corre, "%d", &tip_carp);
            corr[i].tipo_carpeta = tip_carp; //Posicion en la que se encuentra la posicion de la carpeta a la que pertenece///////////////////////////////////
            carp[corr[i].tipo_carpeta].numero_correos++;
                    
            fgets(datos, 2 * INTRODUCIR + EXTENSION, corre);
            datos[strcspn(datos, "\n")] = '\0';
            corr[i].leido = datos[1];           //Posicion en la que se encuentra si esta o no leido
                        
            corr[i].contenido.longitud = strlen(datos) - EXTENSION;
            corr[i].contenido.nombre = (char *)malloc(corr[i].contenido.longitud * sizeof(char));
            for(j = EXTENSION; j < corr[i].contenido.longitud + EXTENSION; j++)
                corr[i].contenido.nombre[j - EXTENSION] = datos[j];
            
            if(corr[i].contenido.longitud > (*long_corr))//2. En caso de que la longitud del nuevo nombre es mayor que el mayor actual
                *long_corr = corr[i].contenido.longitud;
        }
    }
    
    fclose(corre);
    return corr;
}


void guardarCarpetas(info_carpetas *carp, int max_carp, int inicio)
{/*Funcion que nos permite crear ficheros ".txt" con los nombres de los correos*/
    FILE *carpet = fopen("carpetas.txt", "r");
    int i = 0, j = 0;
    
    if(carpet == NULL){     //1.A En caso de que la carpeta no exista, la crea e introduce los datos
        carpet = fopen("carpetas.txt", "w");
        
        for(i = inicio; i < max_carp; i++){
            for(j = 0; j < carp[i].contenido.longitud; j++)
                fprintf(carpet, "%c", carp[i].contenido.nombre[j]);
            fprintf(carpet, "\n");
        }
        
    } else {                //1.B En caso de que la carpeta exista, anyade los datos
        fclose(carpet);
        carpet = fopen("carpetas.txt", "a");
        
        for(i = inicio; i < max_carp; i++){
            for(j = 0; j < carp[i].contenido.longitud; j++)
                fprintf(carpet, "%c", carp[i].contenido.nombre[j]);
            fprintf(carpet, "\n");
        }
    }
    
    fclose(carpet);
}


void guardarCorreos(info_correos *corr, int max_corr, int inicio, int exist_carp)
{
    FILE *corre = fopen("correos.txt", "r");
    int i = 0, j = 0;
    
    if(corre == NULL){     //1.A En caso de que la carpeta no exista, la crea e introduce los datos
        corre = fopen("correos.txt", "w");
        
        for(i = inicio; i < max_corr; i++){
            fprintf(corre, "%d:%c:", corr[i].tipo_carpeta + exist_carp, corr[i].leido);
            for(j = 0; j < corr[i].contenido.longitud; j++)
                fprintf(corre, "%c", corr[i].contenido.nombre[j]);
            fprintf(corre, "\n");
        }
        
    } else {                //1.B En caso de que la carpeta exista, anyade los datos
        fclose(corre);
        corre = fopen("correos.txt", "a");
        
        for(i = inicio; i < max_corr; i++){
            fprintf(corre, "%d:%c:", corr[i].tipo_carpeta + exist_carp, corr[i].leido);
            for(j = 0; j < corr[i].contenido.longitud; j++)
                fprintf(corre, "%c", corr[i].contenido.nombre[j]);
            fprintf(corre, "\n");
        }
    }
    
    fclose(corre);
}


info_carpetas * eliminarCarpetas(info_carpetas *carp, int *num_carp, int *num_corr)
{/*Borra las carpetas de los correos borrados, y edita la estructura dinamica de las carpetas*/
    info_carpetas *auxiliar = NULL;
    int borrar = 0;
    int eliminar = 0;
    int i = 0, j = 0;
    
    do{
        printf("\nIntroduce cuantas carpetas quieres eliminar: ");
        scanf("%d", &borrar);
        if(borrar < 0 || borrar >= (*num_carp)) //1.A En caso de que la carpeta seleccionada no exista
            errorTamanyo(&borrar);
        else if(borrar > 0){                    //1.B En caso de que haya carpetas que eliminar
            auxiliar = (info_carpetas *)malloc(((*num_carp) - borrar) * sizeof(info_carpetas));

            for(i = 0; i < borrar; i++){
                do{
                    carpetasExistentes(carp, num_carp);
                    printf("\nIntroduce el numero de la carpeta: ");
                    scanf("%d", &eliminar);
                    if(eliminar < 0 || eliminar >= (*num_carp)) //2.A En caso de que la carpeta seleccionada no exista
                        errorAccederCarp(&eliminar);
                    else                                        //2.B Indica que hay que eliminar la carpeta
                        carp[eliminar].eliminar = 'S';
                }while(eliminar < 0 || eliminar >= (*num_carp));
            }

            for(i = 0; i < (*num_carp); i++){
                if(carp[i].eliminar == 'N')         //3.A Carpetas que hay que conservar
                {
                    auxiliar[j].contenido.nombre    = carp[i].contenido.nombre;
                    auxiliar[j].contenido.longitud  = carp[i].contenido.longitud;
                    auxiliar[j].numero_correos      = carp[i].numero_correos;
                    auxiliar[j].tipo                = j;
                    j++;
                }
                else if(carp[i].eliminar == 'S')    //3.B Carpetas que hay que borrar
                {
                    free(carp[i].contenido.nombre);
                    *num_corr -= carp[i].numero_correos;
                }
            }
            
            carp = (info_carpetas *) realloc(carp, ((*num_carp) - borrar) * sizeof(info_carpetas));
            for(i = 0; i < ((*num_carp) - borrar); i++){   //Recoloca los datos almacenados en la auxiliar, a las carpetas
                carp[i].contenido.nombre    = auxiliar[i].contenido.nombre;
                carp[i].contenido.longitud  = auxiliar[i].contenido.longitud;
                carp[i].numero_correos      = auxiliar[i].numero_correos;
                carp[i].tipo                = auxiliar[i].tipo;
            }
            free(auxiliar);     //Para que no haya problemas al volver a eliminar carpetas
        }
    }while(borrar < 0 || borrar >= (*num_carp));
    
    *num_carp -= borrar;    //Cambio el valor de nim_carp al final, porque el do - while depende de su valor inicial
    return carp;            
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




/*info_carpetas * anyadirCarpetas(info_carpetas *carp, int *new_carp, int exist_carp, int *long_carp)
{
    FILE *carpet = fopen("carpetas.txt", "a");
    //char introducir[INTRODUCIR];
    int i = 0, j = 0;
    
    if(carpet == NULL){
        errorArchivo("carpetas.txt");
    } else {
        pedirNumeroCC(new_carp, "anyadir de carpetas (minimo 1)", MINCARP);
        carp = (info_carpetas *) realloc(carp, ((*new_carp) + exist_carp) * sizeof(info_carpetas));
        printf("\n\n");
        
        /*for(i = exist_carp; i < ((*new_carp) + exist_carp); i++){
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
        }*
        
        pedirNombreCarpetas(carp, long_carp, ((*new_carp) + exist_carp), exist_carp);
        
        for(i = exist_carp; i < ((*new_carp) + exist_carp); i++){
            for(j = 0; j < carp[i].contenido.longitud; j++)
                fprintf(carpet, "%c", carp[i].contenido.nombre[j]);
            fprintf(carpet, "\n");
        }
    }
    
    fclose(carpet);
    return carpet;
}*/