/**************************************************************************************
 * Fecha: 04/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Sincronización de hilos con Mutex y Variables de Condición (PTHREAD)
 * 
 * Objetivo:
 *    Explicar y demostrar el funcionamiento del mecanismo de sincronización de hilos 
 *    mediante el uso de pthread_mutex y pthread_cond, permitiendo que múltiples 
 *    hilos accedan de forma controlada a recursos compartidos sin generar condiciones 
 *    de carrera.
 *
 * Descripción:
 *    El programa utiliza dos hilos que incrementan un contador global. Sin embargo, uno 
 *    de los hilos debe detener temporalmente su ejecución cuando el contador se encuentra 
 *    dentro de un rango específico. Para lograr esto, se usa un mutex para proteger el 
 *    acceso al contador y una variable de condición para suspender y reanudar la ejecución 
 *    del hilo de forma segura, evitando el consumo innecesario de CPU mientras espera.
 *
 * Aspectos cubiertos:
 *  - Protección de zonas críticas mediante pthread_mutex_lock y pthread_mutex_unlock
 *  - Suspensión de hilos con pthread_cond_wait
 *  - Reanudación de hilos con pthread_cond_signal
 *  - Acceso seguro a variables compartidas entre hilos
 *
 **************************************************************************************/


#include <stdio.h>   //Libreria para entrada y salida estándar
#include <pthread.h> //Libreria para creación y manejo de hilos
#include <stdlib.h>  //Funciones generales del sistema, manejo de memoria, números y salida del programa

/*Se inicializan dos mutex usando las siguientes carácterísticas:

pthread_mutex_t: Es el tipo de variable usado por POSIX para representar un mutex (mutual exclusion lock), el mutex
se utiliza para proteger secciones críticas, asegurando que un solo hilo pueda ejecutar cierta región de código o acceder a cierta estructura
de datos a la vez, evitando que se generen errores durante la ejecución.

El valor asignado a los mutex es PTHREAD_MUTEX_INITIALIZER, que es una inicialización estática del mutex provista
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

-pthread_mutex_lock(&mutex): Bloquea un proceso hasta adquirir el mutex.
-pthread_mutex_trylock(&mutex): Trara de adquirir el mutex, si no puede devuelve EBUSY.
-pthread_mutex_timedlock(&mutex, &abstime): Bloquea el proceso y espera hasta un tiempo límite.
-pthread_mutex_unlock(&mutex): Libera el mutex.*/
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;

/*Se inicializa una variable de condición usando las siguientes características:

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
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;

int count = 0; // Se declara una variable global de tipo entero llamada counter

/*Directiva de preprocesador en C, hace que antes de que el programa compile, cada vez que aparezca
la palabra "NTHREADS" en el código, el preprocesador lo reemplace por 10, es decir, NTHREADS no ocupa memoria,
no crea variables y tampoco existe en tiempo de ejecución, simplemente busca la palabra definida y la reemplaza
por el número dado*/
#define COUNT_DONE 15
#define COUNT_HALT1 4
#define COUNT_HALT2 11

/*Función que realiza un ciclo for infinito hasta que un contador sea mayor o igual a 15, realiza los procesos de
mutex lock, mutex unluck y de cond_wait, dependiendo del valor que tenga un contador*/
void *count01()
{
    /*Se declara un ciclo for sin condiciones, de tal manera se ejecuta infinitamente o hasta que la función en donde se
    está ejecutando produzca un retorno*/
    for (;;)
    {
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
  3. Si está ocupado, el hilo se queda esperando hasta que se libere el mutex*/
        pthread_mutex_lock(&condition_mutex);

        /*Se declara un ciclo while que indicará cuando se debe ejecutar la función pthread_cond_wait,
        este while lo que indica es que mientras el contador declarado globalmente sea mayor o igual a COUNT_HALT1, es decir,
        a 4 y mientras sea menor a COUNT_HATL2, es decir, a 11, debe seguir ejecutando la función*/
        while (count >= COUNT_HALT1 && count <= COUNT_HALT2)
        {
            /*Función pthread_cond_wait, recibe como parámetros una dirección de memoria a pthread_cond_t y otra a
            pthread_mutex_t, la función pthread_cond_wait realiza lo siguiente:

            1. Desbloquea condition_mutex para que otros hilos puedan entrar y modificar la condición
            2. El hilo que está ejecutando la función, queda en estado de espera
            hasta que se reciba una señal que se guarda en la dirección de condition_cond
            3. Cuando se recibe la señal, se despierta al hilo que estaba esperando
            4. Antes de continuar la ejecución, se vuelve a bloquear condition_mutex*/
            pthread_cond_wait(&condition_cond, &condition_mutex);
        }
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
        pthread_mutex_unlock(&condition_mutex);

        /*Se vuelve a realizar un mutex_lock, pero en este caso para el contador, de tal forma solo un hilo puede modificarlo a la vez*/
        pthread_mutex_lock(&count_mutex);
        count++;                                                       // Zona critica protegina por el mutex
        printf("Counter value functionCount ==> 01 <==: %d\n", count); // Se indica el valor que se le dió al contador en esta función (01)
        pthread_mutex_unlock(&count_mutex);                            // se libera el mutex luego de modificar el contador

        /*Si el contador es mayor o igual a COUNT_DONE, es decir, a 15 se realiza el retorno de la función
        finalizando así el ciclo for infinito que había sido declarado anteriormente*/
        if (count >= COUNT_DONE)
            return (NULL);
    }
}

