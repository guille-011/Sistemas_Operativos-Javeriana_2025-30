/*#######################################################################################
 #* Fecha: 29 Oct 2025
 #* Autor: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
 #* Programa: 
 #* 	 Multiplicación de Matrices algoritmo clásico usando OpenMP
 #* Versión:
 #*	 Paralelismo con OpenMP - Biblioteca de funciones
######################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include "moduloClasicoOpenMP.h"

struct timeval inicio, fin; // Variables para medicion de tiempo

void InicioMuestra(){
	gettimeofday(&inicio, (void *)0); // Funcion que inicia el temporizador 
}

void FinMuestra(){
	gettimeofday(&fin, (void *)0); // Funcion que detiene el temporizador
	fin.tv_usec -= inicio.tv_usec; // tiempo en microsegundos
	fin.tv_sec  -= inicio.tv_sec; // tiempo en segundos
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); // Calcula el tiempo total
	printf("%9.0f \n", tiempo); // Imprime el tiempo en microsegundos, con 9 caracteres de espaciado, y redondeado. 
}

void impMatrix(double *matrix, int D){ // imprime la matriz si es menor a 9x9
	if(D < 9){
		printf("\n");
		for(int i=0; i<D*D; i++){
			if(i%D==0) printf("\n");	// Salto de linea por fila
			printf("%.2f ", matrix[i]); // Imprime el valor con 2 decimales
		}
		printf("\n**-----------------------------**\n");
	}
}

void iniMatrix(double *m1, double *m2, int D){ // inicializa las matrices con numeros aleatorios
	for(int i=0; i<D*D; i++){ 
		m1[i] = (double)rand()/RAND_MAX*(5.0-1.0) + 1.0;	// Se asume que los numeros que se buscan estan entre 1.0 y 5.0
		m2[i] = (double)rand()/RAND_MAX*(9.0-2.0) + 2.0;	// Se asume que los numeros que se buscan estan entre 2.0 y 9.0
	}
}

void multiMatrix(double *mA, double *mB, double *mC, int D){ // Algoritmo clasico de multiplicaion de matrices usando OpenMP
	double Suma, *pA, *pB; // Variables auxiliares para el calculo
	#pragma omp parallel for private(pA, pB, Suma)
	for(int i=0; i<D; i++){ // Recorre las filas de la matriz A
		for(int j=0; j<D; j++){ // Recorre las columnas de la matriz B
			pA = mA+i*D; // Apunta al inicio de la fila i de la matriz A
			pB = mB+j; // Apunta al inicio de la columna j de la matriz B
			Suma = 0.0; // Inicializa la suma para el elemento C[i][j]
			for(int k=0; k<D; k++, pA++, pB+=D){ // Recorre los elementos de la fila i y columna j
				Suma += *pA * *pB; // Realiza la multiplicacion y suma acumulada
			}
			mC[i*D+j] = Suma; // Asigna el valor calculado a la matriz C
		}
	}
}
