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


info_carpetas * eliminarCarpetas(info_carpetas *carp, int *num_carp, int *total_corr)
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
                    auxiliar[j].archivo.nombre      = carp[i].archivo.nombre;
                    auxiliar[j].archivo.longitud    = carp[i].archivo.longitud;
                    auxiliar[j].contenido.nombre    = carp[i].contenido.nombre;
                    auxiliar[j].contenido.longitud  = carp[i].contenido.longitud;
                    auxiliar[j].numero_correos      = carp[i].numero_correos;
                    auxiliar[j].tipo                = j;
                    j++;
                }
                else if(carp[i].eliminar == 'S')    //3.B Carpetas que hay que borrar
                {
                    remove(carp[i].archivo.nombre);
                    free(carp[i].contenido.nombre);
                    free(carp[i].archivo.nombre);
                    *total_corr -= carp[i].numero_correos;
                }
            }
            carp = (info_carpetas *) realloc(carp, ((*num_carp) - borrar) * sizeof(info_carpetas));
            for(i = 0; i < ((*num_carp) - borrar); i++){   //Recoloca los datos almacenados en la auxiliar, a las carpetas
                carp[i].archivo.nombre      = auxiliar[i].archivo.nombre;
                carp[i].archivo.longitud    = auxiliar[i].archivo.longitud;
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

/*
 
info_carpetas * eliminarCarpetas(info_carpetas *carp, int *num_carp)
{/*Borra las carpetas de los correos borrados, y edita la estructura dinamica de las carpetas*
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
            if(borrar > 0)
            {
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
                return auxiliar;    
            }  
            
        }
    }while(borrar < 0 || borrar >= (*num_carp));
    
    return carp;            
}*/