/*Función que realiza un ciclo for infinito hasta que un contador sea mayor o igual a 15, realiza los procesos de
mutex lock, mutex unluck y de cond_signal, dependiendo del valor que tenga un contador*/
void *count02()
{
    /*Se declara un ciclo for sin condiciones, de tal manera se ejecuta infinitamente o hasta que la función en donde se
   está ejecutando produzca un retorno*/
    for (;;)
    {
        /*Se usa la función pthread_mutex_lock para proteger una zona crítica del código que sería la que está después
  del llamado a esta función, en este caso, la zona critica corresponde a la
  creación de un pthread_cond_signal, se debe usar un lock antes de esto, para que solo un hilo a la vez*/
        pthread_mutex_lock(&condition_mutex);
        /*Se declara un condicional que indicará al hilo que esté ejecutando esta función cuando debe producir
        un pthread_cond_signal, en este caso, se produce cuando el contador es menor a 4 y mayor a 11*/
        if (count < COUNT_HALT1 || count > COUNT_HALT2)
        {
            /*
  Esta función despierta UN hilo que esté esperando en
  pthread_cond_wait(&cond, &mutex). Si varios hilos están
  esperando, el que despierta NO está definido (lo decide el
 scheduler, no hay orden garantizado).

  Hace lo siguiente::
  1. Busca hilos bloqueados en pthread_cond_wait sobre `cond`.
  2. Si hay al menos uno esperando:
       - Despierta a uno.
       - El hilo despertado NO continúa de una vez:
           * Primero debe volver a adquirir el mutex que se pasó
             en pthread_cond_wait (esto se hace internamente).
             Si el mutex sigue tomado por otro hilo, espera.
  3. Si NO hay hilos esperando:
       - La señal se pierde (no se guarda para después).

  Cosas importantes:
  - Esta función NO entrega el mutex al hilo despertado.
    Solo lo despierta; el hilo debe esperar por el mutex cuando
    intente continuar.

  - No garantiza orden ni prioridad de qué hilo despierta.

  - Debe usarse con un mutex que proteja la condición lógica
    para evitar condiciones de carrera.*/
            pthread_cond_signal(&condition_cond);
        }
        /*Se usa mutex unlock para liberar el mutex asociado con pthread_cond*/
        pthread_mutex_unlock(&condition_mutex);
        /*Se vuelve a realizar un mutex_lock, pero en este caso para el contador, de tal forma solo un hilo puede modificarlo a la vez*/
        pthread_mutex_lock(&count_mutex);
        count++; //zona crítica protegina por el mutex
        printf("Counter value functionCount ==> 02 <==: %d\n", count); // Se indica el valor que se le dió al contador en esta función (02)
        pthread_mutex_unlock(&count_mutex);  // se libera el mutex luego de modificar el contador
        /*Si el contador es mayor o igual a COUNT_DONE, es decir, a 15 se realiza el retorno de la función
        finalizando así el ciclo for infinito que había sido declarado anteriormente*/
        if (count >= COUNT_DONE)
            return (NULL);
    }
}
/*Función principal donde se realiza toda la operación con hilos y el llamado a las funciones definidas anteriormente*/
void main()
{
    /*Se crean dos variables de tipo pthread_t, que es el tipo de dato que usa pthread para identificar un hilo, almacenando en el los ID
    de todos lo hilos que son creados posteriormente por pthread_create*/
    pthread_t thread1, thread2;

    /*Creacion de dos hilos usanto la función pthread_create. Esta función guarda el ID de un hilo en la dirección de memoria de tipo
        pthread_t, y hace que una función sea ejecutada por esa varaible. Los parámetros que espera la función son los siguientes:
        
        -Dirección de memoria de tipo pthread_t, que guarda el ID del hilo creado en ese momento
        -Valores por defecto que se le van a dar al hilo, como por ejemplo el tamaño de su stack, indicarle que no debe esperar join, etc.
        en este caso se le envia NULL, pues los hilos utilizados en el programa no tienen ningún valor pre-definido.
        -La función que será ejecutada por los hilos, esta función debe retornar un apuntador de memoria y solo debe recibir un parámetro
        que debe ser de tipo genérico (void *arg), pues esto es lo exigido por pthread_create, ya que si la función recibiera mas de un parámetro
        y además no fuera genérico, el programa no podría saber con certeza que debe retornar, ni pthread_join sabria con certeza que debe recibir
        en caso de que sea usado.
        -El valor que se le enviará a la función declarada anteriormente, este valor puede ser de cualquier tipo, pues la función recibe un 
        parámetro genérico, en este caso se le envía NULL, pues las funciones no necesitan ningún argumento.
        
        La fución pthread_create retorna 0 si el hilo fue creado correctamente, sino devuelve un código de error POSIX*/
    pthread_create(&thread1, NULL, &count01, NULL);
    pthread_create(&thread2, NULL, &count02, NULL);
    /*Se ejecuta la función join en todos los hilos creados anteriormente usando pthread_create, pues sin esta los hilos 
    pueden morir antes de ejecutarse.
        
        Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba 
        ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya 
        fueron liberados.
        
        El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
        lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le 
        envía NULL en este parámetro, para que la función no guarde*/
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    exit(0);
}
