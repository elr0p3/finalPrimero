/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <windows.h>    //Biblioteca para mostrar colores
#include "tabla.h"
#define VERDE 47        //Para imprimir la carpeta seleccionada, e indicarla en verde
#define BLANCO 7        //Para volver al color normal

/*
 *
 */

void imprimirEncabezadoInicial(int *simbolos, texto *user)
{
    int long_usuario = user->longitud - 1;
        
    printf("\n\n");
    imprimirLineas(simbolos[0], simbolos[5], simbolos[2], simbolos[0], simbolos[5], simbolos[0], simbolos[11], user->longitud, long_usuario, long_usuario);
    textoEncabezado(simbolos[1], user->longitud, user->longitud, user->nombre);
    imprimirLineas(simbolos[0], simbolos[4], simbolos[3], simbolos[0], simbolos[4], simbolos[0], simbolos[11], user->longitud, long_usuario, long_usuario);
    printf("\n");
}


void imprimirLineas(int ln_hrzntl_A, int ln_hrzntl_B, int esq_izq, int esq_cntrl_izq, int esq_cntrl_der, int esq_der, int final, int long_necesaria, int long_carp, int long_corr)
{/*Funcion que nos permite imprimir las lineas horizontales que separan las celdas, ya que para imprimirlas hay que seguir el mismo patron*/
    int i = 0;

    printf("%c", esq_izq);
    /*1_1   Imprime el primer caracter de la linea*/
    for(i = 0; i <= long_necesaria; i++)
    {/*Es posible juntar el primer if y elif, pero de esta forma puedo reutilizar esta funcion en la funcion
      imprimirCorreosRestantes(), ya que es necesario dejar espacios donde antes estaban las carpetas*/
        if (i < long_carp)
        //1.A Se activa si NO hemos llegado al cruce que separa las carpetas de correos
            printf("%c", ln_hrzntl_A);
        else if (i > long_carp && i != long_carp + long_corr + 1 && i != long_carp + 1 + long_corr + 1 + LONG_DER)
        //1.B Se activa si hemos pasado el primer cruce y NO nos encontramos en el segundo
            printf("%c", ln_hrzntl_B);
        else if (i == long_carp)
        //1.C Se activa si nos encontramos en el cruce carpetas con el de correos
            printf("%c", esq_cntrl_izq);
        else if (i == long_carp + 1 + long_corr)
        //1.D Se activa si nos encontramos en el cruce correos con el de la derecha
        //Este +1 se debe a que hay una columna que separa las carpetas de los correos, asi que esta para que concidan todas las lineas
            printf("%c", esq_cntrl_der);
        else if(i == long_carp + 1 + long_corr + 1 + LONG_DER)
        //1.E Se activa al llegar al final de la linea de las celdas de leido o no
        //Los dos +1 se deben a las 2 lineas verticales que hay en medio
            printf("%c", esq_der);
    }
    /*1_2   Bucle encargado de imprimir el "cuerpo" de la linea horizontal*/
    printf("%c\n", final);//Normalmente imprime un espacio, pero en caso de que el titulo se mas largo que la longitud total, es util para imprimir el ultimo caracter
    /*1_3   Imprime el ultimo caracter de la linea*/
}


void textoEncabezado(int barra, int long_total, int long_nombr, char *nombre)
{/*Funcion que imprime el nombre de usuario en el encabezado*/
    int centrar = (long_total / 2) - (long_nombr / 2);       //Para centrar el nombre de usuario por la izquierda
    int restante = 1 + long_total - (long_nombr + centrar);  //Para centrar el nombre de usuario por la dereha
    int i = 0;
    
    printf("%c", barra);
    for(i = 0; i < long_total; i++)
        printf(" ");
    printf("%c\n%c", barra, barra);
    for(i = 0; i < centrar; i++)
        printf(" ");
    for(i = 0; i < long_nombr; i++)
        printf("%c", nombre[i]);
    printf("%*c\n%c", restante, barra, barra);
    for(i = 0; i < long_total; i++)
        printf(" ");
    printf("%c\n", barra);
}


void textoCarpetas(int barra, int long_carp, int long_nombr, int tipo_carp, int elecc_carp, char *nombre)
{/*Funcion que imprime el contenido de las carpetas*/
    int i = 0;
    
    printf("%c", barra);
    if(tipo_carp == elecc_carp)    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VERDE);
    for(i = 0; i < long_carp; i++){
        if(i < long_nombr)  printf("%c", nombre[i]);
        else                printf(" ");
    }
    //printf("%-*s", long_carp, nombre);
    
    if(tipo_carp == elecc_carp)    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLANCO);
    printf("%c", barra);
}


