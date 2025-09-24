/**************************************************************************************
 * Fecha: 11/09/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Concurrencia
 * Objetivo: Hacer una metodología para la implementación de la multiplicación de matrices.
 * La idea principal, es construir paso a paso la implementación para hacer uso de la
 * biblioteca PTHREAD. Se implementa el Algoritmo Clásico de multiplicación de matrices, para
 * matrices cuadradas, es decir, la dimensión de filas es igual a la de columnas.
 * A continuación se describen los paso
 *  - Reserva de memoria
 *      - Creación de punteros para matrices del tipo doble
 *      - Asignación de memoria
 *      - Ingreso de argumentos de entrada (Dimensión matriz, número de hilos)
 *      - Validación argumentos de entrada
 *      - Inicializar las matrices
 *      - Imprimir las matrices
 *      - Función para inicializar las matrices
 *      - Función para imprimir las matrices
 *      - Algoritmo clásico de multiplicación matrices
 *  - Se verifica el resultado
 *      - Función para multiplicación las matrices
 *      - Declaración vector de hilos
 *      - Creación de hilos según tamaño de vector de hilos
 *      - Crear estructura de datos que encapsule los argumentos de entrada de la función MM
 * Implementación de paralelismo: resolución de la multiplicación de matrices
 *  - Se crea el vector de hilos
 *  - Se tiene pendiente la exclusión de los hilos
 *  - Se pasa a globales las matrices
 *  - Encapsular los datos para enviarlos a la función MxM
 *  - Se desencapsulan los datos dentro de la función MxM (descomprimen)
*************************************************************************************/
#ifndef __MODULO_H__
#define __MODULO_H__


/*Funcion para tomar el tiempo inicial*/
void InicioMuestra();
/*Funcion para tomar el tiempo final y presenta el resultado en ms*/
void FinMuestra();
/*Funcion que inicializa dos matrices pasadas como parametro dado su tamaño*/
void iniMatriz(int n, double *m1, double *m2);
/*Funcion que imprime una matriz dado su tamaño*/
void imprMatrices(int n, double *matriz);
/*Funcion que multiplica dos matrices pasadas como parametro y guarda el resultado*/
/*en una tercera también pasada, dado un tamaño n*/
void multiMatrizClasica(double *m1, double *m2, double *m3, int n);

#endif
