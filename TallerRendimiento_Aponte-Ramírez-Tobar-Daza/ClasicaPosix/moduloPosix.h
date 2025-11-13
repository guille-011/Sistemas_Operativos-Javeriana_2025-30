/*#######################################################################################
#* Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD 
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Hilos Pthreads "Posix" 
######################################################################################*/

/*Se verifica si el identificador __MODULOPOSIX_H__ ha sido definido antes.
 Sirve para evitar que el contenido de este archivo se incluya varias veces en un mismo programa.
 Si ya estuviera definido, el compilador se saltaría todo el contenido hasta el #endif.*/
#ifndef __MODULOPOSIX_H__

 /*Se define el identificador __MODULOPOSIX_H__.
De tal forma se marca marca que el archivo ya fue incluido una vez, evitando duplicaciones posteriores.
En este espacio se colocan las declaraciones, estructuras, constantes o prototipos de funciones
que se necesitan compartir entre varios archivos .c del proyecto.*/
#define __MODULOPOSIX_H__

#include <stdio.h> //librería para manejar entradas y salidas
#include <pthread.h> //librería para manejar hilos
#include <stdlib.h> //librería para manejar funciones como free() o rand() y memoria dinámica
#include <sys/time.h> //librería para manejar funciones como gettimeofday()

//Se "declaran" variables globales
//Se usa extern para indicar que son variables globales que existen en los archivos que incluyan este modulo.h
extern double *matrixA, *matrixB, *matrixC; //La matrizA es la primera a multiplicar, matrizB es la segunda y en matrizC se guarda el resultado final
extern struct timeval inicio, fin;  //estructuras de tipo timeval, sirven para guardar el inicio y el final de la ejecución de una multiplicación

//Estructura "parámetros"
//Sirve para indicarle a los hilos que parte del trabajo deberán realizar en una matriz
struct parametros{ 
	int nH; //numero total de hilos
	int idH;//id del hilo actual
	int N; //tamaño de las matrices
};
void InicioMuestra();//Funcion para tomar el momento de inicio
void FinMuestra();//Funcion para tomar el momento de final
void iniMatrix(double *m1, double *m2, int D); //Funcion para inicializar las matrices A y B
void impMatrix(double *matriz, int D);//Función para imprimir la matriz por pantalla
void *multiMatrix(void *variables);//Función que ejecutan los hilos, a cada uno le es asignado un espacio de las matrices para realizar las operaciones


#endif