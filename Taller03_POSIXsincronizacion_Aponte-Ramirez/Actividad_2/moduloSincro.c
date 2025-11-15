/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: MUTEXES y COND VARIABLES con POSIX
 * Archivo: moduloSincro.c
 * Descripcion: Implementa la lógica de sincronización productor–
 *              consumidor sobre un búfer de mensajes en memoria,
 *              utilizando mutexes y variables de condición POSIX
 *              para coordinar el acceso concurrente, controlar la
 *              disponibilidad de espacios en el búfer y gestionar
 *              el flujo de producción e impresión de líneas.
 * Fecha: 14/11/25
 ******************************************************************/


#include "moduloSincro.h"

char buf [MAX_BUFFERS] [100]; //Se declara un buffer de caracteres de tamaño MAX_BUFFERS x 100
int buffer_index; //Índice para el buffer de producción
int buffer_print_index; //Índice para el buffer de impresión


/*Se inicializa un mutex usando las siguientes carácterísticas:

pthread_mutex_t: Es el tipo de variable usado por POSIX para representar un mutex (mutual exclusion lock), el mutex
se utiliza para proteger secciones críticas, asegurando que un solo hilo pueda ejecutar cierta región de código o acceder a cierta estructura
de datos a la vez, evitando que se generen errores durante la ejecución.

al mutex declarado se le da el nombre: buf_mutex.

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

-pthread_mutex_lock(&buf_mutex.): Bloquea un proceso hasta adquirir el mutex.
-pthread_mutex_trylock(&buf_mutex.): Trara de adquirir el mutex, si no puede devuelve EBUSY.
-pthread_mutex_timedlock(&buf_mutex., &abstime): Bloquea el proceso y espera hasta un tiempo límite.
-pthread_mutex_unlock(&buf_mutex.): Libera el mutex.*/
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;


/*Se inicializan dos variables de condición usando las siguientes características:

pthread_cond_t: Es el tipo de variable usado por POSIX para representar una condición
(condition variable). Las variables de condición permiten que los hilos se sincronicen
entre sí mediante señales, haciendo que un hilo pueda esperar (bloquearse) hasta que
otro hilo indique (despierte) que cierta condición se ha cumplido.

El valor asignado a la variable de condición es PTHREAD_COND_INITIALIZER, lo cual indica
una inicialización estática provista por POSIX. Esto significa que la configuración se
realiza en tiempo de compilación. Si se quisiera inicializar dinámicamente, se usaría
pthread_cond_init, que permite configurar atributos personalizados.

Valores por defecto dados por PTHREAD_COND_INITIALIZER:

- Tipo de atributo: PTHREAD_COND_DEFAULT
- Sin prioridad ni orden especial de activación de hilos
- No compartible entre procesos a menos que se configure explícitamente

Operaciones ofrecidas por pthread_cond_t:

- pthread_cond_wait(&condition_cond, &mutex):
  Hace que el hilo espere hasta que ocurra una señal. Mientras espera, el mutex asociado
  se libera automáticamente y se vuelve a adquirir cuando la condición se cumple.

- pthread_cond_signal(&condition_cond):
  Despierta a un hilo que esté esperando en la variable de condición.

- pthread_cond_broadcast(&condition_cond):
  Despierta a *todos* los hilos que estén esperando en la condición.

Notas importantes:
- Siempre debe usarse junto con un mutex para evitar race conditions.
- Las variables de condición no guardan un estado por sí mismas; dependen de una condición
  lógica definida en el programa y del mutex asociado.
*/
pthread_cond_t buf_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t spool_cond = PTHREAD_COND_INITIALIZER;
int buffers_available = MAX_BUFFERS; //se declara una variable entera que indica la cantidad de buffers disponibles
int lines_to_print = 0; //se declara una variable entera que indica la cantidad de líneas por imprimir

