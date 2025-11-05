/**************************************************************************************
 * Fecha: 04/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Sincronización de hilos usando mutex con PTHREAD
 *
 * Objetivo:
 *    Demostrar el uso de la interfaz PTHREAD para crear múltiples hilos y coordinar
 *    su ejecución mediante un mecanismo de exclusión mutua (mutex), evitando
 *    condiciones de carrera en una sección crítica del programa.
 *
 * Descripción:
 *    El programa crea tres hilos utilizando pthread_create, los cuales ejecutan
 *    una misma función (compute). Dentro de dicha función, se incrementa un
 *    contador global y se simula una operación costosa mediante un ciclo vacío.
 *
 *    Para evitar que varios hilos modifiquen la variable counter al mismo tiempo
 *    (lo que causaría una race condition), se usa un mutex. El mutex asegura que
 *    solo un hilo a la vez pueda ingresar a la sección crítica donde se incrementa
 *    el contador e imprime mensajes de inicio y fin de su tarea.
 *
 *    Además, se valida la creación de cada hilo verificando el valor retornado por
 *    pthread_create. Si hay un error, se utiliza strerror() para imprimir una
 *    descripción legible del error.
 *
 * Aspectos cubiertos:
 *  - Creación de hilos con pthread_create
 *  - Uso de pthread_join para esperar la finalización de hilos
 *  - Sincronización de hilos con pthread_mutex_lock y pthread_mutex_unlock
 *  - Protección de una sección crítica para evitar condiciones de carrera
 *  - Manejo básico de errores y uso de strerror para interpretarlos
 *
 **************************************************************************************/


#include <stdio.h> //Libreria para entrada y salida estándar
#include <pthread.h> //Libreria para creación y manejo de hilos
#include <stdlib.h> //Funciones generales del sistema, manejo de memoria, números y salida del programa


 /*Se crea un arreglo de tipo pthread_t, que es el tipo de dato que usar pthreads para identificar un hilo, almacenando en el los ID
    de todos lo hilos que son creados posteriormente por pthread_create*/
pthread_t tid[3];
int counter = 0; // Se declara una variable global de tipo entero llamada counter
 


/*Se inicializa un mutex usando las siguientes carácterísticas:

pthread_mutex_t: Es el tipo de variable usado por POSIX para representar un mutex (mutual exclusion lock), el mutex
se utiliza para proteger secciones críticas, asegurando que un solo hilo pueda ejecutar cierta región de código o acceder a cierta estructura
de datos a la vez, evitando que se generen errores durante la ejecución.

al mutex declarado se le da el nombre: lock.

El valor asignado a lock es PTHREAD_MUTEX_INITIALIZER, que es una inicialización estática del mutex provista
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

-pthread_mutex_lock(&lock): Bloquea un proceso hasta adquirir el mutex.
-pthread_mutex_trylock(&lock): Trara de adquirir el mutex, si no puede devuelve EBUSY.
-pthread_mutex_timedlock(&lock, &abstime): Bloquea el proceso y espera hasta un tiempo límite.
-pthread_mutex_unlock(&lock): Libera el mutex.*/
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;;



