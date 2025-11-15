/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: paralelismo con POSIX
 * Archivo: moduloConcurrenciaPosix.h
 * Descripcion: Define la estructura de parámetros que usan los hilos
 *              para procesar segmentos de un vector de enteros y 
 *              declara las funciones que permiten, mediante hilos 
 *              POSIX, buscar el máximo valor de un arreglo de forma 
 *              concurrente dividiendo el trabajo entre varios hilos.
 * Fecha: 14/11/25
 ******************************************************************/



 /*Se verifica si el identificador __MODULOCONCURRENCIA_H__ ha sido definido antes.
 Sirve para evitar que el contenido de este archivo se incluya varias veces en un mismo programa.
 Si ya estuviera definido, el compilador se saltaría todo el contenido hasta el #endif.*/
#ifndef __MODULOCONCURRENCIA_H__

 /*Se define el identificador __MODULOCONCURRENCIA_H__.
De tal fomra se marca marca que el archivo ya fue incluido una vez, evitando duplicaciones posteriores.
En este espacio se colocan las declaraciones, estructuras, constantes o prototipos de funciones
que se necesitan compartir entre varios archivos .c del proyecto.*/
#define __MODULOCONCURRENCIA_H__




#include <stdio.h>      // printf(), fprintf()
#include <stdlib.h>     // exit()
#include <pthread.h>    // pthread_create(), pthread_join(), mutex, cond

/*Estructura de tipo argHilos

con variables inicio
fin
*vector
maxparcial

*/
struct argHilos{
        int inicio;
        int fin;
        int *vector;
        int maxparcial;
};

typedef struct argHilos param_H; //Se define un alias param_h para la estructura anterior


/*Función que guarda el valor máximo encontrado en el vector de la estructura argumentos de tipo param_H*/
void *buscarMax(void *parametro);
/*Fucion que calcunda el elemento máximo en una lista de enteros usando paralelismo*/
int maximoValor(int *vec, int n, int nhilos);

#endif