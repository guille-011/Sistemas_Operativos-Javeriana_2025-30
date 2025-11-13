/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
 * Materia: Sistemas Operativos
 * Tema: Implementación de multiplicación de matrices con OpenMP
 * Archivo: modulo_filas_openMP.c
 *
 * Fecha: 05/11/2025
 *
 * Descripción general:
 *   Implementación de las funciones auxiliares y paralelas utilizadas
 *   por el programa principal (mmFilasOpenMP.c) para realizar la 
 *   multiplicación de matrices usando OpenMP.
 *
 * Funciones implementadas:
 *   - InicioMuestra(): Registra el instante de inicio de ejecución.
 *   - FinMuestra(): Calcula y muestra el tiempo total de ejecución.
 *   - impMatrix(): Imprime matrices de forma normal o traspuesta.
 *   - iniMatrix(): Inicializa dos matrices con valores aleatorios.
 *   - multiMatrixTrans(): Multiplica dos matrices usando paralelismo
 *     con OpenMP y guarda el resultado en una tercera matriz.
 *
 * Archivos relacionados:
 *   - modulo_filas_openMP.h → Cabecera que declara las funciones.
 *   - mmFilasOpenMP.c → Programa principal que utiliza este módulo.
 *******************************************************************/



#include <stdio.h>          // Permite el uso de funciones de entrada/salida estándar,                  
#include <stdlib.h>         // Funciones generales de utilidad
#include <string.h>         // Proporciona funciones para manipular cadenas de texto 
#include <time.h>           // Libreria para manejo de tiempo 
#include <sys/time.h>       // Permite obtener tiempos de alta precisión en microsegundos 
#include <omp.h>            // Librería de OpenMP: habilita la programación paralela 
#include "modulo_filas_openMP.h" // Archivo de cabecera propio donde se declaran las funciones del módulo 



/*Se declaran dos variables de tipo struct timeval, estructura definida en sys/time.h. 
Estas variables se usan para guardar una marca de tiempo precisa, con resolución de segundos
y microsegundos.

Se tiene una variable inicio, para cuando comienza la muestra y una variable fin para cuando
termina*/
struct timeval inicio, fin; 

/*Funcion inicioMuestra que guarda en la variable inicio, la hora en que se llamó a la función*/
void InicioMuestra(){
	/*Se llama a la función gettimeofday, que recibe como parámetro una variable de tipo struct timeval,
	en ester caso se le envia la dirección de memoria de la variable inicio, declarada anteriormente,
	como segundo parámetro se le envia información de zona horaria, sin embargo, al no ser importante
	en este caso, se envia (void*)0, para indicar que no se envia nada en este caso*/
	gettimeofday(&inicio, (void *)0);
}

/*Funcion finMuestra que calcula el tiempo total de ejecución en microsegundos de un programa*/
void FinMuestra(){
	/*Se llama a la función gettimeofday, que recibe como parámetro una variable de tipo struct timeval,
	en ester caso se le envia la dirección de memoria de la variable fin, declarada anteriormente,
	como segundo parámetro se le envia información de zona horaria, sin embargo, al no ser importante
	en este caso, se envia (void*)0, para indicar que no se envia nada en este caso*/
	gettimeofday(&fin, (void *)0);

	/*Se calcula el tiempo total transcurrido desde que inició la muestra hasta que finalizó,
	para esto se resta la hora a la que se llamó la función InicioMuestra, contenido en la variable inicio
	de la hora a la que se llamó a la función FinMuestra, contenida en la variable fin*/
	fin.tv_usec -= inicio.tv_usec; //Se restan los los microsegundos iniciales de los finales
	fin.tv_sec  -= inicio.tv_sec; //Se restan los segundos iniciales de los finales

	/*Se convierten los segundos y microsegundos a segundos totales, multiplicando los segundos por 
	1000000 para pasarlos a microsegundos y sumándole los microsegundos anteriormente obtenidos
	luego, este valor se castea a double y se guarda en la variable tiempo*/
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); 
	printf("%9.0f \n", tiempo); //Se imprime la variable tiempo como números flotantes de caracteres y con ancho de 9 caracteres
}

