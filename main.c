#include <stdio.h>
#include <stdlib.h>
#include "tabla.h"
#include "errores.h"
#define MINCARP 1       //Numero minimo posible para introducir carpetas
#define MINCORR 0       //Numero minimo posible para introducir correos

/*
 *
 */

void tipoLinea(texto *, info_carpetas *, info_correos *, int*, int*, int, int, int);
void imprimirInfoCarp(info_carpetas *, int, int *);


int main(int argc, char** argv)
{
    int simbolos_simples[] = {196, 179, 218, 192, 217, 191, 194, 195, 193, 180, 197, 32};   //Numeros en el codigo ASCII de lineas simples que conformaran las celdas
    int simbolos_dobles[] = {205, 186, 201, 200, 188, 187, 203, 204, 202, 185, 206, 32};    //Numeros en el codigo ASCII de lineas dobles que conformaran las celdas
    /*Los numeros, que representan los caracteres a imprimir, estan en arrays para tenerlos juntos*/
    texto usuario;                    //Almacena el nombre de usuario
    info_carpetas *carpetas = NULL;   //Puntero a estructura, para tener con memoria dinamica las carpetas necesarias
    info_correos *correos = NULL;     //Puntero a estructura, para tener con memoria dinamica los correos necesarios
    int num_carp = 0;             //Numero total de carpetas
    int long_carp = 0;            //Longitud maxima de las carpetas
    int total_corr = 0;           //Numero total de correos
    int long_corr = 0;            //Longitud maxima de los correos
    char repetir = 'N';           //Caracter que nos servirá para decidir reimprimir una tabla o no
    char eliminar_carp = 'N';
    char eliminar_corr = 'N';

    pedirNombreUsuario(&usuario);
    imprimirEncabezadoInicial(simbolos_simples, &usuario);
    imprimirEncabezadoInicial(simbolos_dobles, &usuario);

    pedirNumeroCC(&num_carp, "carpetas (minimo 1)", MINCARP);
    carpetas = (info_carpetas *) malloc(num_carp * sizeof(info_carpetas));
    printf("\n\n");
    pedirNombreCarpetas(carpetas, &long_carp, num_carp);
    guardarCarpetas(carpetas, num_carp);
    printf("\n\n");
    pedirNumeroCC(&total_corr, "correos (minimo 0)", MINCORR);

    if(total_corr > 0){ //1. En caso de que haya correos que usar
        correos = (info_correos *) malloc(total_corr * sizeof(info_correos));
        printf("\n");
        pedirNombreCorreos(correos, &long_corr, total_corr);
        printf("\n\n");
        meterCorreosACarpetas(carpetas, correos, num_carp, total_corr);
        guardarCorreos(carpetas, correos, num_carp, total_corr);
    }

    do{ //Bucle que nos permite reimprimir, o no, una tabla
      tipoLinea(&usuario, carpetas, correos, simbolos_simples, simbolos_dobles, num_carp, long_carp, long_corr);

        do{//Bucle para introducir un caracter acertado
            printf("\n\nQuieres imprimir una nueva tabla? [ S / N ]: ");
            fflush(stdin);
            scanf("%c", &repetir);
            if(repetir != 'S' && repetir != 's' && repetir != 'N' && repetir != 'n')
                //1. Se activa en caso de NO introducir un caracter para continuar o terminar el programa
                errorComandoDesconocido();

            do{
                printf("\n\nQuieres quitar alguna carpeta? [ S / N ]: ");
                fflush(stdin);
                scanf("%c", &eliminar_carp);
                switch (eliminar_carp)
                {
                    case 'S':
                    case 's':
                        carpetas = eliminarCarpetas(carpetas, &num_carp);
                        break;

                    case 'N':
                    case 'n':
                        break;

                    default:
                        errorComandoDesconocido();
                }

            }while(eliminar_carp != 'S' && eliminar_carp != 's' && eliminar_carp != 'N' && eliminar_carp != 'n');

            /*do{
                printf("\n\nQuieres quitar algun correo? [ S / N ]: ");
                fflush(stdin);
                scanf("%c", &eliminar_corr);
                switch (eliminar_corr)
                {
                    case 'S':
                    case 's':
                        correos = ;
                        break;

                    case 'N':
                    case 'n':
                        break;

                    default:
                        errorComandoDesconocido();
                }

            }while(eliminar_corr != 'S' && eliminar_corr != 's' && eliminar_corr != 'N' && eliminar_corr != 'n');*/

        }while (repetir != 'S' && repetir != 's' && repetir != 'N' && repetir != 'n');

    }while(repetir == 'S' || repetir == 's');

    liberar(&usuario, carpetas, num_carp, correos, total_corr);
    return (EXIT_SUCCESS);
}


