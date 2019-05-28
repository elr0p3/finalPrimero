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
