/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: r0p3
 *
 * Created on 25 de mayo de 2019, 20:19
 */

#include <stdio.h>
#include <stdlib.h>
#include "tabla.h"
#include "errores.h"
#define MINCARP 1       //Numero minimo posible para introducir carpetas
#define MINCORR 0       //Numero minimo posible para introducir correos

/*
 *
 */

info_carpetas * introducirDatosCarpetas(info_carpetas *carp, datos *_carp);
info_correos * introducirDatosCorreo(info_carpetas *carp, info_correos *corr, datos _carp, datos *_corr);
info_carpetas * anyadirDatosCarp(info_carpetas *carp, datos *_carp);
info_correos * anyadirDatosCorr(info_carpetas *carpetas, info_correos *correos, datos *_carp, datos *_corr);

void tipoLinea(texto *, info_carpetas *, info_correos *, int*, int*, int, int, int, int);
void imprimirInfoCarp(info_carpetas *, int, int *);

//void reajustarDatos(texto *, info_carpetas *, info_correos *, datos *, datos *, char, int *, int *);

void recargarDatosAntiguos(almacen *puntero, datos *_carp, datos *_corr, char *old_data);

void modifCarpetas(almacen *puntero, datos *_carp, datos *_corr, char old_data, char *modif_carp);
void menosCarpetas(almacen *puntero, datos *_carp, datos *_corr, char old_data);
info_carpetas * masCarpetas(info_carpetas *carp, datos *_carp, char old_data);

info_correos * modifCorreos(info_carpetas *carp, info_correos *corr, datos *_carp, datos *_corr, char old_data, char *);
info_correos * menosCorreos(info_carpetas *carp, info_correos *corr, datos *_carp, datos *_corr, char);
info_correos * masCorreos(info_carpetas *carp, info_correos *corr, datos *_carp, datos *_corr, char);