/*Funcion que imprime una matriz enviada como parámetro, dado su tamaño y la condición de si se debe
imprimir traspuesta o no, según el valor dado en la variable t

0= normal
1= traspuesta*/
void impMatrix(double *matrix, int D, int t){
	int aux = 0; //Se declara una variable auxiliar
	if(D < 6) //Solo se imprime la matriz si su tamaño es menor a 6
		switch(t){
			/*Caso para cuando se quiere imprimir la matriz de manera normal*/
			case 0:
				for(int i=0; i<D*D; i++){ //Cilo hasta el tanaño de la matriz
					if(i%D==0) printf("\n"); //Se imprime un salto de linea si se llega al un borde de la matriz
						printf("%.2f ", matrix[i]); //Se imprime la posición iterada de la matriz, indicando que contiene un float o double y que solo deben ser dos decimales
				}
				printf("\n  - \n"); //Se imprimen dos saltos de linea con un guión entre ellos
				break;
			/*Caso para cuando se quiere imprimir la matriz de manera traspuesta, es decir
			recorriendola por columnas en vez de por filas*/	
			case 1:
				while(aux<D){ //Ciclo mientras que aux sea menor que el tamaño de la matriz
					for(int i=aux; i<D*D; i+=D) //Ciclo desde la posición de aux, hasta el tamaño de la matriz, saltando según cuando mide su lado
						printf("%.2f ", matrix[i]); //Se imprime la posición iterada de la matriz, indicando que contiene un float o double y que solo deben ser dos decimales
					aux++; //Se aumenta aux
					printf("\n"); //Se imprime un salto de línea
				}	
				printf("\n  - \n"); //Se imprimen dos saltos de linea con un guión entre ellos
				break;
			default:
				printf("Sin tipo de impresión\n"); //Si no se recibe un 1 o un 0 se imprime un error de impresión
		}
}

/*Función iniMatrix que inicializa dos matrices con valores aleatorios*/
void iniMatrix(double *m1, double *m2, int D){
	for(int i=0; i<D*D; i++, m1++, m2++){ //Ciclo que recorre las matrices desde su inicio hasta su tamaño

		/*Inicialización de las matrices. A cada elemento de la matriz se le asigna un casteo de double
		de la función rand(), de tal forma la matriz se asegura de guardar valores decimales,
		la función rand retorna valores entre 0 y RAND_MAX, que es un número muy grande. Por tal motivo
		el retorno de rand se divide entre RAND_MAX, de esta forma, se asegura que el valor mas grande sea 1
		en vez de un número gigante, finalmente, la división se multiplica por 5.0-1.0 o por 9.0-5.0. De esta forma
		se logra que los valores guardados en las matrices siempre estén entre 0.0 y 4.0*/
		*m1 = (double)rand()/RAND_MAX*(5.0-1.0);	
		*m2 = (double)rand()/RAND_MAX*(9.0-5.0);		
	}
}

/*Funcion multiMatrixTrans que recibe dos matrices llenas, una vacia y el tamaño de las matrices
la función multiplica las matrices mA y mB, y luego el resultado dela multiplicación lo guarda en 
la matriz mC*/
void multiMatrixTrans(double *mA, double *mB, double *mC, int D){
	double Suma, *pA, *pB; //Declara una variable suma y dos apuntadores a double
	#pragma omp parallel //Directiva que crea un equipo de hilos para ejecutar todo el bloque de código seleccionado en los corchetes
	{
	/*directiva que le indica al programa que debe dividir cada iteración del primer ciclo for entre los hilos creados, 
	de tal forma cada hilo realiza una iteración de i diferente, incluyendo los ciclos for anidados adentro de esta iteración, sin 
	paralelizar estos ciclos interiores*/
	#pragma omp for
	for(int i=0; i<D; i++){ //Cilo desde 0 al tamaño de las matrices
		for(int j=0; j<D; j++){ //Ciclo desde 0 al tamaño de las matrices
			pA = mA+i*D; //Al apuntador pA se le asigna todo el principio de las filas de la matriz mA	 
			pB = mB+j*D; //Al apuntador pB se le asigna todo el principio de las filas de la matriz mB
			Suma = 0.0; //Se inializa suma en 0.0
			for(int k=0; k<D; k++, pA++, pB++){ //Se realiza un ciclo que recorre toda la fila apuntada por pA y la apuntada por pB
				Suma += *pA * *pB; //Se guarda en suma la sumatoria de la multiplicación del contenido de los apuntadores, es decir, de toda la fila 
			}
			mC[i*D+j] = Suma; //Se guarda en la posición [i][j] de la matriz el resultado de la sumatoria anterior
		}
	}
	}
}
