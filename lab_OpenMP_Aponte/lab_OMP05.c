/**************************************************************************************
 * Fecha: 11/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Uso de la cláusula reduction y paralelización de sumatorias en OpenMP
 *
 * Objetivo:
 *    Implementar un ciclo for paralelo utilizando OpenMP, aprovechando la cláusula
 *    reduction para realizar sumatorias sin condiciones de carrera. Se busca demostrar
 *    cómo distribuir el trabajo entre múltiples hilos para acelerar la ejecución
 *    de operaciones matemáticas intensivas, garantizando al mismo tiempo la coherencia
 *    del resultado final en una variable compartida.
 *
 * Descripción:
 *    El programa calcula la sumatoria de los valores del seno de una serie de números,
 *    donde cada número de iteración define un rango propio de sumatoria interna. 
 *    Para esto, se utiliza la función fsum(), la cual calcula la suma de los senos 
 *    desde un valor de inicio hasta un valor final determinado por el índice i.
 *
 *    La región paralela se define mediante la directiva:
 *        #pragma omp parallel for reduction(+: resSumatoria)
 *    Esta directiva distribuye las iteraciones del ciclo entre varios hilos, 
 *    creando una copia privada de la variable resSumatoria para cada uno. 
 *    Una vez que todos los hilos terminan, OpenMP combina automáticamente los 
 *    resultados parciales en la variable compartida original, evitando así 
 *    condiciones de carrera y garantizando la suma correcta de todos los valores.
 *
 * Aspectos cubiertos:
 *  - Inclusión y uso del encabezado omp.h
 *  - Paralelización de bucles con #pragma omp parallel for
 *  - Uso de la cláusula reduction para combinar resultados parciales
 *  - Evitación de race conditions mediante variables privadas implícitas
 *  - Obtención del número máximo de hilos con omp_get_max_threads()
 *  - Cálculo de sumatorias matemáticas con funciones de la librería math.h
 *
 **************************************************************************************/


#include <omp.h>     // Proporciona las funciones y directivas para paralelismo con OpenMP
#include <stdio.h>   // Permite entrada y salida estándar (printf, scanf, etc.)
#include <stdlib.h>  // Contiene funciones generales del sistema (malloc, atoi, exit, etc.)
#include <math.h>    //Contiene funciones matemáticas

/*Se utiliza un macro, es decir, un preprocesador de C, que antes de compilar, realiza un reemplazo del nombre
del macro por el operador ternario que hay en su interior. De la siguiente manera:

MIN: Es el nombre que recibe el macro, x y y son los parámetros que se le deben enviar para que realice
el reemplazo correctamente

(x<y)?x:y es un operador ternario, que compara a x con y, retorna el valor mínimo de los dos enviados como parámetro*/
#define MIN(x, y) ((x < y)?x:y)

/*Función sumatoria que realiza las sumatoria de todos los senos entre inicio = i*(i+1)/2 y fin = inicio + i
la guarda en una variable valorSum y la retorna*/
double fsum(int i){
    int inicio = i*(i+1)/2; //Se guarda en inicio el valor de i*(i+1)/2
    int fin = inicio+i; //Se guarda en fin el valor de inicio + i
    double valorSum = 0.0; //Se declara la variable valorSum y se iguala a 0
    /*Cilo for que itera j desde inicio hasta fin*/
    for(int j = inicio; j < fin; j++){
        /*Se guarda en valorSum la sumatoria del valor retornado por la funcion sin de la libreria math.h, la función
        recibe un número entero, el cuál interpreta como ángulo en radianes, y luego, utilizando una serie polinómica,
        calcula el valor aproximado del seno de este ángulo y retorna un valor de double con este resultado*/
        valorSum += sin(j); 
    }

    return valorSum; //Se retorna valorSum
}

/*Función principal que realiza el llamado a las demás funciones*/
int main(int argc, char *argv[]) {

    double resSumatoria = 0; //Se declara una variable double llamada resSumatoria y se iguala a 0
    int rep = 30000; //Se declara una variable rep y se le da el valor de 30000
 
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
    
    Luego de la directiva, se llama a la otra parte, compuesta por las siguientes directivas:

    
    omp: indica que debe seguir una instrucción de OpenMP
    parallel for: indica que el siguiente ciclo for que se ejecute en el programa se debe realizar de manera paralela, donde
    cada hilo debe realizar una parte de las iteraciones del ciclo de forma que el trabajo se reparte automáticamente.
    reduction(+: resSumatoria): Clausula de OpenMp utilizada par combinar los resultados parciales de varios hilos en una 
    sola variable compartida, sin causar condiciones de carrera. De la siguiente forma: 
    Se crea una copia privada de resSumatoria para cada hilo.
    Cuando termina de ejecutarse, OpenMp combina todas las copias locales y las guarda en resSumatoria*/
    #pragma omp parallel for reduction(+: resSumatoria)
    for(int i = 0; i < rep; i++){
        /*Se guarda en resSumatoria la suma de todos los resultados retornados por la funcion fsum, enviándole como parámetro el número iterado*/
        resSumatoria += fsum(i);
    
    }
    printf("Resultado Sumatoria función SENO: %0.2f \n", resSumatoria); //Se imprime el reultado de la sumatoria del llamado a la funcións seno
    return 0;
}