int main(int argc, char** argv)
{   /*Los numeros, que representan los caracteres a imprimir, estan en arrays para tenerlos juntos*/
    int simbolos_simples[] = {196, 179, 218, 192, 217, 191, 194, 195, 193, 180, 197, 32};
    //Numeros en el codigo ASCII de lineas simples que conformaran las celdas
    int simbolos_dobles[] = {205, 186, 201, 200, 188, 187, 203, 204, 202, 185, 206, 32};
    //Numeros en el codigo ASCII de lineas dobles que conformaran las celdas
    texto usuario;                    //Almacena el nombre de usuario
    almacen puntero = {NULL, NULL};
    datos _carp = {0, 0, 0, 0};
    datos _corr = {0, 0, 0, 0}; 
    char old_data = 'N', repetir = 'N', anyadir = 'N'; //Cargar datos guardados en ficheros / Anyadir carp-corr
    char modif_carp = 'N', modif_corr = 'N';
    
    pedirNombreUsuario(&usuario);
    imprimirEncabezadoInicial(simbolos_simples, &usuario);
    imprimirEncabezadoInicial(simbolos_dobles, &usuario);
    _carp.exist = obtenerCarpExistentes(_carp.exist);
    _corr.exist = obtenerCorrExistentes(_corr.exist);
    
    if(_carp.exist <= 0){
        puntero.carp = introducirDatosCarpetas(puntero.carp, &_carp);
        puntero.corr = introducirDatosCorreo(puntero.carp, puntero.corr, _carp, &_corr);
    } else {
        do{
            printf("\nQuieres cargar [ C ] o borrar [ B ] los datos de las carpetas anteriores? [ C / B / N ]: ");
            fflush(stdin);  scanf("%c", &old_data);
            switch (old_data)
            {
                case 'C':   //1.A En caso de querercargar datos antiguos
                case 'c':
                    puntero.carp = cargarDatosCarpetas(puntero.carp, _carp.exist, &(_carp.longtd));
                    puntero.corr = cargarDatosCorreos(puntero.carp, puntero.corr, _corr.exist, &(_corr.longtd));
                    do{
                        printf("\nQuieres anyadir mas correos y carpetas [ S / N ]: ");
                        fflush(stdin);  scanf("%c", &anyadir);
                        if(anyadir != 'S' && anyadir != 's' && anyadir != 'N' && anyadir != 'n')
                            errorComandoDesconocido();
                        else if (anyadir == 'S' || anyadir == 's'){
                            puntero.carp = anyadirDatosCarp(puntero.carp, &_carp);
                            puntero.corr = anyadirDatosCorr(puntero.carp, puntero.corr, &_carp, &_corr);
                        }
                    }while(anyadir != 'S' && anyadir != 's' && anyadir != 'N' && anyadir != 'n');
                    _carp.exist += _carp.new;
                    _carp.total  = _carp.exist;
                    _corr.exist += _corr.new;
                    _corr.total  = _corr.exist;
                    break;

                case 'B':   //1.B En caso de querer borrar los datos antiguos
                case 'b':
                    remove("carpetas.txt");
                    _carp.exist = 0;
                    remove("correos.txt");
                    _corr.exist = 0;
                case 'N':   //1.C En caso de querer introducir nuevos datos
                case 'n':
                    puntero.carp = introducirDatosCarpetas(puntero.carp, &_carp);
                    puntero.corr = introducirDatosCorreo(puntero.carp, puntero.corr, _carp, &_corr);
                    break;

                default:    //1.D En caso de que se introduzca una orden erronea
                    errorComandoDesconocido();
            }
        }while (old_data != 'C' && old_data != 'c' && old_data != 'B' && old_data != 'b' && old_data != 'N' && old_data != 'n');
    }
   
    do{ //Bucle que nos permite reimprimir, o no, una tabla
      tipoLinea(&usuario, puntero.carp, puntero.corr, simbolos_simples, simbolos_dobles, _carp.total, _carp.longtd, _corr.longtd, _carp.exist);

        do{//Bucle para introducir un caracter acertado
            printf("\n\nQuieres imprimir una nueva tabla? [ S / N ]: ");
            fflush(stdin);  scanf("%c", &repetir);
            if(repetir != 'S' && repetir != 's' && repetir != 'N' && repetir != 'n')
                //2. Se activa en caso de NO introducir un caracter para continuar o terminar el programa
                errorComandoDesconocido();
            if(repetir == 'S' || repetir == 's'){
                if((old_data == 'N' || old_data == 'n') && _carp.exist > 0)
                    recargarDatosAntiguos(&puntero, &_carp, &_corr, &old_data);
                
                do{
                    modifCarpetas(&puntero, &_carp, &_corr, old_data, &modif_carp);
                }while (modif_carp != 'Q' && modif_carp != 'q' && modif_carp != 'A' && modif_carp != 'a' && modif_carp != 'N' && modif_carp != 'n');

                do{
                    puntero.corr = modifCorreos(puntero.carp, puntero.corr, &_carp, &_corr, old_data, &modif_corr);
                }while (modif_corr != 'Q' && modif_corr != 'q' && modif_corr != 'A' && modif_corr != 'a' && modif_corr != 'N' && modif_corr != 'n');
            }
        }while (repetir != 'S' && repetir != 's' && repetir != 'N' && repetir != 'n');

    }while(repetir == 'S' || repetir == 's');
    
    if(old_data != 'C' && old_data != 'c')
        //En caso de que no se haya elegido recargar los datos antiguos, para guardalos en ficheros
        finalGuardarDatos(puntero.carp, puntero.corr, _carp.total, _corr.total, _carp.exist, _corr.exist, old_data);
    free(usuario.nombre);
    liberar(puntero.carp, _carp.total, puntero.corr, _corr.total);
    
    return (EXIT_SUCCESS);
}


