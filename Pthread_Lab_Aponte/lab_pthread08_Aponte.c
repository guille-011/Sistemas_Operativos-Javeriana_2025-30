/**************************************************************************************
 * Fecha: 04/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Sincronización de hilos usando Mutex y Variables de Condición (PTHREAD)
 *
 * Objetivo:
 *    Demostrar la creación y ejecución de múltiples hilos utilizando la interfaz PTHREAD,
 *    y exponer cómo las race conditions pueden ocurrir al acceder y modificar variables
 *    compartidas sin protección.
 *
 * Descripción:
 *    El programa crea dos hilos:
 *      - Hilo 'asignar': cambia el valor de la variable global 'valor' de 100 a 20.
 *      - Hilo 'reportar': imprime el valor de la variable global 'valor'.
 *
 *    La race condition ocurre porque ambos hilos acceden a 'valor' sin sincronización:
 *    'reportar' podría ejecutarse antes, después o durante la modificación de 'valor' 
 *    por 'asignar', provocando resultados impredecibles.
 *
 * Corrección de la race condition:
 *    - Se utiliza un mutex 'bloqueoCC' para proteger la sección crítica donde se lee o 
 *      escribe 'valor'.
 *    - 'reportar' adquiere el mutex y verifica si 'valor' está listo mediante la variable 
 *      de condición 'notificar'. Si no lo está, espera con pthread_cond_wait, liberando 
 *      el mutex temporalmente hasta recibir señal.
 *    - 'asignar' adquiere el mutex, cambia 'valor' a 20, establece 'notificar = true' y 
 *      envía pthread_cond_signal para despertar a 'reportar'.
 *    - De esta forma, se garantiza que 'reportar' solo lea 'valor' después de que 'asignar' 
 *      lo haya modificado, eliminando la race condition.
 *
 * Aspectos cubiertos:
 *  - Creación de hilos con pthread_create
 *  - Espera de finalización de hilos con pthread_join
 *  - Uso de mutex (pthread_mutex_t) para proteger secciones críticas
 *  - Uso de variables de condición (pthread_cond_t) para sincronización
 *  - Identificación y corrección de race conditions
 *
 **************************************************************************************/


#include <stdio.h>   //Libreria para entrada y salida estándar
#include <pthread.h> //Libreria para creación y manejo de hilos
#include <stdlib.h>  //Funciones generales del sistema, manejo de memoria, números y salida del programa
#include <stdint.h> //Libreria para trabajar con enteros de tamaño fijo
#include <stdbool.h> //libreria que incluye las variables de tipo booleano

int valor = 100; //Se declara una variable global llamada valor y se le da el valor de 100
bool notificar = false; //Se declara un booleano global llamado notificar y se le da el valor de false


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

- pthread_cond_wait(&condiCC, &mutex):
  Hace que el hilo espere hasta que ocurra una señal. Mientras espera, el mutex asociado
  se libera automáticamente y se vuelve a adquirir cuando la condición se cumple.

- pthread_cond_signal(&condiCC):
  Despierta a un hilo que esté esperando en la variable de condición.

- pthread_cond_broadcast(&condiCC):
  Despierta a *todos* los hilos que estén esperando en la condición.

Notas importantes:
- Siempre debe usarse junto con un mutex para evitar race conditions.
- Las variables de condición no guardan un estado por sí mismas; dependen de una condición
  lógica definida en el programa y del mutex asociado.
*/
pthread_cond_t condiCC = PTHREAD_COND_INITIALIZER;


/*Función reportar que imprime el valor declarado como entero global*/
void *reportar(void *nousada){
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
    /*Se declara un ciclo while que indicará cuando se debe ejecutar la función pthread_cond_wait,
        este while lo que indica es que mientras el booleano notificar declarado globalmente
        mantenga un valor de false, debe seguir ejecutando la función*/
    while(!notificar){
        /*Función pthread_cond_wait, recibe como parámetros una dirección de memoria a pthread_cond_t y otra a
            pthread_mutex_t, la función pthread_cond_wait realiza lo siguiente:

            1. Desbloquea condition_mutex para que otros hilos puedan entrar y modificar la condición
            2. El hilo que está ejecutando la función, queda en estado de espera
            hasta que se reciba una señal que se guarda en la dirección de condition_cond
            3. Cuando se recibe la señal, se despierta al hilo que estaba esperando
            4. Antes de continuar la ejecución, se vuelve a bloquear condition_mutex*/
        pthread_cond_wait(&condiCC, &bloqueoCC);
    }
    
    printf("\n\tEl valor es: %d\n", valor); //Impresión de valor
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
    return NULL; // Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se retorna NULL, es decir, un puntero vacío
}

/*Función asignar que cambia el valor declarado como entero global a 20*/
void *asignar(void *nousada){
    valor = 20; //Se cambia el valor de valor = 100 a valor = 20

    /*Se usa la función pthread_mutex_lock para proteger una zona crítica del código que sería la que está después
  del llamado a esta función, en este caso, la zona critica corresponde a la
  creación de un pthread_cond_signal, y el cambio de notificar a true se debe usar un lock antes de esto, para que solo un hilo a la vez*/
    pthread_mutex_lock(&bloqueoCC);
    notificar = true; //se cambia el valor de notiicar a true

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
    pthread_cond_signal(&condiCC);

    /*Se usa mutex unlock para liberar el mutex asociado con la condicion*/
    pthread_mutex_unlock(&bloqueoCC);
    

    return NULL; // Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se retorna NULL, es decir, un puntero vacío
}
/*Función principal donde se realiza toda la operación con hilos y el llamado a las funciones definidas anteriormente*/
int main(){
    /*Se crean dos variables de tipo pthread_t, que es el tipo de dato que usa pthread para identificar un hilo, almacenando en el los ID
    de todos lo hilos que son creados posteriormente por pthread_create*/
    pthread_t reporte, asigne;
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
        parámetro genérico, en este caso se le envía el casteo a valor genérico de los mensajes declarados anteriormente.
        
        La fución pthread_create retorna 0 si el hilo fue creado correctamente, sino devuelve un código de error POSIX*/
    pthread_create(&reporte, NULL, &reportar, NULL);
    pthread_create(&asigne, NULL, &asignar, NULL);


    /*Se usa la función join en todos los hilos creados anteriormente usando pthread_create, pues sin esta los hilos 
    pueden morir antes de ejecutarse, por eso no se imprimia el mensaje en todas las ejecuciones, cosa que se corrije con esto.
        La función de join, es esperar que todos terminen y poder liberar los recursos utilizados por estos hilos,
        
        Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba 
        ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya 
        fueron liberados.
        
        El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
        lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le 
        envía NULL en este parámetro, para que la función no guarde*/
    pthread_join(reporte, NULL);
    pthread_join(asigne, NULL);
    
    return 0;
}

