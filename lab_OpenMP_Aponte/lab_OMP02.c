/**************************************************************************************
 * Fecha: 11/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Uso de interfaz de paralelismo con OpenMP
 *
 * Objetivo:
 *    Implementar un programa básico que ejemplifique el uso de la biblioteca OpenMP
 *    para la creación y ejecución de regiones paralelas mediante múltiples hilos,
 *    mostrando cómo se puede fijar la cantidad de hilos desde la línea de comandos
 *    y cómo OpenMP aprovecha los núcleos del procesador para la ejecución concurrente.
 *
 * Descripción:
 *    El programa recibe como argumento el número de hilos a utilizar. Luego obtiene
 *    el número máximo de hilos disponibles en la máquina mediante la función
 *    omp_get_max_threads() y fija el número deseado de hilos con omp_set_num_threads().
 *    Posteriormente, se crea una región paralela usando la directiva #pragma omp parallel,
 *    dentro de la cual cada hilo imprime un mensaje indicando su identificador,
 *    obtenido con omp_get_thread_num().
 *
 * Aspectos cubiertos:
 *  - Inclusión y uso del encabezado omp.h
 *  - Lectura de argumentos desde la línea de comandos (argc, argv)
 *  - Conversión de valores con atoi()
 *  - Obtención del número máximo de hilos (omp_get_max_threads)
 *  - Fijación manual de hilos (omp_set_num_threads)
 *  - Creación de región paralela con #pragma omp parallel
 *  - Identificación individual de hilos (omp_get_thread_num)
 *  - Demostración práctica del paralelismo implícito con OpenMP
 *
 **************************************************************************************/

#include <omp.h>     // Proporciona las funciones y directivas para paralelismo con OpenMP
#include <stdio.h>   // Permite entrada y salida estándar (printf, scanf, etc.)
#include <stdlib.h>  // Contiene funciones generales del sistema (malloc, atoi, exit, etc.)


/*Función principal que realiza todo el programa*/
int main(int argc, char *argv[]) {

    if(argc != 2){  //Si se envian menos de 2 argumentos (La cantidad de hilos y el ejecutable), se imprime un mensaje de error
        printf("Error \n\t $Ejecutable numHilos\n"); //Mensaje de error
        exit(0); //Salida del programa en caso de error
    }

    /*Se declara el entero numHilos y se le envia la conversión a entero del valor enviado como parámetro al main por el usuario*/
    int numHilos = (int) atoi(argv[1]);

    /*Se declara una variable de tipo entero llamada maxCores, a esta variable se le asigna el retorno de la función
    de openMP omp_get_max_threads(), lo que hace esta función es retornar el número máximo de hilos que el entorno
    OpenMP puede usar en una región praalela si no se especifica esta cantidad. Generalmente, el número máximo de hilos
    utilizables corresponde con el total de hilos de los que dispone la maquina donde se está ejecutando el programa*/
    int maxCores = omp_get_max_threads();

    /*Se imprime el retorno de omp_get_max_threads por pantalla, indicándole al usuario con cuantos hilos va a trabajar 
    openMP para la región paralela*/
    printf("OpenMP ejecutando con %d hilos\n", maxCores);


    /*Función de OpenMp utilizada para fijar la cantidad de hilos que será usada en el siguiente bloque de código paralelizado
    recibe un entero que representa este número.
    
    El siguiente bloque de código que implemente un pragma para usar paralelismo usará máximo la cantidad de hilos ingresada en esta
    función
    
    En caso de que se coloque un número mayor al que tiene disponible el sistema operativo, se gestiona con "times slicing", alternando
    los hilos en los núcleos disponibles en el sistema operativo, generando cambios de contexto que reducen el rendimiento real*/
    omp_set_num_threads(numHilos);

    printf("Número de hilos fijados: %d \n", numHilos); //Se imprime por pantalla la cantidad de hilos fijada por omp_get_max_threads

    
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