info_carpetas * introducirDatosCarpetas(info_carpetas *carp, datos *_carp)
{
    pedirNumeroCC(&(_carp->new), "de carpetas (minimo 1)", MINCARP);
    printf("\n\n");
    _carp->total = _carp->new;
    carp = (info_carpetas *) malloc(_carp->new * sizeof(info_carpetas));
    pedirNombreCarpetas(carp, &(_carp->longtd), _carp->new, INICIO);
    printf("\n\n");
        
    return carp;
}


info_correos * introducirDatosCorreo(info_carpetas *carp, info_correos *corr, datos _carp, datos *_corr)
{
    pedirNumeroCC(&(_corr->new), "de correos (minimo 0)", MINCORR);
    _corr->total = _corr->new;

    if(_corr->new > 0){ //1. En caso de que haya correos que usar
        corr = (info_correos *) malloc(_corr->new * sizeof(info_correos));
        printf("\n");
        pedirNombreCorreos(corr, &(_corr->longtd), _corr->new, INICIO);
        printf("\n\n");
        meterCorreosACarpetas(carp, corr, _carp.new, _corr->new, INICIO);
    }
    
    return corr;
}


info_carpetas * anyadirDatosCarp(info_carpetas *carp, datos *_carp)
{
    pedirNumeroCC(&(_carp->new), "anyadir de carpetas (minimo 1)", MINCARP);
    carp = (info_carpetas *) realloc(carp, (_carp->exist + _carp->new) * sizeof(info_carpetas));
    printf("\n\n");
    pedirNombreCarpetas(carp, &(_carp->longtd), (_carp->exist + _carp->new), _carp->exist);
    guardarCarpetas(carp, (_carp->exist + _carp->new), _carp->exist);
    printf("\n\n");
    
    return carp;
}


info_correos * anyadirDatosCorr(info_carpetas *carp, info_correos *corr, datos *_carp, datos *_corr)
{
    pedirNumeroCC(&(_corr->new), "anyadir de correos (minimo 0)", MINCORR);

    if(_corr->new > 0){ //1. En caso de que haya correos que usar
        corr = (info_correos *) realloc(corr, (_corr->exist + _corr->new) * sizeof(info_correos));
        printf("\n");
        pedirNombreCorreos(corr, &(_corr->longtd), (_corr->exist + _corr->new), _corr->exist);
        printf("\n\n");
        meterCorreosACarpetas(carp, corr, (_carp->exist + _carp->new), (_corr->exist + _corr->new), _corr->exist);
        guardarCorreos(corr, (_corr->exist + _corr->new), _corr->exist, INICIO);
    }
    
    return corr;
}


void tipoLinea(texto *user, info_carpetas *carp, info_correos *corr, int *simples, 
                int *dobles, int total_carp, int long_carp, int long_corr, int exist_carp)
{/*Funcion que nos permite seleccionar el tipo de linea que deseamos imprimir*/
    int long_total = 2;     //Variable resultante de todas las demas longitudes
    //Iniciualizado a 2 porque corresponde a las 2 columnas que no entran en las longitudes
    char elecc_simbol = 'S';//Variable utilizada para decidir si imprimir lineas simples o doble
    int ult_carp = 0;       //Para imprimir el numero restante de carpetas, necesario conocer la ultima posicion impresa
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
                imprimirInfoCarp(carp, total_carp, &elecc_carp);
                imprimirEncabezado(long_total, long_carp, long_corr, total_carp, carp[elecc_carp].numero_correos, simples, elecc_carp, ult_carp, user, carp);
                imprimirCeldasCC(total_carp, carp[elecc_carp].numero_correos, long_total, long_carp, long_corr, simples, elecc_carp, ult_carp, carp, corr);
                break;
            
            case 'D':   //1.B Se activa si queremos imprimir una tabla con lineas DOBLES
            case 'd':
                imprimirInfoCarp(carp, total_carp, &elecc_carp);
                imprimirEncabezado(long_total, long_carp, long_corr, total_carp, carp[elecc_carp].numero_correos, dobles, elecc_carp, ult_carp, user, carp);
                imprimirCeldasCC(total_carp, carp[elecc_carp].numero_correos, long_total, long_carp, long_corr, dobles, elecc_carp, ult_carp, carp, corr);
                break;
                
            default:    //1.C Se activa si caracter introducido no es para entrar en las otras 2 condiciones
                errorComandoDesconocido();
        }
        
    }while (elecc_simbol != 'S' && elecc_simbol != 's' && elecc_simbol != 'D' && elecc_simbol != 'd');
}


