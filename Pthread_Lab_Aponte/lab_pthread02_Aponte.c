/**************************************************************************************
 * Fecha: 04/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Creación de múltiples hilos con exclusión mutua (Mutex) usando PTHREAD
 *
 * Objetivo:
 *    Implementar un programa concurrente en C que ejemplifique el uso coordinado de
 *    múltiples hilos mediante PTHREAD y la protección de zona crítica usando mutex.
 *
 * Descripción:
 *    El programa crea N hilos (definidos mediante la directiva de preprocesador
 *    NTHREADS). Cada hilo recibe un identificador único, imprime su número de hilo
 *    junto con su ID real del sistema, y posteriormente accede a una sección crítica
 *    donde incrementa de forma segura un contador global utilizando un mutex.
 *
 *    El uso de pthread_mutex_lock y pthread_mutex_unlock asegura que el contador
 *    sea modificado por un solo hilo a la vez, evitando condiciones de carrera.  
 *    Además, cada argumento pasado al hilo se reserva dinámicamente y se libera dentro
 *    del mismo para prevenir fugas de memoria.
 *
 * Aspectos cubiertos:
 *  - Creación de hilos con pthread_create
 *  - Uso de pthread_t para manejo de identificadores de hilo
 *  - Implementación de exclusión mutua mediante pthread_mutex_t
 *  - Protección de sección crítica con pthread_mutex_lock y pthread_mutex_unlock
 *  - Uso de memoria dinámica para paso de argumentos a los hilos
 *  - Liberación de memoria reservada en cada hilo
 *  - Sincronización mediante pthread_join
 *  
 **************************************************************************************/


/*Directiva de preprocesador en C, hace que antes de que el programa compile, cada vez que aparezca
la palabra "NTHREADS" en el código, el preprocesador lo reemplace por 10, es decir, NTHREADS no ocupa memoria,
no crea variables y tampoco existe en tiempo de ejecución, simplemente busca la palabra definida y la reemplaza
por el número dado*/
#define NTHREADS 10

#include <stdio.h>   //Libreria para entrada y salida estándar
#include <pthread.h> //Libreria para creación y manejo de hilos
#include <stdlib.h>  //Funciones generales del sistema, manejo de memoria, números y salida del programa

/*Se inicializa un mutex usando las siguientes carácterísticas:

pthread_mutex_t: Es el tipo de variable usado por POSIX para representar un mutex (mutual exclusion lock), el mutex
se utiliza para proteger secciones críticas, asegurando que un solo hilo pueda ejecutar cierta región de código o acceder a cierta estructura
de datos a la vez, evitando que se generen errores durante la ejecución.

al mutex declarado se le da el nombre: mutex1.

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

-pthread_mutex_lock(&mutex1): Bloquea un proceso hasta adquirir el mutex.
-pthread_mutex_trylock(&mutex1): Trara de adquirir el mutex, si no puede devuelve EBUSY.
-pthread_mutex_timedlock(&mutex1, &abstime): Bloquea el proceso y espera hasta un tiempo límite.
-pthread_mutex_unlock(&mutex1): Libera el mutex.*/
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int counter = 0; // Se declara una variable global de tipo entero llamada counter

