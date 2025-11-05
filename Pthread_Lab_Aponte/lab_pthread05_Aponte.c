/**************************************************************************************
 * Fecha: 04/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Creación de hilos con PTHREAD y condición de carrera
 *
 * Objetivo:
 *    Demostrar la creación de múltiples hilos utilizando la interfaz PTHREAD y el
 *    cálculo de la suma de cuadrados de números enviados a cada hilo. 
 *
 * Descripción:
 *    El programa crea 20 hilos mediante pthread_create, cada uno ejecutando la
 *    función cuadrado, que suma el cuadrado de un número al acumulado global. 
 *
 *    La variable compartida 'acumulado' no está protegida mediante mutex, por lo
 *    que cuando varios hilos intentan actualizarla simultáneamente se produce una
 *    condición de carrera. Como consecuencia, el resultado final de la suma de 
 *    cuadrados será correcto la mayoría de las veces, pero puede ser erróneo en 
 *    algunas ejecuciones.
 *
 * Aspectos cubiertos:
 *  - Creación de hilos con pthread_create
 *  - Uso de pthread_join para esperar la finalización de los hilos
 *  - Paso de parámetros a hilos mediante void* y cast a intptr_t
 *  - Condición de carrera en variables globales sin protección
 *  - Necesidad de mutex para evitar resultados inconsistentes
 *
 * Nota:
 *    Para garantizar resultados correctos en todas las ejecuciones, se debe proteger
 *    la actualización de 'acumulado' utilizando un mutex.
 *
 **************************************************************************************/


#include <stdio.h> //Libreria para entrada y salida estándar
#include <pthread.h> //Libreria para creación y manejo de hilos
#include <stdlib.h> //Funciones generales del sistema, manejo de memoria, números y salida del programa
#include <stdint.h> //Libreria para trabajar con enteros de tamaño fijo


int acumulado = 0; //Se declara una variable global de tipo entero y se le coloca el nombre acumulado

/*Función que será ejecutada por cada hilo creado. La función aumenta un contador declarado globalmente,
sumándole la multiplicación de un número enviado como parámetro a la función y casteado a intptr_t, que es
un tipo de variable estandarizado por C, que garantiza el almacenamiento de una dirección de memomoria
sin dañarla, almacenando el mimso tamaño de un puntero, la multiplicación de este número se realiza
por si mismo.
La función es de tipo puntero, pues este es el formato necesario para enviarla como parámetro en la creación de un hilo.
La función retorna un puntero, de
tal forma puede ser recogido posteriormente por pthread_join en caso de que sea necesario
y recibe como parámetro un puntero genérico que puede
apuntar a cualquier tipo de dato*/
void *cuadrado(void *x){
    int xi = (intptr_t)x; //Se le asigna al entero xi el casteo a intptr_t de la variable enviada como parámetro
    acumulado += xi * xi; //Se guarda en acumulado la sumatoria de los cuadrados de la variable enviada como parámetro a la función
    return NULL; //Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se retorna NULL, es decir, un puntero vacío
}


/*Función principal donde se realiza toda la operación con hilos y el llamado a la función definida anteriormente*/
int main(){

    /*Se crea un arreglo de tipo pthread_t, que es el tipo de dato que usar pthreads para identificar un hilo, almacenando en el los ID
    de todos lo hilos que son creados posteriormente por pthread_create*/
    pthread_t WorkersTH[20];

    /*Bucle que ejecuta la creación de 20 hilos con la función pthread_create*/
    for(int i=0; i<20; i++){
         /*Creacion de 20 hilos usando la función pthread_create. Esta función guarda el ID de un hilo en un arreglo de tipo
        pthread_t, y hace que una función sea ejecutada por ese arreglo. Los parámetros que espera la función son los siguientes:

        -Dirección de memoria de tipo pthread_t, que guarda el ID del hilo creado en ese momento
        -Valores por defecto que se le van a dar al hilo, como por ejemplo el tamaño de su stack, indicarle que no debe esperar join, etc.
        en este caso se le envia NULL, pues los hilos utilizados en el programa no tienen ningún valor pre-definido.
        -La función que será ejecutada por los hilos, esta función debe retornar un apuntador de memoria y solo debe recibir un parámetro
        que debe ser de tipo genérico (void *arg), pues esto es lo exigido por pthread_create, ya que si la función recibiera mas de un parámetro
        y además no fuera genérico, el programa no podría saber con certeza que debe retornar, ni pthread_join sabria con certeza que debe recibir
        en caso de que sea usado
        -El valor que se le enviará a la función declarada anteriormente, este valor puede ser de cualquier tipo, pues la función recibe un
        parámetro genérico, en este caso se le envía el valor númerico que se está iterando casteado a intptr_t, para que tenga el tamaño
        de un entero y a void para que pueda ser recibido por la función.*/
        pthread_create(&WorkersTH[i], NULL, &cuadrado, (void *)(intptr_t)(i+1));
    }
    /*Se crea un ciclo para la unión de los hilos: esperar a que todos terminen*/
    for(int i=0; i<20; i++){

        /*Se utiliza la función join iterando en todos los hilos creados anteriormente usando pthread_create.
        La función de join, es esperar que todos terminen y poder liberar los recursos utilizados por estos hilos,

        Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba
        ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya
        fueron liberados.

        El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
        lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le
        envía NULL en este parámetro, para que la función no guarde*/
        pthread_join(WorkersTH[i], NULL);
    }

    /*Se imprime el resultado de la variable acumulado luego de ser ejecutada por todos los hilos*/
    printf("La suma de cuadrados es: %d\n", acumulado);
    return 0;
}
/*El codigo presente da el resultado correcto 99.7% de las veces, esto debido a una condicion de carrera
producida cuando mas de un hilo trata de ejecutar la función cuadrado a la vez, afectando así a la variable
acumulado y generando resultados erroneos, para corregir esto, se debe aplicar un mutex*/