void imprimirInfoCarp(info_carpetas *carp, int total_carp, int *elecc_carp)
{
    printf("\nIntroduce el contenido de la carpeta que quieras ver:\n");
    carpetasExistentes(carp, &total_carp);

    do{
        printf("\nIntroduce el numero de la carpeta: ");
        scanf("%d", elecc_carp);
        
        if(*elecc_carp < 0 || *elecc_carp >= total_carp){ //En caso de que la carpeta seleccionada no exista
            errorAccederCarp(elecc_carp);
        }
    }while(*elecc_carp < 0 || *elecc_carp >= total_carp);
}


void recargarDatosAntiguos(almacen *puntero, datos *_carp, datos *_corr, char *old_data)
{
    char recargar = 'N';
    
    do{
        printf("\n\nQuieres cargar tus datos antiguos [ S / N ]: ");
        fflush(stdin);  scanf("%c", &recargar);
        if(recargar == 'S' || recargar == 's')
            *old_data = 'C';

        if(recargar == 'S' || recargar == 's'){
            guardarCarpetas(puntero->carp, _carp->total, INICIO);
            guardarCorreos(puntero->corr, _corr->total, INICIO, _carp->exist);
            liberar(puntero->carp, _carp->total, puntero->corr, _corr->total);
            _carp->exist   += _carp->total;
            _corr->exist   += _corr->total;
            puntero->carp  = cargarDatosCarpetas(puntero->carp, _carp->exist, &(_carp->longtd));
            _carp->total   = _carp->exist;
            puntero->corr  = cargarDatosCorreos(puntero->carp, puntero->corr, _corr->exist, &(_corr->longtd));
            _corr->total   = _corr->exist;
        }
        else if(recargar != 'S' && recargar != 's' && recargar != 'N' && recargar != 'n')
            errorComandoDesconocido();
    }while(recargar != 'S' && recargar != 's' && recargar != 'N' && recargar != 'n');
}


void modifCarpetas(almacen *puntero, datos *_carp, datos *_corr, char old_data, char *modif_carp)
{
    printf("\n\nQuieres modificar las carpetas? [ Quitar / Anyadir / No ] [ Q / A / N ]: ");
    fflush(stdin);  scanf("%c", modif_carp);
    switch (*modif_carp)
    {
        case 'Q':   //3.A En caso de querer quitar carpetas
        case 'q':
            menosCarpetas(puntero, _carp, _corr, old_data);
            break;

        case 'A':   //3.B En caso de querer anyadir carpetas
        case 'a':
            puntero->carp = masCarpetas(puntero->carp, _carp, old_data);
            break;

        case 'N':   //3.C En caso de no querer cambiar nada
        case 'n':
            break;

        default:    //3.D En caso de pulsar una tecla equivocada
            errorComandoDesconocido();
    }
}


