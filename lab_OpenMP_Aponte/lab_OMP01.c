/**************************************************************************************
 * Fecha: 11/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Uso de interfaz de paralelismo con OpenMP
 *
 * Objetivo:
 *    Implementar un programa básico que ejemplifique el uso de la biblioteca OpenMP
 *    para la creación y ejecución de regiones paralelas mediante múltiples hilos,
 *    demostrando cómo se aprovechan los núcleos del procesador en la ejecución
 *    concurrente de instrucciones.
 *
 * Descripción:
 *    El programa obtiene el número máximo de hilos que OpenMP puede utilizar en la
 *    máquina local mediante la función omp_get_max_threads(). Posteriormente, se
 *    crea una región paralela usando la directiva #pragma omp parallel, dentro de la
 *    cual cada hilo imprime un mensaje identificando su número único de ejecución,
 *    obtenido mediante la función omp_get_thread_num(). Este ejemplo muestra cómo
 *    OpenMP distribuye automáticamente el trabajo entre los diferentes hilos activos.
 *
 * Aspectos cubiertos:
 *  - Inclusión y uso del encabezado omp.h
 *  - Obtención del número máximo de hilos disponibles (omp_get_max_threads)
 *  - Creación de una región paralela con #pragma omp parallel
 *  - Identificación individual de hilos mediante omp_get_thread_num
 *  - Ejecución simultánea de instrucciones en múltiples núcleos
 *  - Comprensión del paralelismo implícito en la programación concurrente
 *
 *************************************************************************************/


#include <omp.h>     // Proporciona las funciones y directivas para paralelismo con OpenMP
#include <stdio.h>   // Permite entrada y salida estándar (printf, scanf, etc.)
#include <stdlib.h>  // Contiene funciones generales del sistema (malloc, atoi, exit, etc.)

/*Función principal que realiza todo el programa*/
int main(int argc, char *argv[]) {

    /*Se declara una variable de tipo entero llamada maxCores, a esta variable se le asigna el retorno de la función
    de openMP omp_get_max_threads(), lo que hace esta función es retornar el número máximo de hilos que el entorno
    OpenMP puede usar en una región praalela si no se especifica esta cantidad. Generalmente, el número máximo de hilos
    utilizables corresponde con el total de hilos de los que dispone la maquina donde se está ejecutando el programa*/
    int maxCores = omp_get_max_threads();

    /*Se imprime el retorno de omp_get_max_threads por pantalla, indicándole al usuario con cuantos hilos va a trabajar 
    openMP para la región paralela*/
    printf("OpenMP ejecutando con %d hilos\n", maxCores);

    /*Se llama a una directiva del compilador mediante #pragma
    Una directiva le dice al compilador que debe realizar algo diferente durante la compilación del código, de tal forma,
    no se cambia el código en si pero si la manera en como se ejecutará posteriormente a compilarlo.
    
    Luego de la directiva, se llama a la otra parte, que sería omp parallel.
    
    omp, indica que debe seguir una instrucción de OpenMP, mientras que parallel indica que la sección de código siguiente a
    esta directiva se debe ejecutar de forma paralela, mediante la creación de un número máximo de hilos, ya sea fijados con una función
    de OpenMP, o en caso contrario la cantidad máxima de hilos de la máquina en donde se ejecute el código */
    #pragma omp parallel
    {
        /*Se realiza paralelamente la función printf, en la que se imprime un mensaje desde cada hilo creado por OpenMP y
        posteriormente se usa omp_get_thread_num para imprimir el númeor del hilo desde donde se está ejecutando el código, siendo
        este número único para cada hilo, yendo desde 0 hasta la cantidad máxima de hilos que usará OpenMP */
        printf("Hello World desde el thread %d\n", omp_get_thread_num());
    }
    return 0;
}