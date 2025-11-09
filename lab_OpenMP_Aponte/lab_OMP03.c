/**************************************************************************************
 * Fecha: 11/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Ejecución paralela de ciclos con OpenMP y manejo de variables privadas
 *
 * Objetivo:
 *    Implementar un programa que ejemplifique el uso de la directiva #pragma omp parallel for
 *    de la biblioteca OpenMP para paralelizar ciclos de iteración, y analizar el impacto del
 *    uso de variables privadas y compartidas en la ejecución concurrente de los hilos.
 *
 * Descripción:
 *    El programa define un ciclo que realiza operaciones simples sobre variables enteras.
 *    Mediante la directiva #pragma omp parallel for, se distribuyen las iteraciones del ciclo
 *    entre varios hilos para ejecutarse de forma simultánea. Las variables 'i' y 'a' se declaran
 *    como privadas (private), de modo que cada hilo mantiene su propia copia independiente.
 *    Sin embargo, la variable 'b' permanece compartida, provocando una condición de carrera
 *    (race condition) al ser modificada simultáneamente por múltiples hilos, lo que genera
 *    resultados inconsistentes.
 *
 * Aspectos cubiertos:
 *  - Inclusión y uso del encabezado omp.h
 *  - Empleo de la directiva #pragma omp parallel for
 *  - Declaración de variables privadas mediante la cláusula private()
 *  - Análisis del comportamiento de variables compartidas (shared)
 *  - Identificación de condiciones de carrera en entornos paralelos
 *  - Ejecución simultánea de iteraciones en múltiples hilos
 *  - Comprensión del impacto del paralelismo en la coherencia de datos
 *
 **************************************************************************************/


#include <omp.h>     // Proporciona las funciones y directivas para paralelismo con OpenMP
#include <stdio.h>   // Permite entrada y salida estándar (printf, scanf, etc.)
#include <stdlib.h>  // Contiene funciones generales del sistema (malloc, atoi, exit, etc.)

/*Función principal que realiza todo el programa*/
int main() {
    int i; //Se declara un entero i
    const int N = 1000; //Se declara un entero constante N y se le asigna el valor de 1000
    int a = 50; //Se declara un entero a y se le da el valor de 50
    int b = 0; //Se declara un entero b y se le da el valor de 0

    /*Se llama a una directiva del compilador mediante #pragma
    Una directiva le dice al compilador que debe realizar algo diferente durante la compilación del código, de tal forma,
    no se cambia el código en si pero si la manera en como se ejecutará posteriormente a compilarlo.
    
    Luego de la directiva, se llama a la otra parte, compuesta por las siguientes directivas:

    
    omp: indica que debe seguir una instrucción de OpenMP
    parallel for: indica que el siguiente ciclo for que se ejecute en el programa se debe realizar de manera paralela, donde
    cada hilo debe realizar una parte de las iteraciones del ciclo de forma que el trabajo se reparte automáticamente.
    private(i) y private(a): Indica que las variables acá señaladas tendrán su propia copia para cada hilo, de tal forma que lo que haga
    un hilo con estas variables no afecte a lo que haga otro hilo con las mismas, de tal manera se evita la generación de race conditions*/
    #pragma omp parallel for private(i) private(a)
    for (i = 0; i < N; i++) { //Se ejecuta el ciclo for que será paralelizado, desde 0 hasta 999
        b = a + i; //Se guarda en b la suma de a con i
    }

    /*Se imprimen los valores de a y de b, además del valor que se espera obtener de cada uno de estos*/
    printf("a = %d b = %d (Se espera a = 50 b = 1049)\n", a, b);
    return 0;
}

/*Es importante resaltar que en este ejemplo, los valores finales obtenidos van a ser diferentes a los que se esperan, pues,
al b ser una variable compartida para todos los hilos que ejecutan el programa, lo que causa es que varios hilos puedan estar
sobreescribiendo en b al mismo tiempo, generando así una race condition que hace cambiar al valor de manera indefinida*/