void menosCarpetas(almacen *puntero, datos *_carp, datos *_corr, char old_data)
{
    int mantener_carp = 0;
    int mantener_corr = 0;
    
    mantener_carp = obtenerCarpRstnts(puntero->carp, _carp->total);
    if(mantener_carp > 0){
        puntero->carp = eliminarCarpetas(puntero->carp, puntero->corr, &(_carp->total), _corr->total, mantener_carp, &mantener_corr);
        puntero->corr = eliminarCorreos(puntero->corr, &(_corr->total), mantener_corr);
        if(old_data == 'C' || old_data == 'c'){
            remove("carpetas.txt");
            guardarCarpetas(puntero->carp, _carp->total, INICIO);
            _carp->exist = _carp->total;
            remove("correos.txt");
            guardarCorreos(puntero->corr, _corr->total, INICIO, INICIO);
            _corr->exist = _corr->total;
        }
    }
}


info_carpetas * masCarpetas(info_carpetas *carp, datos *_carp, char old_data)
{
    pedirNumeroCC(&(_carp->new), "anyadir de carpetas", INICIO);
    if(_carp->new > 0){
        carp = (info_carpetas *) realloc(carp, (_carp->total + _carp->new) * sizeof(info_carpetas));
        if(old_data == 'C' || old_data == 'c'){
            pedirNombreCarpetas(carp, &(_carp->longtd), (_carp->total + _carp->new), _carp->exist);
            guardarCarpetas(carp, (_carp->total + _carp->new), _carp->exist);
            _carp->exist = _carp->total + _carp->new;
        } else
            pedirNombreCarpetas(carp, &(_carp->longtd), (_carp->total + _carp->new), _carp->total);
        _carp->total += _carp->new;
    }
    
    return carp;
}


info_correos * modifCorreos(info_carpetas *carp, info_correos *corr, datos *_carp, datos *_corr, char old_data, char *modif_corr)
{
    printf("\n\nQuieres modificar los correos? [ Quitar / Anyadir / No ] [ Q / A / N ]: ");
    fflush(stdin);  scanf("%c", modif_corr);
    switch (*modif_corr)
    {
        case 'Q':   //4.A En caso de querer quitar correos
        case 'q':
            corr = menosCorreos(carp, corr, _carp, _corr, old_data);
            break;

        case 'A':   //4.B En caso de querer anyadir correos
        case 'a':
            corr = masCorreos(carp, corr, _carp, _corr, old_data);
            break;

        case 'N':   //4.C En caso de no querer cambiar nada
        case 'n':
            break;

        default:    //4.D En caso de pulsar una tecla equivocada
            errorComandoDesconocido();
    }
    
    return corr;
}


info_correos * menosCorreos(info_carpetas *carp, info_correos *corr, datos *_carp, datos *_corr, char old_data)
{
    int mantener_corr = 0;
    
    mantener_corr = obtenerCorrRstnts(carp, corr, _corr->total);
    if(mantener_corr > 0){
        corr = eliminarCorreos(corr, &(_corr->total), mantener_corr);
        if(old_data == 'C' || old_data == 'c'){
            remove("correos.txt");
            guardarCorreos(corr, _corr->total, INICIO, INICIO);
            _corr->exist = _corr->total;
        }
    }
    
    return corr;
}


info_correos * masCorreos(info_carpetas *carp, info_correos *corr, datos *_carp, datos *_corr, char old_data)
{
    pedirNumeroCC(&(_corr->new), "anyadir de correos", INICIO);
    if(_corr->new > 0){ //.A En caso de que haya correos que usar
        corr = (info_correos *) realloc(corr, (_corr->total + _corr->new) * sizeof(info_correos));
        if(old_data == 'C' || old_data == 'c'){
            pedirNombreCorreos(corr, &(_corr->longtd), (_corr->total + _corr->new), _corr->exist);
            meterCorreosACarpetas(carp, corr, _carp->total, (_corr->total + _corr->new), _corr->exist);
            guardarCorreos(corr, (_corr->total + _corr->new), _corr->exist, INICIO);
            _corr->exist = _corr->total + _corr->new;
        } else {
            pedirNombreCorreos(corr, &(_corr->longtd), (_corr->total + _corr->new), _corr->total);
            meterCorreosACarpetas(carp, corr, _carp->total, (_corr->total + _corr->new), _corr->total);
        }
        _corr->total += _corr->new;
    }
    
    return corr;
}