void textoCorreos(int barra, int long_corr, int elecc_carp, int *ult_corr, info_correos *corr)
{/*Funcion que permite mostrar el contendio de los correos*/
    int j = 0;
    
    do{
        //*ult_corr += 1; //No utilizo el atajo ++ ya que no funciona 
        (*ult_corr)++;      //Suma +1 a la posicion del ultimo correo para no volverlo a imprimirlo 
        if(corr[*ult_corr].tipo_carpeta == elecc_carp){ 
            /*Como los correos estan es su propia estructura, esta condicion verifica
            que la carpeta a la que pertenecen los correos es la que se ha seleccionado para imprimir*/
            for(j = 0; j < long_corr; j++){
                if(j < corr[*ult_corr].contenido.longitud)  printf("%c", corr[*ult_corr].contenido.nombre[j]);
                else                                        printf(" ");
            }
            //printf("%-*s", long_corr, corr[*ult_corr].contenido.nombre);
        }
    }while(corr[*ult_corr].tipo_carpeta != elecc_carp);
    printf("%c %c %c\n", barra, corr[*ult_corr].leido, barra);
}


void imprimirEncabezado(int long_total, int long_carp, int long_corr, int num_carp, int num_corr, int *simbolos, int tipo_corr, int ult_carp, texto *user, info_carpetas *carp)
{/*Funcion que sirve para imprimir el encabezado de la tabla*/
    int long_usuario = user->longitud - 1;
    
    printf("\n");
    if(user->longitud <= long_total)//1.A Se activa en caso de que la longitud total se mayor o igual que la del nombre
        imprimirLineas(simbolos[0], simbolos[0], simbolos[2], simbolos[0], simbolos[0], simbolos[5], simbolos[11], long_total, long_carp, long_corr);
    else                            //2.B Se activa en caso de que la longitud total se menor que la del nombre
        imprimirLineas(simbolos[0], simbolos[0], simbolos[2], simbolos[0], simbolos[0], simbolos[0], simbolos[5], long_usuario, long_carp, long_corr);
    /*1_1   Parte encargada de imprimir la primera linea del encabezado*/
    
    
    if(user->longitud <= long_total)//2.A Se activa en caso de que la longitud total se mayor o igual que la del nombre
        textoEncabezado(simbolos[1], long_total, user->longitud, user->nombre);
    else                            //2.B Se activa en caso de que la longitud total se menor que la del nombre
        textoEncabezado(simbolos[1], user->longitud, user->longitud, user->nombre);
    /*1_2   Parte encargada de imprimir todos los espacios y laterales del encabezado*/

    
    if(user->longitud <= long_total)//3.A Se activa en caso de que la longitud total se mayor o igual que la del nombre
        if(num_corr == 0){//3.A.A En caso de que NO haya correos que mostrar
            imprimirLineas(simbolos[0], simbolos[0], simbolos[7], simbolos[6], simbolos[0], simbolos[4], simbolos[11], long_total, long_carp, long_corr);
            imprimirCarpetasRestantes(num_carp, long_carp, simbolos, ult_carp, tipo_corr, carp);
        }
        else              //3.A.B En caso de que haya correos que mostrar
            imprimirLineas(simbolos[0], simbolos[0], simbolos[7], simbolos[6], simbolos[6], simbolos[9], simbolos[11], long_total, long_carp, long_corr);
    
    else                            //2.B Se activa en caso de que la longitud total se menor que la del nombre
        if(num_corr == 0){//1.A En caso de que NO haya correos que mostrar
            imprimirLineas(simbolos[0], simbolos[0], simbolos[7], simbolos[6], simbolos[0], simbolos[0], simbolos[4], long_usuario, long_carp, long_corr);
            imprimirCarpetasRestantes(num_carp, long_carp, simbolos, ult_carp, tipo_corr, carp);
        }
        else              //1.B En caso de que haya correos que mostrar
            imprimirLineas(simbolos[0], simbolos[0], simbolos[7], simbolos[6], simbolos[6], simbolos[6], simbolos[4], long_usuario, long_carp, long_corr);
    /*1_3   Parte encargada de imprimir la ultima linea del encabezado*/
}


