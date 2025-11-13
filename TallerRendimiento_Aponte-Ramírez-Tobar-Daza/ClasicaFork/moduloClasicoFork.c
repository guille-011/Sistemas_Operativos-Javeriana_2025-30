/*#######################################################################################
#* Pontificia Universidad Javeriana
#* Facultad de Ingeniería
#* Carrera: Ingeniería de Sistemas
#* Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
#* Materia: Sistemas Operativos
#*Archivo:
#*      mmModuloClasicaFork.c
#* Descripción:
#*      Este archivo contiene las funciones encargadas de medir tiempos de ejecución,
#*      inicializar matrices, imprimirlas y realizar la multiplicación clásica de matrices.
#*      Estas funciones son utilizadas por el programa principal, el cual distribuye
#*      el cálculo entre varios procesos mediante fork(). Aquí se implementa la lógica
#*      operativa necesaria para apoyar dicho paralelismo.
######################################################################################*/

#include <stdio.h>		   // Biblioteca para entrada y salida estándar
#include <stdlib.h>		   // Funciones de memoria dinámica y utilidades
#include <unistd.h>		   // Para fork(), getpid() y funciones de UNIX
#include <sys/wait.h>	   // Para wait() y manejo de procesos hijos
#include <sys/time.h>	   // Para medir tiempos (InicioMuestra/FinMuestra)
#include <time.h>		   // Para srand() y time()
#include "moduloClasicoFork.h" // Archivo con funciones auxiliares usadas

struct timeval inicio, fin;// Estructura para catalogar el inicio y el fin de la ejecución del programa 

void InicioMuestra(){ 		// Función para iniciar la medición del tiempo
	gettimeofday(&inicio, (void *)0);// Obtener el tiempo de inicio del programa
}

void FinMuestra(){ // Función para finalizar la medición del tiempo y mostrar el resultado
	gettimeofday(&fin, (void *)0); // Obtener el tiempo de finalización del programa
	fin.tv_usec -= inicio.tv_usec; // Calcular la diferencia en microsegundos
	fin.tv_sec  -= inicio.tv_sec; // Calcular la diferencia en segundos
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); // Convertir el tiempo total a microsegundos
	printf("%9.0f \n", tiempo);// Mostrar el tiempo total en microsegundos
}

void multiMatrix(double *mA, double *mB, double *mC, int D, int filaI, int filaF) { // Función para multiplicar dos matrices
	double Suma, *pA, *pB; // Variables auxiliares para la multiplicación
    for (int i = filaI; i < filaF; i++) {// Recorrer las filas asignadas a este proceso
        for (int j = 0; j < D; j++) { // Recorrer las columnas de la matriz B
			Suma = 0.0; // Inicializar la suma para el elemento C[i][j]
			pA = mA+i*D; // Puntero al inicio de la fila i de la matriz A
			pB = mB+j; // Puntero al inicio de la columna j de la matriz B
		for (int k = 0; k < D; k++, pA++, pB+=D) { // Recorrer los elementos de la fila i y columna j
				Suma += *pA * *pB;	 // Realizar la multiplicación y acumulación
            }
			mC[i*D+j] = Suma; // Almacenar el resultado en la matriz C
        }
    }
}

void impMatrix(const double *matrix, int D) { // Función para imprimir una matriz
	if (D < 9) { // Imprimir solo si la dimensión es menor que 9
    	printf("\nImpresión	...\n"); // Mensaje de inicio de impresión
    	for (int i = 0; i < D*D; i++, matrix++) { // Recorrer todos los elementos de la matriz
			if(i%D==0) printf("\n"); // Nueva línea al inicio de cada fila
            	printf(" %.2f ", *matrix);// Imprimir el elemento con dos decimales
        	}
        printf("\n "); // Nueva línea al final de la matriz
    }
}

void iniMatrix(double *mA, double *mB, int D){ // Función para inicializar dos matrices con valores aleatorios
	for (int i = 0; i < D*D; i++, mA++, mB++){ // Recorrer todos los elementos de ambas matrices
            *mA = (double)rand()/RAND_MAX*(5.0-1.0); // Asignar un valor aleatorio entre 1.0 y 5.0 a la matriz A
            *mB = (double)rand()/RAND_MAX*(9.0-5.0);  // Asignar un valor aleatorio entre 5.0 y 9.0 a la matriz B	
        }
}