#ifndef TABLA_H
#define TABLA_H


#define INTRODUCIR 50   //Numero que indica la longitud de unos arrays, para introducir texto
#define LONG_DER 3      //Numero constante que determina la longitud de las celdas que indican si el correo esta leido


typedef struct cadena{
  char *nombre;     //Nos permite almacenar en memoria dinamica los nombres de las carpetas y correos
  int longitud;     //Nos permite registrar las longitudes de los nombres
}texto;

typedef struct carpeta{
  texto contenido;      //Llamada a estructura para guardar los nombres y longitudes de las carpetas
  int numero_correos;   //Guarda cuantos correos hay en la carpeta correspondiente
  int tipo;             //Indica la posicion de la carpeta
  texto archivo;
  char eliminar;
}info_carpetas;

typedef struct correo{
  texto contenido;      //Llamada a estructura para guardar los nombres y longitudes de los correos
  char leido;           //Marca si el correo esta leido o no
  int tipo_carpeta;     //Indica en que carpeta se encentra el correo
}info_correos;


/*Parte encargada de una cierta estetica*/
void imprimirEncabezadoInicial(int *, texto *);
void carpetasExistentes(info_carpetas *, int *);


/*Parte dedicada a obtener la informacion de las carpetas, correos y usuario; con uso de la memoria dinamica*/
void pedirNombreUsuario(texto *);
void pedirNumeroCC(int *, char *, int);
void pedirNombreCarpetas(info_carpetas *, int *, int);
void pedirNombreCorreos(info_correos *, int *, int);
void meterCorreosACarpetas(info_carpetas *, info_correos *, int, int);
void liberar(texto *, info_carpetas *, int, info_correos *, int);

/*Parte dedicada a guardar las carpetas y los correos en archivos*/
void guardarCarpetas(info_carpetas *, int);
void guardarCorreos(info_carpetas *, info_correos *, int, int);
info_carpetas * eliminarCarpetas(info_carpetas *, int *);


/*Parte dedicada a imprimir por pantalla la tabla con la informacion obtenda de las funciones anteriores*/
void imprimirLineas(int, int, int, int, int, int, int, int, int, int);
void textoEncabezado(int, int, int, char *);
void textoCarpetas(int, int, int, int, int, char *);
void textoCorreos(int, int, int, int *, info_correos *);
void imprimirEncabezado(int, int, int, int, int, int *, int, int, texto *, info_carpetas *);
void imprimirCeldasCC(int, int, int, int, int, int *, int, int, info_carpetas *, info_correos *);
void imprimirCarpetasRestantes(int, int, int *, int, int, info_carpetas *);
void imprimirCorreosRestantes(int, int, int, int , int *, int, int, info_correos *);

#endif /* TABLA_H */