/*{
    remove("carpetas.txt");
    guardarCarpetas(carpetas, _carp.total, INICIO);
    _carp.exist = _carp.total;
    remove("correos.txt");
    guardarCorreos(correos, _corr.total, INICIO, INICIO);
    _corr.exist = _corr.total;
}*/




/*pedirNumeroCC(&(_carp.new), "anyadir de carpetas", INICIO);
if(_carp.new > 0){
    carpetas = (info_carpetas *) realloc(carpetas, (_carp.total + _carp.new) * sizeof(info_carpetas));
    if(old_data == 'C' || old_data == 'c'){
        pedirNombreCarpetas(carpetas, &(_carp.longtd), (_carp.total + _carp.new), _carp.exist);
        guardarCarpetas(carpetas, (_carp.total + _carp.new), _carp.exist);
        _carp.exist = _carp.total + _carp.new;
    } else
        pedirNombreCarpetas(carpetas, &(_carp.longtd), (_carp.total + _carp.new), _carp.total);
    _carp.total += _carp.new;
}*/




/*do{
    printf("\n\nQuieres modificar los correos? [ Quitar / Anyadir / No ] [ Q / A / N ]: ");
    fflush(stdin);
    scanf("%c", &modif_corr);
    switch (modif_corr)
    {
        case 'Q':   //4.A En caso de querer quitar correos
        case 'q':
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            mantener_corr = obtenerCorrRstnts(carpetas, correos, _corr.total);
            if(mantener_corr > 0){
                correos = eliminarCorreos(correos, &(_corr.total), mantener_corr);
                if(old_data == 'C' || old_data == 'c'){
                    remove("correos.txt");
                    guardarCorreos(correos, _corr.total, INICIO, INICIO);
                    _corr.exist = _corr.total;
                }
            }
            break;

        case 'A':   //4.B En caso de querer anyadir correos
        case 'a':
            pedirNumeroCC(&(_corr.new), "anyadir de correos", INICIO);
            if(_corr.new > 0){ //.A En caso de que haya correos que usar
                correos = (info_correos *) realloc(correos, (_corr.total + _corr.new) * sizeof(info_correos));
                if(old_data == 'C' || old_data == 'c'){
                    pedirNombreCorreos(correos, &(_corr.longtd), (_corr.total + _corr.new), _corr.exist);
                    meterCorreosACarpetas(carpetas, correos, _carp.total, (_corr.total + _corr.new), _corr.exist);
                    guardarCorreos(correos, (_corr.total + _corr.new), _corr.exist, INICIO);
                    _corr.exist = _corr.total + _corr.new;
                } else {
                    pedirNombreCorreos(correos, &(_corr.longtd), (_corr.total + _corr.new), _corr.total);
                    meterCorreosACarpetas(carpetas, correos, _carp.total, (_corr.total + _corr.new), _corr.total);
                }
                _corr.total += _corr.new;
            }
            break;

        case 'N':   //4.C En caso de no querer cambiar nada
        case 'n':
            break;

        default:    //4.D En caso de pulsar una tecla equivocada
            errorComandoDesconocido();
    }

}while (modif_corr != 'Q' && modif_corr != 'q' && modif_corr != 'A' && modif_corr != 'a' && modif_corr != 'N' && modif_corr != 'n');*/