/*Función que será ejecutada por cada hilo creado.La función imprime un contador aumentado con un mutex, para que 
no se produzca ninguna race condition al ser ejecutada por varios hilos.
De tipo puntero, pues este es el formato necesario para enviarla como parámetro en la creación de un hilo.
La función retorna un puntero, de
tal forma puede ser recogido posteriormente por pthread_join y recibe como parámetro un puntero genérico que puede 
apuntar a cualquier tipo de dato*/
void* compute(void *arg) {
    unsigned long i = 0; //Se declara una variable de tipo unsigned long y se le coloca el nombre i

    /*Se usa la función pthread_mutex_lock para proteger una zona crítica del código que sería la que está después
    del llamado a esta función, la función recibe una variable de tipo pthread_mutex_t y con ella hace lo siguiente:

    -Le asigna a la variable mutex una flag o estado, los cuáles pueden ser LOCKED, UNLOCKED
    -Guarda en el mutex la información del hilo que lo posee
    -Guarda en el mutex una cola de los hilos que están esperando por su uso

    La función como tal hace lo siguiente:

    1. Revisa si el mutex está libre
    2. Si el mutex está libre, se lo asigna al hilo que está ejecutando el programa
    3. Si está ocupado, el hilo se queda esperando hasta que se libere el mutex*/
    pthread_mutex_lock(&lock);
    counter += 1; //Se le aumenta 1 al contador

    printf("\n Job %d has started\n", counter); //Se imprime que el trabajo del número del contador ha empezado

    /*Ciclo for vacio usado para que el procesador se quede un tiempo esperando sin hacer nada en específico, este ciclo for va desde
    0 hasta el número en hexadecimal 0xFFFFFFFF, que en decimal seria: 4294967295, al final de este for se escribe ;, lo que significa que su 
    cuerpo etá vacio */
    for(i = 0; i < (0xFFFFFFFF); i++);
    
    printf("\n Job %d has finished\n", counter);  //Se imprime que el trabajo del número del contador ha terminado

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
    pthread_mutex_unlock(&lock);

    return NULL; // Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se retorna NULL, es decir, un puntero vacío
}

/*Función principal donde se realiza toda la operación con hilos y el llamado a la función definida anteriormente*/
int main(void) {
    int i = 0; //Se declara un entero y se le coloca el nombre i
    int error; //Se declara un entero y se le coloca el nombre error

    /*Se crea un ciclo while hasta que i sea menor que 3, adentro de este ciclo se realiza la creación de los hilos usando el arreglo
    global de hilos declarado anteriormente, además de verificar errores usando la variable error creada anteriormente*/
    while(i < 3) {
        /*Creacion de 3 hilos usando la función pthread_create. Esta función guarda el ID de un hilo en un arreglo de tipo
        pthread_t, y hace que una función sea ejecutada por ese arreglo. Los parámetros que espera la función son los siguientes:

        -Dirección de memoria de tipo pthread_t, que guarda el ID del hilo creado en ese momento
        -Valores por defecto que se le van a dar al hilo, como por ejemplo el tamaño de su stack, indicarle que no debe esperar join, etc.
        en este caso se le envia NULL, pues los hilos utilizados en el programa no tienen ningún valor pre-definido.
        -La función que será ejecutada por los hilos, esta función debe retornar un apuntador de memoria y solo debe recibir un parámetro
        que debe ser de tipo genérico (void *arg), pues esto es lo exigido por pthread_create, ya que si la función recibiera mas de un parámetro
        y además no fuera genérico, el programa no podría saber con certeza que debe retornar, ni pthread_join sabria con certeza que debe recibir
        en caso de que sea usado
        -El valor que se le enviará a la función declarada anteriormente, este valor puede ser de cualquier tipo, pues la función recibe un
        parámetro genérico, en este caso se le envía el valor númerico que se está iterando y guardando en el arreglo de thread_id.
        
        pthread_create puede retornar 0, si la creación de los hilos fue correcta o diferentes codigos de error definido por POSIX, 
        según sea el caso. Para manejar estos errores se guarda el retorno de pthread_create en la variable error*/
        error = pthread_create(&(tid[i]), NULL, &compute, NULL);
        /*Si la variable error guarda un valor diferente a 0, es porque se tuvo un error en la creación de los hilos, en caso
        de que esto suceda, se imprime que ocurrió un error y cuál fué, utilizando la función strerror de C, que recibe un código
        de error y lo imprime en lenguaje natural*/
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
        i++; //Se aumenta i
    }

    /*Se utiliza la función join en todos los hilos creados anteriormente usando pthread_create.
        La función de join, es esperar que todos terminen y poder liberar los recursos utilizados por estos hilos,

        Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba
        ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya
        fueron liberados.

        El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
        lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le
        envía NULL en este parámetro, para que la función no guarde*/
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    return 0;
}