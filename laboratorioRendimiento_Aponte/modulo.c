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


/*Se incluyen la librerias necesarias para */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "modulo.h"


struct timeval ini, fin;

/*Funcion para tomar tiempo inicial*/
void InicioMuestra(){
	gettimeofday(&ini, (void *)0); //Obtiene la hora del dia y la guarda en fin
}

/*Funcion para tomar el tiempo final y presenta el resultado en ms*/
void FinMuestra(){
	gettimeofday(&fin, (void *)0); //obtiene la hora del dia y la guarda en fin
	fin.tv_usec -= ini.tv_usec; //resta la hora final con la inicial
	fin.tv_sec -= ini.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); //Guarda el tiempo total
	printf("%9.0f \n", tiempo); //Imprime el tiempo total
}

/*Funcion que inicializa dos matrices pasadas como parametro dado su tamaño*/
void iniMatriz(int n, double *m1, double *m2){
   	for(int i=0; i<n*n; i++){ //Recorre las matrices
	        m1[i] = i*2.3 + 1.3; //llena la matriz m1
	        m2[i] = i*2.3; //llena la matriz m2
	}
};

/*Funcion que imprime una matriz dado su tamaño*/
void imprMatrices(int n, double *matriz){
    if(n<9){ //Si el tamaño es menor a 9 realiza la impresion
        printf("\n#######################################################\n");
   	for(int i=0; i<n*n; i++){ //Recorre las matrices
                if(i%n==0) printf("\n"); //Imprime un salto de linea luego de la ultima columna
	        printf(" %f ", matriz[i]); //Imprime las matrices
   }
    } else {
        printf("\n#######################################################\n");
    }
};

/*Funcion que multiplica dos matrices pasadas como parametro y guarda el resultado*/
/*en una tercera también pasada, dado un tamaño n*/
void multiMatrizClasica(double *m1, double *m2, double *m3, int N){
    for(int i=0; i<N; i++){ //Ciclo hasta N
        for(int j=0; j<N; j++){ //Ciclo hasta N
            double sumaTemp, *pA, *pB; //Se declara una variable sumaTemp y dos apuntadores
            sumaTemp = 0.0; //Se inicializa sumaTemp
            pA = m1 + i*N; //Se guarda en pA la direccion de la matriz sumada al iterador por N
            pB = m2 + j; //Se guarda en pB la direccion de la matriz sumada al iterador j
            for(int k=0; k<N; k++, pA++, pB+=N){ //se hace un ciclo hasta N, sumandole al iterador k, a pA y a pB
                sumaTemp += *pA * *pB; //Se va sumando en sumaTemp la multiplicacion del contenido de pA y pB
            }
            m3[j+i*N] = sumaTemp; //Se va guardando en cada posicion de m3 el resultado de sumaTemp
        }
    }
}