void imprimirCeldasCC(int num_carp, int num_corr, int long_total, int long_carp, int long_corr, int *simbolos, int elecc_carp, int ult_carp, info_carpetas *carp, info_correos *corr)
{/*Funcion que imprime las celdas comunes, ademas de las no comunes segun el numero de carpetas y correos*/
    int numero_peque = 0;       //Variable que permite almacenar el numero mas pequeño ya sea de carpetas o correos
    int dif_num_carp_y_corr = 0;//Variable que determina la diferencia entre el numero de carptes frente al numero de correos
    int ult_corr = -1;          //Para controlar los correos necesarios a imprimir
    int i = 0;

    if (num_carp <= num_corr){ //1.A Si el numero de correos es mayor o igual
        numero_peque = num_carp;
        dif_num_carp_y_corr = num_corr - num_carp;
    }
    else{                      //1.B Si el numero de carpetas es mayor
        numero_peque = num_corr;
        dif_num_carp_y_corr =  num_carp - num_corr;
    }

    for(i = 0; i < numero_peque; i++){//El numero mas pequeño ayuda a imprimir las celdas comunes
        textoCarpetas(simbolos[1], long_carp, carp[i].contenido.longitud, carp[i].tipo, elecc_carp, carp[i].contenido.nombre);
        ult_carp = i + 1;   //El +1 esta para que imprima en la siguiente posicion
        textoCorreos(simbolos[1], long_corr, elecc_carp, &ult_corr, corr);
        /*1_    Parte encargada de imprimir los espacios y laterales, y de hacer la altura de las celdas*/

        if (i != numero_peque - 1)  //2.A Entramos si NO nos encontremos en la ultima linea
            imprimirLineas(simbolos[0], simbolos[0], simbolos[7], simbolos[10], simbolos[10], simbolos[9], simbolos[11], long_total, long_carp, long_corr);
        else{                       //2.B Entramos si nos encontremos en la ultima linea
            if (num_carp == num_corr)//2.B.A Se activa si el numero de carpetas y el de correos es el mismo; no habria que imprimir mas celdas
                imprimirLineas(simbolos[0], simbolos[0], simbolos[3], simbolos[8], simbolos[8], simbolos[4], simbolos[11], long_total, long_carp, long_corr);
            else if (num_carp <  num_corr){
                //2.B.B Se activa si el numero de carpetas es inferior al de correos; habria que imprimir mas celdas de correos
                imprimirLineas(simbolos[0], simbolos[0], simbolos[3], simbolos[10], simbolos[10], simbolos[9], simbolos[11], long_total, long_carp, long_corr);
                imprimirCorreosRestantes(dif_num_carp_y_corr, long_total, long_carp, long_corr, simbolos, ult_corr, elecc_carp, corr);
            }
            else if (num_carp > num_corr){
                //2.B.B Se activa si el numero de carpetas es superior al de correos; habria que imprimir mas celdas de carpetas
                imprimirLineas(simbolos[0], simbolos[0], simbolos[7], simbolos[10], simbolos[8], simbolos[4], simbolos[11], long_total, long_carp, long_corr);
                imprimirCarpetasRestantes(dif_num_carp_y_corr, long_carp, simbolos, ult_carp, elecc_carp, carp);
            }
        }
        /*2_    Parte encargada de imprimir las lineas que separan cada celda verticalmente*/
    }
}


void imprimirCarpetasRestantes(int dif_num_carp_y_corr, int long_carp, int *simbolos, int upcc, int elecc_carp, info_carpetas *carp)
{/*Funcion encargada de imprimir las celdas de las carpetas restantes*/
    int i = 0;

    for(i = upcc; i < dif_num_carp_y_corr + upcc; i++)//Se le suma la ultima pocision de carpetas, para que el iterador sea 'i' y no 'upcc'
    {//La diferencia entre los numeros de carpetas y correos, se refiere al numero de celdas de CARPETAS restantes
        
        textoCarpetas(simbolos[1], long_carp, carp[i].contenido.longitud, carp[i].tipo, elecc_carp, carp[i].contenido.nombre);
        printf("\n");
        /*1_    Parte encargada de imprimir el texto y laterales, y de hacer la altura de las celdas*/

        if (i != (dif_num_carp_y_corr + upcc) - 1)   //1.A Se activa si NO nos encontramos en la ultima linea a imprimir
            imprimirLineas(simbolos[0], simbolos[0], simbolos[7], simbolos[9], simbolos[9], simbolos[9], simbolos[11], long_carp, long_carp, long_carp);
        else                                //1.B Se activa si estamos en la ultima linea a imprimir
            imprimirLineas(simbolos[0], simbolos[0], simbolos[3], simbolos[4], simbolos[4], simbolos[4], simbolos[11], long_carp, long_carp, long_carp);
        //En este los argumentos 4, 5, 7 y 8 no son necesarios, pero estan repetidos para no desaprovechar la funcion
        /*2_    Parte encargada de imprimir las lineas que separan de forma vertical las celdas*/
  }
}


void imprimirCorreosRestantes(int dif_num_carp_y_corr, int long_total, int long_carp, int long_corr, int *simbolos, int ult_corr, int elecc_carp, info_correos *corr)
{/*Funcion que se encarga de imprimir las celdas de las correos restantes*/
    int i = 0;
    
    for(i = 0; i < dif_num_carp_y_corr; i++){//Se le suma la ultima pocision de carpetas, para que el iterador sea 'i' y no 'upcc'
    //La diferencia entre los numeros de carpetas y correos, se refiere al numero de celdas de CORREOS restantes
        
        for(int j = 0; j < long_carp + 1; j++)//El +1 indica la primera barra a la izquierda
            printf(" ");
        printf("%c", simbolos[1]);
        textoCorreos(simbolos[1], long_corr, elecc_carp, &ult_corr, corr);
        /*1_    Parte encargada de imprimir los espacios y laterales, para darle altura a las celdas*/

        if (i != dif_num_carp_y_corr - 1)     //1.A Se activa si NO estamos en la ultima linea a imprimir
            imprimirLineas(simbolos[11], simbolos[0], simbolos[11], simbolos[7], simbolos[10], simbolos[9], simbolos[11], long_total, long_carp, long_corr);
        else                                            //1.B Se activa si estamos en la ultima linea a imprimir
            imprimirLineas(simbolos[11], simbolos[0], simbolos[11], simbolos[3], simbolos[8], simbolos[4], simbolos[11], long_total, long_carp, long_corr);
        /*2_    Parte encargada de imprimir las lineas que separan de forma vertical las celdas*/
  }
}