/*Función que escribe una serie de mensajes sobre el buffer compartido*/
void *producer (void *arg){
    int i, r; //declara dos variables de tipo entero, i y r
    int my_id = *((int *) arg); //declara una
    int count = 0; //declara e inicializa count en 0


    /*Ciclo desde 0 hasta 10 que enviará los mensajes del productos*/
    for (i = 0; i < 10; i++) {
        /*Se usa la función pthread_mutex_lock para proteger una zona crítica del código que sería la que está después
        del llamado a esta función, en este caso, la zona critica corresponde a la
        creación de un pthread_cond_wait, se debe usar un lock antes de esto, para que solo un hilo a la vez
        pueda ver la condición la función recibe una variable de tipo pthread_mutex_t y con ella hace lo siguiente:

        -Le asigna a la variable mutex una flag o estado, los cuáles pueden ser LOCKED, UNLOCKED
        -Guarda en el mutex la información del hilo que lo posee
        -Guarda en el mutex una cola de los hilos que están esperando por su uso

        La función como tal hace lo siguiente:

        1. Revisa si el mutex está libre
        2. Si el mutex está libre, se lo asigna al hilo que está ejecutando el programa
        3. Si está ocupado, el hilo se queda esperando hasta que se libere el mutex
        
        La función debe retornar 0 si el proceso se realiza correctamente, en caso de que haya alguna falla,
        retorna un código de error diferente a 0, que es impreso por la función fprintf(), que recibe un código de error,
        dice su código y lo imprime por pantalla traducido usando strerror*/
        if ((r = pthread_mutex_lock (&buf_mutex)) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
        /*Se entra al while siempre que buffers_available sea 0*/
            while (!buffers_available) 
                 /*Función pthread_cond_wait, recibe como parámetros una dirección de memoria a pthread_cond_t y otra a
            pthread_mutex_t, la función pthread_cond_wait realiza lo siguiente:

            1. Desbloquea condition_mutex para que otros hilos puedan entrar y modificar la condición
            2. El hilo que está ejecutando la función, queda en estado de espera
            hasta que se reciba una señal que se guarda en la dirección de condition_cond
            3. Cuando se recibe la señal, se despierta al hilo que estaba esperando
            4. Antes de continuar la ejecución, se vuelve a bloquear condition_mutex*/
                pthread_cond_wait (&buf_cond, &buf_mutex);

            int j = buffer_index; //Guarda en j el valor actual de buffer_index
            buffer_index++; //Incrementa en 1 el valor de buffer_index
            /*Si buffer_index es igual a la cantidad máxima de buffers, le asigna 0 a buffer_index*/
            if (buffer_index == MAX_BUFFERS) 
                buffer_index = 0;
            buffers_available--; //Decrementa en 1 la cantidad de buffers disponibles

            /*Función que escribe en una cadena de char de la siguiente forma:
            
            -1.Parámetros que recibe:
                char *destino: puntero a un arreglo de char donde se guardará el texto, en este caso se le envia el buffer en la posición j
                const char *formato: cadena de formato que especifica cómo se debe formatear la salida, en este caso "Thread %d: %d\n",
                donde %d son marcadores de posición para enteros
                ...: argumentos adicionales que se insertarán en la cadena de formato, se le envian my_id y ++count
                */
            sprintf (buf [j], "Thread %d: %d\n", my_id, ++count);
            lines_to_print++; //Se aumenta lines_to_print en 1

             /*
            Esta función despierta UN hilo que esté esperando en pthread_cond_wait(&cond, &mutex). Si varios hilos están
            esperando, el que despierta NO está definido (lo decide el scheduler, no hay orden garantizado).

            Hace lo siguiente::
            1. Busca hilos bloqueados en pthread_cond_wait sobre cond.
            2. Si hay al menos uno esperando:
            - Despierta a uno.
            - El hilo despertado NO continúa de una vez:
                    * Primero debe volver a adquirir el mutex que se pasó en pthread_cond_wait (esto se hace internamente).
                    Si el mutex sigue tomado por otro hilo, espera.
            3. Si NO hay hilos esperando:
            - La señal se pierde (no se guarda para después).

            Cosas importantes:
            - Esta función NO entrega el mutex al hilo despertado. Solo lo despierta; el hilo debe esperar por el mutex cuando
            intente continuar.

            - No garantiza orden ni prioridad de qué hilo despierta.

            - Debe usarse con un mutex que proteja la condición lógica para evitar condiciones de carrera.*/
            pthread_cond_signal (&spool_cond);

        /*Se usa pthread_mutex_unlock para liberar la zona critica marcada durante el bloque de buf_mutex.
        La función debe retornar 0 si el proceso se realiza correctamente, en caso de que haya alguna falla,
        retorna un código de error diferente a 0, que es impreso por la función fprintf(), que recibe un código de error,
        dice su código y lo imprime por pantalla traducido usando strerror
        */    
        if ((r = pthread_mutex_unlock (&buf_mutex)) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    
        sleep (1); //Da un tiempo de espera
    }
}

/*Función que imprime los mensajes enviados por el buffer*/
void *spooler (void *arg){
    int r; //declara una variable entera r

    /*Ciclo while infinito*/
    while (1) { 
        /*De igual forma que en la función anterior, se usa la función pthread_mutex_lock enviandole la variable
        buf_mutex como parámetro para guardar todo lo relacionado con el funcionamiento del mutex.
        
        Imprime un mensaje de error en caso de que se presente*/ 
        if ((r = pthread_mutex_lock (&buf_mutex)) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
        /*Se entra al while siempre que lines_to_print sea 0*/
            while (!lines_to_print) 

            /*Se usa pthread_cond_wait para dormir al hilo que use este, pero en este caso se usa spool_cond para realizar esta función*/
                pthread_cond_wait (&spool_cond, &buf_mutex);
            /*Imprime el contenido del buffer en la posición buffer_print_index*/
            printf ("%s", buf [buffer_print_index]);
            lines_to_print--; //Decrementa en 1 lines_to_print

            buffer_print_index++; //Incrementa en 1 buffer_print_index
            if (buffer_print_index == MAX_BUFFERS) //Si buffer_print_index es igual a la cantidad máxima de buffers, le asigna 0 a buffer_print_index
               buffer_print_index = 0;

            buffers_available++; //Incrementa en 1 buffers_available
            pthread_cond_signal (&buf_cond); //Despierta a un hilo que esté esperando en buf_cond

        /*Se usa pthread_mutex_unlock para liberar la zona critica marcada durante el bloque de buf_mutex.
        La función debe retornar 0 si el proceso se realiza correctamente, en caso de que haya alguna falla,
        retorna un código de error diferente a 0, que es impreso por la función fprintf(), que recibe un código de error,
        dice su código y lo imprime por pantalla traducido usando strerror
        */   
        if ((r = pthread_mutex_unlock (&buf_mutex)) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }

    }
}
