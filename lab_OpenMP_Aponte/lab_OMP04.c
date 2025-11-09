/**************************************************************************************
 * Fecha: 11/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Control de variables privadas y uso de lastprivate en OpenMP
 *
 * Objetivo:
 *    Implementar un ciclo for paralelo utilizando OpenMP, controlando adecuadamente
 *    el alcance de las variables privadas y compartidas para evitar condiciones de
 *    carrera, y garantizando que el valor final de la variable b corresponda al de
 *    la última iteración del ciclo mediante la cláusula lastprivate.
 *
 * Descripción:
 *    El programa declara e inicializa variables locales que serán utilizadas dentro
 *    de una región paralela. Utiliza la directiva #pragma omp parallel for para
 *    distribuir las iteraciones del ciclo entre varios hilos, aprovechando los núcleos
 *    del procesador. Las variables i y a se declaran privadas para cada hilo, mientras
 *    que b se declara con lastprivate, asegurando que conserve el valor final de la
 *    última iteración (i = 999). De esta forma, el programa produce el resultado
 *    esperado: a = 50 y b = 1049.
 *
 * Aspectos cubiertos:
 *  - Inclusión y uso del encabezado omp.h
 *  - Paralelización de bucles con #pragma omp parallel for
 *  - Definición de variables privadas (private)
 *  - Uso de la cláusula lastprivate para conservar valores finales
 *  - Prevención de condiciones de carrera (race conditions)
 *  - Comprensión del comportamiento de las variables en regiones paralelas
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
    un hilo con estas variables no afecte a lo que haga otro hilo con las mismas, de tal manera se evita la generación de race conditions
    lastprivate(b): Originalmente era private, pero esto causaba que se perdiera su valor cuando acababa el ciclo for
    Con el cambio, indica que la variable b será privada para cada hilo, pero que conservará el valor que tuvo al ser ejecutada por el último*/
    #pragma omp parallel for private(i) private(a) lastprivate(b)
    for (i = 0; i < N; i++) {  //Se ejecuta el ciclo for que será paralelizado, desde 0 hasta 999
        a = 50; //Al principio de cada iteración, se reitera que a es igual a 50, pues al ser una variable privada este valor puede verse afectado
        b = a + i; //Se guarda en b la suma de a con i
    }
       /*Se imprimen los valores de a y de b, además del valor que se espera obtener de cada uno de estos*/
    printf("a = %d b = %d (Se espera a = 50 b = 1049)\n", a, b); 
    return 0;
}
/*Esta versión del código se encuentra corregida con respecto al laboratorio 3, pues ya cuenta con todo lo necesario para que el resultado mostrado
sea igual al resultado esperado*/