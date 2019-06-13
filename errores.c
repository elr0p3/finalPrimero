/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <windows.h>    //Biblioteca para mostrar colores
#include "errores.h"

void errorComandoDesconocido()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ROJO);
    fprintf(stderr, "ERROR. Comando desconocido!\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLANCO);
}


void errorAccederCarp(int *elecc_carp)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ROJO);
    fprintf(stderr, "ERROR. La carpeta [ %d ] no existe!\n", *elecc_carp);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLANCO);
}


void errorPosicionCC(int *posicion)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ROJO);
    fprintf(stderr, "ERROR. No se puede acceder a la posicion [ %d ]!\n", *posicion);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLANCO);
}


void errorArchivo(char *archivo)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ROJO);
    fprintf(stderr, "ERROR. Abrir el archivo '%s'!\n", archivo);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLANCO);
}


void errorTamanyo(int *numero)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ROJO);
    fprintf(stderr, "ERROR. El tamanyo %d esta fuera de rango!\n", *numero);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLANCO);
}

void errorSinDatos()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ROJO);
    fprintf(stderr, "ERROR. No hay ficheros de los cuales extraer datos, o no hay datos en los ficheros!\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLANCO);
}