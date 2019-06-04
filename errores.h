/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   errores.h
 * Author: r0p3
 *
 * Created on 25 de mayo de 2019, 20:20
 */

#ifndef ERRORES_H
#define ERRORES_H


#define ROJO 4          //Para mostrar ERRORES, e indicarlos en rojo
#define BLANCO 7        //Para volver al color normal


void errorComandoDesconocido();
void errorAccederCarp(int *);
void errorPosicionCC(int *);
void errorArchivo(char *);
void errorTamanyo(int *);


#endif /* ERRORES_H */

