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

/*Se declaran las librerias necesarias*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "modulo.h"
/*Se declaran los apuntadores mA, mB y mC como variables globales*/
double *mA, *mB, *mC;

int main(int argc, char *argv[]) {
    if(argc<2){ //verifica que se haya pasado la cantidad correcta de argumentos como parametro
        printf("Numero argumentos incorrectos\n");
        printf("\n\t $ejecutable.exe DIM \n");
        return -1;
    }

    int N = (int) atof(argv[1]); //Convierte a entero los valores enviados como parametro
    if(N<=0){
        printf("\n Valores deben ser mayor que cero\n"); //verifica que los parametros no sean 0
        return -1;
    };

    mA = (double *) malloc(N*N*sizeof(double)); //Reserva la cantidad pasada por parametro como memoria para una matriz
    mB = (double *) malloc(N*N*sizeof(double)); //Reserva la cantidad pasada por parametro como memoria para una matriz
    mC = (double *) calloc(N*N,sizeof(double)); //Reserva la cantidad pasada por parametro como memoria para una matriz y la inicializa en 0

	/** Se inicializan las 2 matrices no inicializadas anteriormente**/
	iniMatriz(N, mA, mB);

	imprMatrices(N, mA); //se imprime la matriz A con la funcion imprMatrices
	imprMatrices(N, mB); //Se imprime la matriz B con la funcion imprMatrices
	InicioMuestra(); //Se toma la hora antes de la multiplicaicon
	multiMatrizClasica(mA, mB, mC, N); //Se realiza la multiplicacion de las matrices
	FinMuestra(); //Se toma la hora después de la multiplicacion
	imprMatrices(N, mC); //se impime la matriz c

        printf("\n\tFin del programa.............!\n");
       	/*Se libera la memoria reservada para las matrices*/
	    free(mA);
        free(mB);
        free(mC);
        return 0;

}
