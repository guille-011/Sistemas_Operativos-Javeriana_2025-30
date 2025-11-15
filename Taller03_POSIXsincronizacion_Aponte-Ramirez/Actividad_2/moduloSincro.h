/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: MUTEXES y COND VARIABLES con POSIX
 * Archivo: moduloSincro.h
 * Descripcion: Declara las variables globales compartidas y los
 *              prototipos de las funciones del productor y el 
 *              spooler que utilizan mutexes y variables de 
 *              condición POSIX para sincronizar el acceso a un 
 *              búfer de mensajes en memoria
 * 
 * Fecha: 14/11/25
 ******************************************************************/

/*Se verifica si el identificador __MODULOSINCRO_H__ ha sido definido antes.
 Sirve para evitar que el contenido de este archivo se incluya varias veces en un mismo programa.
 Si ya estuviera definido, el compilador se saltaría todo el contenido hasta el #endif.*/
#ifndef __MODULOSINCRO_H__

 /*Se define el identificador __MODULOSINCRO_H__.
De tal fomra se marca marca que el archivo ya fue incluido una vez, evitando duplicaciones posteriores.
En este espacio se colocan las declaraciones, estructuras, constantes o prototipos de funciones
que se necesitan compartir entre varios archivos .c del proyecto.*/
#define __MODULOSINCRO_H__



#include <stdio.h>      // printf(), fprintf()
#include <stdlib.h>     // exit()
#include <string.h>     // strerror()
#include <pthread.h>    // pthread_create(), pthread_join(), mutex, cond
#include <unistd.h>     // sleep()

#define MAX_BUFFERS 100 //Se inicializa una constante que define el tamaño máximo del buffer


/*Se definen las variables globales que se usarán en dos archivos .c, usando la claúsula extern
para indicarle al compilador que estas solo serán usadas una vez, pues si no se indica esto se producirían 
errores de compilación al duplicarse las variables globales*/
extern int buffer_index;
extern int buffer_print_index;
extern int lines_to_print;
/*Función que escribe una serie de mensajes sobre el buffer compartido*/
void *producer (void *arg);
void *spooler (void *arg);


#endif