/*void reajustarDatos(texto *user, info_carpetas *carp, info_correos *corr, datos *_carp, datos *_corr, char old_data, int *simples, int *dobles)
{
    char recargar = 'N';
    char repetir = 'N';
    char modif_carp = 'N';
    int mantener_carp = 0;
    int mantener_corr = 0;
    
    
    do{ //Bucle que nos permite reimprimir, o no, una tabla
      tipoLinea(user, carp, corr, simples, dobles, _carp->total, _carp->longtd, _corr->longtd, _carp->exist);

        do{//Bucle para introducir un caracter acertado
            printf("\n\nQuieres imprimir una nueva tabla? [ S / N ]: ");
            fflush(stdin);  scanf("%c", &repetir);
            if(repetir != 'S' && repetir != 's' && repetir != 'N' && repetir != 'n')
                //2. Se activa en caso de NO introducir un caracter para continuar o terminar el programa
                errorComandoDesconocido();
            if(repetir == 'S' || repetir == 's'){
                
                if((old_data == 'N' || old_data == 'n') && _carp->exist > 0){
                    do{
                        printf("\n\nQuieres cargar tus datos antiguos [ S / N ]: ");
                        fflush(stdin);  scanf("%c", &recargar);
                        if(recargar == 'S' || recargar == 's')
                            old_data = 'C';
                        
                        if(recargar == 'S' || recargar == 's'){
                            guardarCarpetas(carp, _carp->total, INICIO);
                            guardarCorreos(corr, _corr->total, INICIO, _carp->exist);
                            liberar(carp, _carp->total, corr, _corr->total);
                            _carp->exist   += _carp->total;
                            _corr->exist   += _corr->total;
                            carp            = cargarDatosCarpetas(carp, _carp->exist, &(_carp->longtd));
                            _carp->total    = _carp->exist;
                            corr            = cargarDatosCorreos(carp, corr, _corr->exist, &(_corr->longtd));
                            _corr->total    = _corr->exist;
                        }
                        else if(recargar != 'S' && recargar != 's' && recargar != 'N' && recargar != 'n')
                            errorComandoDesconocido();
                    }while(recargar != 'S' && recargar != 's' && recargar != 'N' && recargar != 'n');
                }
                
                do{
                    printf("\n\nQuieres modificar las carpetas? [ Quitar / Anyadir / No ] [ Q / A / N ]: ");
                    fflush(stdin);  scanf("%c", &modif_carp);
                    switch (modif_carp)
                    {
                        case 'Q':   //3.A En caso de querer quitar carpetas
                        case 'q':
                            mantener_carp = obtenerCarpRstnts(carp, _carp->total);
                            if(mantener_carp > 0){
                                carp = eliminarCarpetas(carp, corr, &(_carp->total), _corr->total, mantener_carp, &mantener_corr);
                                corr  = eliminarCorreos(corr, &(_corr->total), mantener_corr);
                                if(old_data == 'C' || old_data == 'c')
                                    menosCarpetas(carp, corr, _carp, _corr);
                            }
                            break;

                        case 'A':   //3.B En caso de querer anyadir carpetas
                        case 'a':
                            carp = masCarpetas(carp, _carp, old_data);
                            break;

                        case 'N':   //3.C En caso de no querer cambiar nada
                        case 'n':
                            break;

                        default:    //3.D En caso de pulsar una tecla equivocada
                            errorComandoDesconocido();
                    }

                }while (modif_carp != 'Q' && modif_carp != 'q' && modif_carp != 'A'
                        && modif_carp != 'a' && modif_carp != 'N' && modif_carp != 'n');

                corr = modifCorreos(carp, corr, _carp, _corr, old_data);
            }
        }while (repetir != 'S' && repetir != 's' && repetir != 'N' && repetir != 'n');

    }while(repetir == 'S' || repetir == 's');
    
    if(old_data != 'C' && old_data != 'c')
        //En caso de que no se haya elegido recargar los datos antiguos, para guardalos en ficheros
        finalGuardarDatos(carp, corr, _carp->total, _corr->total, _carp->exist, _corr->exist, old_data);
    free(user->nombre);
    liberar(carp, _carp->total, corr, _corr->total);
}*/
//reajustarDatos(&usuario, carpetas, correos, &_carp, &_corr, &old_data, simbolos_simples, simbolos_dobles);