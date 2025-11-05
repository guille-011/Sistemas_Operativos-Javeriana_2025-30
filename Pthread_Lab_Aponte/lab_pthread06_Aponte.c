/**************************************************************************************
 * Fecha: 04/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Sincronización de hilos usando Mutex (PTHREAD)
 *
 * Objetivo:
 *    Demostrar la creación y ejecución de múltiples hilos utilizando la interfaz PTHREAD,
 *    y proteger una variable compartida mediante un mutex para evitar condiciones de carrera,
 *    asegurando resultados consistentes.
 *
 * Descripción:
 *    El programa crea 20 hilos que ejecutan la función 'cuadrado'. Cada hilo recibe un 
 *    número, lo eleva al cuadrado y suma el resultado a la variable global 'acumulado'.
 *    
 *    A diferencia de la versión anterior, en esta se utiliza un mutex (bloqueoCC) para 
 *    proteger la sección crítica donde se modifica 'acumulado'. Esto garantiza que solo 
 *    un hilo a la vez pueda acceder a la variable, eliminando los errores ocasionados 
 *    por condiciones de carrera.
 *
 *    Cada hilo es creado con pthread_create y luego se espera a que todos terminen 
 *    utilizando pthread_join antes de imprimir el resultado final.
 *
 * Aspectos cubiertos:
 *  - Creación de hilos con pthread_create
 *  - Espera de finalización de hilos con pthread_join
 *  - Protección de secciones críticas con pthread_mutex_lock y pthread_mutex_unlock
 *  - Eliminación de condiciones de carrera en la variable compartida
 *  - Garantía de resultados correctos en todas las ejecuciones
 *
 **************************************************************************************/


#include <stdio.h> //Libreria para entrada y salida estándar
#include <pthread.h> //Libreria para creación y manejo de hilos
#include <stdlib.h> //Funciones generales del sistema, manejo de memoria, números y salida del programa
#include <stdint.h> //Libreria para trabajar con enteros de tamaño fijo


int acumulado = 0;  //Se declara una variable global de tipo entero y se le coloca el nombre acumulado
/*Se inicializa un mutex usando las siguientes carácterísticas:

pthread_mutex_t: Es el tipo de variable usado por POSIX para representar un mutex (mutual exclusion lock), el mutex
se utiliza para proteger secciones críticas, asegurando que un solo hilo pueda ejecutar cierta región de código o acceder a cierta estructura
de datos a la vez, evitando que se generen errores durante la ejecución.

al mutex declarado se le da el nombre: bloqueoCC.

El valor asignado a mutex1 es PTHREAD_MUTEX_INITIALIZER, que es una inicialización estática del mutex provista
por POSIX, es decir, esta inicialización se realiza en tiempo de compilación en lugar de hacerlo en tiempo de ejecución,
si se quiere realizar su inicialización dinámica se tendria que usar pthread_mutex_init, que permite configurarle los valores
por defecto a diferencia de como se está haciendo en este código.

Valores por defecto dados por PTHREAD_MUTEX_INITIALIZER:

-Tipo de mutex: PTHREAD_MUTEX_DEFAULT: Mutex normal, no recursivo.
-Protocolo: PTHREAD_PRIO_NONE: No da prioridad al mutex.
-Compartición: PTHREAD_PROCESS_PRIVATE: Solo sirve dentro del mimso proceso.
-Robustez: no robusto, si un hilo muere con el mutex tomado se le asigna undefined behavior.
-Chequeo de errores: No valida errores, como por ejemplo desbloquearlo antes de bloquearlo.
-Recursividad: No recursivo.

Operaciones ofrecidas por pthread_mutex_t:

-pthread_mutex_lock(&bloqueoCC): Bloquea un proceso hasta adquirir el mutex.
-pthread_mutex_trylock(&bloqueoCC): Trara de adquirir el mutex, si no puede devuelve EBUSY.
-pthread_mutex_timedlock(&bloqueoCC, &abstime): Bloquea el proceso y espera hasta un tiempo límite.
-pthread_mutex_unlock(&bloqueoCC): Libera el mutex.*/
pthread_mutex_t bloqueoCC = PTHREAD_MUTEX_INITIALIZER;

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


    /*Se usa la función pthread_mutex_lock para proteger una zona crítica del código que sería la que está después
    del llamado a esta función, la función recibe una variable de tipo pthread_mutex_t y con ella hace lo siguiente:

    -Le asigna a la variable mutex una flag o estado, los cuáles pueden ser LOCKED, UNLOCKED
    -Guarda en el mutex la información del hilo que lo posee
    -Guarda en el mutex una cola de los hilos que están esperando por su uso

    La función como tal hace lo siguiente:

    1. Revisa si el mutex está libre
    2. Si el mutex está libre, se lo asigna al hilo que está ejecutando el programa
    3. Si está ocupado, el hilo se queda esperando hasta que se libere el mutex*/
    pthread_mutex_lock(&bloqueoCC);
    acumulado += xi * xi; //Se guarda en acumulado, protegido por mutex la sumatoria de los cuadrados de la variable enviada como parámetro a la función
       /*Se usa la función pthread_mutex_unlock para liberar el mutex previamente tomado por pthread_mutex_lock.
  La función recibe como parámetro la dirección de una variable de tipo pthread_mutex_t, y realiza lo siguiente:

  -Cambia el estado del mutex de LOCKED a UNLOCKED
  -Elimina la referencia del hilo que tenía el mutex (ya no es dueño)
  -Despierta a uno de los hilos que estuviera esperando en la cola para que intente tomar el mutex

  Internamente la función hace:

  1. Verifica que el hilo que llama esta función sea el mismo que bloqueó el mutex (para evitar que otro hilo lo suelte)
  2. Marca el mutex como disponible
  3. Si hay hilos esperando, le da paso al siguiente para que pueda ejecutar su sección crítica
*/
    
    pthread_mutex_unlock(&bloqueoCC);
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

/*El codigo presente da el resultado correcto 100% de las veces, ya que la condición de carrera fue
corregida mediante la aplicación de mutex*/
