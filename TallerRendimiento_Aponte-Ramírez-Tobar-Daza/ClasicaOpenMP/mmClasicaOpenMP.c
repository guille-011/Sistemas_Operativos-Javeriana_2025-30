/*#######################################################################################
 #* Fecha: 29 Oct 2025 
 #* Autor: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
 #* Programa: 
 #* 	 Multiplicación de Matrices algoritmo clásico usando OpenMP
 #* Versión:
 #*	 Paralelismo con OpenMP - Programa principal
######################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "moduloClasicoOpenMP.h"

int main(int argc, char *argv[]){ // Programa principal
	if(argc < 3){
		printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n"); // TOma como argumentos el tamaño de la matriz y el numero de hilos
		exit(0);
	}

	int N = atoi(argv[1]); // Toma el primer argumento como el tamaño de la matriz
	int TH = atoi(argv[2]); // Toma el segundo argumento como el numero de hilos a utilizar para la paralelización
	double *matrixA  = (double *)calloc(N*N, sizeof(double));
	double *matrixB  = (double *)calloc(N*N, sizeof(double)); // Aloca memoria para las matrices
	double *matrixC  = (double *)calloc(N*N, sizeof(double));

	/*
	Semilla para numeros aleatorios (utiliza el tiempo actual para asegurar que cada 
	ejecucion del programa genere numeros random diferentes) 
	*/ 	
	srand(time(NULL)); 

	omp_set_num_threads(TH); // Configura el numero de hilos para OpenMP

	iniMatrix(matrixA, matrixB, N); // Inicializa las matrices A y B con numeros aleatorios

	impMatrix(matrixA, N);
	impMatrix(matrixB, N); // Imprime las matrices A y B si son menores a 9x9

	InicioMuestra(); // Inicia la medicion de tiempo
	multiMatrix(matrixA, matrixB, matrixC, N); // Realiza la multiplicacion de matrices
	FinMuestra(); // Finaliza la medicion de tiempo

	impMatrix(matrixC, N); // Imprime la matriz resultado si es menor a 9x9

	/*Liberación de Memoria*/
	free(matrixA);
	free(matrixB);
	free(matrixC);
	
	return 0;
}
