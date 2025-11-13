/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
 * Materia: Sistemas Operativos
 * Tema: Implementación de multiplicación de matrices con OpenMP
 * Archivo: mmFilasOpenMP.c
 *
 * Fecha: 05/11/2025
 *
 * Descripción general:
 *   Programa principal que realiza la multiplicación de dos matrices
 *   cuadradas utilizando paralelismo con OpenMP.
 *   El usuario define el tamaño de las matrices y la cantidad de hilos
 *   a usar en la ejecución.
 *
 * Flujo de ejecución resumido:
 *   1. Se leen los argumentos desde la línea de comandos:
 *        - SIZE: tamaño de las matrices NxN.
 *        - Hilos: número de hilos que utilizará OpenMP.
 *   2. Se reservan dinámicamente tres matrices (A, B, C).
 *   3. Se inicializan las matrices A y B con valores aleatorios.
 *   4. Se imprimen A y B (la segunda de forma traspuesta).
 *   5. Se mide el tiempo de ejecución de la multiplicación paralela.
 *   6. Se realiza la multiplicación de A y B mediante la función
 *      `multiMatrixTrans`, que usa OpenMP para paralelizar el cálculo.
 *   7. Se imprime la matriz resultante C y el tiempo total medido.
 *   8. Se libera la memoria reservada dinámicamente.
 *
 * Entradas (argv):
 *   argv[0] -> nombre del ejecutable
 *   argv[1] -> tamaño de las matrices (N)
 *   argv[2] -> número de hilos (TH)
 *
 * Salidas:
 *   - Matrices A, B (traspuesta) y C impresas por consola (solo si N < 6).
 *   - Tiempo total de ejecución del cálculo en microsegundos.
 *
 * Archivos relacionados:
 *   - modulo_filas_openMP.h → Cabecera con declaraciones de funciones auxiliares.
 *   - modulo_filas_openMP.c → Implementación de funciones auxiliares y paralelas.
 *******************************************************************/

#include <stdio.h>				 // Permite el uso de funciones de entrada/salida estándar,
#include <stdlib.h>				 // Funciones generales de utilidad
#include <string.h>				 // Proporciona funciones para manipular cadenas de texto
#include <time.h>				 // Libreria para manejo de tiempo
#include <sys/time.h>			 // Permite obtener tiempos de alta precisión en microsegundos
#include <omp.h>				 // Librería de OpenMP: habilita la programación paralela
#include "modulo_filas_openMP.h" // Archivo de cabecera propio donde se declaran las funciones del módulo
/*Funcion principal donde se desarrolla el programa solicitado en el taller*/
int main(int argc, char *argv[])
{
	/*Si al momento de ejecutar el programa se le envian menos de 3 argumentos se imprime un mensaje de error
	 y se cierra el programa. Los argumentos esperados son los siguientes:

	 0-El nombre del ejecutable
	 1-El tamaño de las matrices que se vana operar
	 2-La cantidad de hilos usados en la paralelización del programa*/
	if (argc < 3)
	{
		printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
		exit(0);
	}
	int N = atoi(argv[1]);	// Se declara la variable N y se le asigna la conversión a entero del segundo argumento recibido por el main
	int TH = atoi(argv[2]); // Se declara la variable TH y se le asigna la conversión a entero del tercer argumento recibido por el main
	/*Se declaran tres matrices de tipo double y se les reserva la memoria necesaria según el tamaño recibido por el main, para esto se utiliza
	 la función calloc casteada a double, que reserva memoria para las matrices y todos esos espacios reservados los inicializa en 0.0*/
	double *matrixA = (double *)calloc(N * N, sizeof(double));
	double *matrixB = (double *)calloc(N * N, sizeof(double));
	double *matrixC = (double *)calloc(N * N, sizeof(double));
	/*Se utiliza la función srand, para inicializar la semilla del generador de números aleatorios que usa la función rand(), es decir,
	el punto de partida que usará rand() para generar esos números. Como parámetro, se le envia time(NULL) a srand,
	esto guarda el número de segundos desde 1970, por lo tanto, cada vez que se ejecuta el programa, los números generados por rand()
	serán diferentes gracias a que la semilla va cambiando según la hora en que se ejecute el programa*/
	srand(time(NULL));
	/*Se utiliza la función omp_set_num_threads para definir el número de hilos que usará openMP en la siguiente sección paralela de código,
	se le envia como parámetro TH, que corresponde al número de hilos definido al ejecutar el código*/
	omp_set_num_threads(TH);
	iniMatrix(matrixA, matrixB, N);					// Se llama a inimatrix para inicializar las matricez A y B con valores aleatorios
	impMatrix(matrixA, N, 0);						// Se imprime a la matrizA de manera normal
	impMatrix(matrixB, N, 1);						// Se imprime a la matrizB de manera transpuesta, pues la transpuesta de la matriz B será tomada como su forma original
	InicioMuestra();								// Se llama a la función inicio muestra para guardar la hora en la que se inició la multiplicación de matrices
	multiMatrixTrans(matrixA, matrixB, matrixC, N); // Se llama a la función que realiza la multiplicación de las matrices
	FinMuestra();									// Se llama a la función fin muestra para guardar el tiempo total que duró el programa multiplicando las matrices

	impMatrix(matrixC, N, 0); // Se imprime la matriz c de manera normal
	/*Se utiliza la función free para liberar la memoria reservada para las 3 matrices*/
	free(matrixA);
	free(matrixB);
	free(matrixC);
	return 0;
}