void tipoLinea(texto *user, info_carpetas *carp, info_correos *corr, int *simples, int *dobles, int num_carp, int long_carp, int long_corr)
{/*Funcion que nos permite seleccionar el tipo de linea que deseamos imprimir*/
    int long_total = 2;     //Variable resultante de todas las demas longitudes. Iniciualizado a 2 porque corresponde a las 2 columnas que no entran en las longitudes
    char elecc_simbol = 'S';//Variable utilizada para decidir si imprimir lineas simples o doble
    int ult_carp = 0;       //Para imprimir el numero restante de carpetas, ya que es necesario conocer la ultima posicion impresa
    int elecc_carp = 0;     //Variable para elegir la carpeta que mostrar
    long_total += long_carp + long_corr + LONG_DER;

    do{
        printf("\n\nQue tipo de linea quieres utilizar, simple o doble? [ S / D ]: ");
        fflush(stdin);
        scanf("%c", &elecc_simbol);

        switch (elecc_simbol)
        {
            case 'S':   //1.A Se activa si queremos imprimir una tabla con lineas SIMPLES
            case 's':
                imprimirInfoCarp(carp, num_carp, &elecc_carp);
                imprimirEncabezado(long_total, long_carp, long_corr, num_carp, carp[elecc_carp].numero_correos, simples, elecc_carp, ult_carp, user, carp);
                imprimirCeldasCC(num_carp, carp[elecc_carp].numero_correos, long_total, long_carp, long_corr, simples, elecc_carp, ult_carp, carp, corr);
                break;

            case 'D':   //1.B Se activa si queremos imprimir una tabla con lineas DOBLES
            case 'd':
                imprimirInfoCarp(carp, num_carp, &elecc_carp);
                imprimirEncabezado(long_total, long_carp, long_corr, num_carp, carp[elecc_carp].numero_correos, dobles, elecc_carp, ult_carp, user, carp);
                imprimirCeldasCC(num_carp, carp[elecc_carp].numero_correos, long_total, long_carp, long_corr, dobles, elecc_carp, ult_carp, carp, corr);
                break;

            default:    //1.C Se activa si caracter introducido no es para entrar en las otras 2 condiciones
                errorComandoDesconocido();
        }

    }while (elecc_simbol != 'S' && elecc_simbol != 's' && elecc_simbol != 'D' && elecc_simbol != 'd');
}


void imprimirInfoCarp(info_carpetas *carp, int num_carp, int *elecc_carp)
{
    int i = 0, j = 0;

    printf("\nIntroduce el contenido de la carpeta que quieras ver:\n");
    /*for(i = 0; i < num_carp; i++){
        printf("Inserte [ %d ] para acceder a la carpeta [ ", carp[i].tipo);
        for(j = 0; j < carp[i].contenido.longitud; j++)
            printf("%c", carp[i].contenido.nombre[j]);
        printf(" ]\n");
    }*/
    carpetasExistentes(carp, &num_carp);

    do{
        printf("\nIntroduce el numero de la carpeta: ");
        scanf("%d", elecc_carp);

        if(*elecc_carp < 0 || *elecc_carp >= num_carp){ //En caso de que la carpeta seleccionada no exista
            errorAccederCarp(elecc_carp);
        }
    }while(*elecc_carp < 0 || *elecc_carp >= num_carp);
}
