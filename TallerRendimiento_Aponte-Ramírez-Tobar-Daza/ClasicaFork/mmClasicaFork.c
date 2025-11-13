/*#######################################################################################
#* Pontificia Universidad Javeriana
#* Facultad de Ingeniería
#* Carrera: Ingeniería de Sistemas
#* Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
#* Materia: Sistemas Operativos
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico usando Fork()
#* Descripción:
#*       En este programa se divide el cálculo de la multiplicación de matrices entre
#      varios procesos creados mediante fork(), asignándole a cada proceso un bloque
#      de filas de la matriz resultado. El objetivo es medir velocidad y practicar
#      paralelismo a nivel de procesos.
######################################################################################*/

#include <stdio.h>		   // Biblioteca para entrada y salida estándar
#include <stdlib.h>		   // Funciones de memoria dinámica y utilidades
#include <unistd.h>		   // Para fork(), getpid() y funciones de UNIX
#include <sys/wait.h>	   // Para wait() y manejo de procesos hijos
#include <sys/time.h>	   // Para medir tiempos (InicioMuestra/FinMuestra)
#include <time.h>		   // Para srand() y time()
#include "moduloClasicoFork.h" // Archivo con funciones auxiliares usadas

int main(int argc, char *argv[])
{

	// Verifica que el usuario pase al menos dos argumentos: tamaño y procesos
	if (argc < 3)
	{
		printf("\n \t\tUse: $./nom_ejecutable Size Hilos \n");
		exit(0); // Sale del programa si faltan parámetros
	}

	// Convierte el tamaño de la matriz recibido por consola (N)
	int N = (int)atoi(argv[1]);

	// Convierte la cantidad de procesos que se desea crear (num_P)
	int num_P = (int)atoi(argv[2]);

	// Reserva memoria dinámica para las matrices A, B y C (todas NxN)
	double *matA = (double *)calloc(N * N, sizeof(double));
	double *matB = (double *)calloc(N * N, sizeof(double));
	double *matC = (double *)calloc(N * N, sizeof(double));

	// Verifica que la memoria haya sido asignada correctamente
	if (!matA || !matB || !matC)
	{
		perror("calloc"); // Muestra error del sistema
		free(matA);		  // Libera memoria asignada parcialmente
		free(matB);
		free(matC);
		return 1; // Sale con error
	}

	// Inicializa la semilla para generar números aleatorios
	srand(time(NULL));

	// Inicializa las matrices A y B con valores aleatorios
	iniMatrix(matA, matB, N);

	// Imprime la matriz A
	impMatrix(matA, N);

	// Imprime la matriz B
	impMatrix(matB, N);

	/*
		Limpia todos los buffers de salida antes de usar fork.
		Esto evita que se dupliquen impresiones en los procesos hijos.
	*/
	fflush(NULL);

	// Calcula cuántas filas de la matriz C va a procesar cada proceso
	int rows_per_process = N / num_P;

	// Inicia la medición del tiempo
	InicioMuestra();

	// Crea tantos procesos como indicó el usuario
	for (int i = 0; i < num_P; i++)
	{

		pid_t pid = fork(); // Crea un proceso hijo

		// Si pid == 0 significa que está dentro del proceso hijo
		if (pid == 0)
		{

			// Calcula la fila inicial del bloque asignado al hijo
			int start_row = i * rows_per_process;

			// Calcula la fila final; el último proceso toma las filas restantes
			int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;

			// El hijo realiza la multiplicación de su bloque de filas
			multiMatrix(matA, matB, matC, N, start_row, end_row);

			// Si N es menor que 9, imprime las filas calculadas por este proceso
			if (N < 9)
			{
				printf("\nChild PID %d calculated rows %d to %d:\n", 
					   getpid(), start_row, end_row - 1); //imprime PID y rango

				// Imprime fila por fila del rango asignado
				for (int r = start_row; r < end_row; r++)
				{
					for (int c = 0; c < N; c++) // Recorre columnas
					{
						printf(" %.2f ", matC[N * r + c]); // Imprime elemento
					}
					printf("\n");
				}
			}

			exit(0); // El proceso hijo termina aquí
		}
		else if (pid < 0)
		{ // Si pid < 0 significa que fork falló

			perror("fork failed"); // Muestra error del sistema
			exit(1);			   // Finaliza el programa
		}
	}

	// El proceso padre espera a que todos los procesos hijos terminen
	for (int i = 0; i < num_P; i++)
	{
		wait(NULL);
	}

	// Detiene medición de tiempo una vez que todos los hijos acabaron
	FinMuestra();

	// Libera memoria asignada dinámicamente
	free(matA);
	free(matB);
	free(matC);

	return 0; // Fin del programa
}