/*Función que será ejecutada por cada hilo creado.La función imprime el argumento que se le pasa y el id del hilo que la
está ejecutando, además de esto aumenta un contador declarado globalmente, que es bloqueado con pthread_mutex_lock, para que se realice
el proceso correctamente, sin ninguna race condition.
De tipo puntero, pues este es el formato necesario para enviarla como parámetro en la creación de un hilo.
La función retorna un puntero, de
tal forma puede ser recogido posteriormente por pthread_join en caso de que sea necesario
y recibe como parámetro un puntero genérico que puede
apuntar a cualquier tipo de dato*/
void *thread_function(void *arg)
{
    int i = *(int *)(arg); // El dato enviado como parámetro a la función es casteado a entero y guardado en la variable i

    /*La función realiza una impresión por consola, que contiene las siguientes carácteristicas:

    Imprime un texto para indicar que se imprime el número del hilo que está realizando la función
    %d: Espera un entero, y lo imprime
    Imprime un segundo texto para indicar que se imprimirá el ID del hilo
    %ld: Espera imprimir un entero largo (long int)
    i: Entero que será imprero por %d, es el valor enviado como parámetro a la función
    pthread_self(): Función que retorna el ID real del hilo que se encuentra ejecutando el código en ese momento
    retornaa pthread_t, valor que es compatible con ld para su impresión en printf*/
    printf("Thread number: %d | Thread ID %ld\n", i, pthread_self());

    /*Se usa la función pthread_mutex_lock para proteger una zona crítica del código que sería la que está después
    del llamado a esta función, la función recibe una variable de tipo pthread_mutex_t y con ella hace lo siguiente:

    -Le asigna a la variable mutex una flag o estado, los cuáles pueden ser LOCKED, UNLOCKED
    -Guarda en el mutex la información del hilo que lo posee
    -Guarda en el mutex una cola de los hilos que están esperando por su uso

    La función como tal hace lo siguiente:

    1. Revisa si el mutex está libre
    2. Si el mutex está libre, se lo asigna al hilo que está ejecutando el programa
    3. Si está ocupado, el hilo se queda esperando hasta que se libere el mutex*/
    pthread_mutex_lock(&mutex1);
    counter++; // Zona crítica del código, en este caso es un contador que se ejecuta por cada llamado a la función, siendo protegido por un mutex

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
    pthread_mutex_unlock(&mutex1);
    free(arg);   // se libera la memoria reservada para el argumento enviado como parámetro
    return NULL; // Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se retorna NULL, es decir, un puntero vacío
}
/*Función principal donde se realiza toda la operación con hilos y el llamado a la función definida anteriormente*/
void main()
{
    /*Se crea un arreglo de tipo pthread_t, que es el tipo de dato que usar pthreads para identificar un hilo, almacenando en el los ID
    de todos lo hilos que son creados posteriormente por pthread_create*/
    pthread_t thread_id[NTHREADS];
    int i, j; // Se declaran dos variables de tipo entero llamadas i y j
    /*Bucle que ejecuta la cantidad NTHREADS, definida anteriormente, en los hilos creados con la función pthread_create*/
    for (i = 0; i < NTHREADS; i++)
    {
        /*Como a pthread_create se le debe enviar una dirección de memoria, no se le puede enviar la dirección de i, pues esto
        significaria que todos los hilos leerian la misma dirección de memoria, cosa que generaria errores en la compilación del
        código. Para corregir esto se declara un apuntador a entero llamado arg, y se le reserva el tamaño de un entero de memoria,
        luego, a este apuntador se le da el valor que está iterando i, de tal forma cada hilo recibe una dirección de memoria
        diferente, evitando posibles errores*/
        int *arg = malloc(sizeof(int));
        *arg = i;
        /*Creacion de NTHREADS hilos usando la función pthread_create. Esta función guarda el ID de un hilo en un arreglo de tipo
        pthread_t, y hace que una función sea ejecutada por ese arreglo. Los parámetros que espera la función son los siguientes:

        -Dirección de memoria de tipo pthread_t, que guarda el ID del hilo creado en ese momento
        -Valores por defecto que se le van a dar al hilo, como por ejemplo el tamaño de su stack, indicarle que no debe esperar join, etc.
        en este caso se le envia NULL, pues los hilos utilizados en el programa no tienen ningún valor pre-definido.
        -La función que será ejecutada por los hilos, esta función debe retornar un apuntador de memoria y solo debe recibir un parámetro
        que debe ser de tipo genérico (void *arg), pues esto es lo exigido por pthread_create, ya que si la función recibiera mas de un parámetro
        y además no fuera genérico, el programa no podría saber con certeza que debe retornar, ni pthread_join sabria con certeza que debe recibir
        en caso de que sea usado
        -El valor que se le enviará a la función declarada anteriormente, este valor puede ser de cualquier tipo, pues la función recibe un
        parámetro genérico, en este caso se le envía el valor númerico que se está iterando yconvertido al apuntador arg.*/
        pthread_create(&thread_id[i], NULL, thread_function, arg);
    }

    /*Se agrega un ciclo para la unión de los hilos: esperar a que todos terminen*/
    for (j = 0; j < NTHREADS; j++)
    {
        /*Se utiliza la función join iterando en todos los hilos creados anteriormente usando pthread_create.
        La función de join, es esperar que todos terminen y poder liberar los recursos utilizados por estos hilos,

        Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba
        ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya
        fueron liberados.

        El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
        lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le
        envía NULL en este parámetro, para que la función no guarde*/
        pthread_join(thread_id[j], NULL);
    }
    /*Luego de ejecutar todo el código se imprime el contador declarado globalmente*/
    printf("Final counter value: %d\n", counter);
}