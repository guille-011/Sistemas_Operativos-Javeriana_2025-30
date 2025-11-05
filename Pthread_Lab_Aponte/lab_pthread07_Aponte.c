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
 *    Actualmente, los hilos acceden a la variable global 'valor' sin ningún mecanismo de
 *    sincronización activo. Esto genera una race condition: el hilo 'reportar' podría
 *    ejecutarse antes, después o durante la modificación de 'valor' por el hilo 'asignar',
 *    provocando resultados impredecibles (podría imprimir 100 o 20 dependiendo del orden
 *    de ejecución de los hilos).
 *
 *    El código incluye un mutex 'bloqueoCC' y una variable de condición 'condiCC', que
 *    están comentados, y que podrían ser usados para proteger la sección crítica y
 *    notificar a 'reportar' cuando 'valor' ha sido actualizado, eliminando la race condition.
 *
 * Aspectos cubiertos:
 *  - Creación de hilos con pthread_create
 *  - Espera de finalización de hilos con pthread_join
 *  - Introducción a race conditions en variables compartidas
 *  - Uso potencial de mutex (pthread_mutex_t) para proteger secciones críticas
 *  - Uso potencial de variables de condición (pthread_cond_t) para sincronización
 *
 * Race condition identificada:
 *  - La variable global 'valor' se lee y se escribe desde dos hilos distintos sin
 *    protección. Esto significa que 'reportar' podría imprimir un valor inconsistente
 *    si ejecuta mientras 'asignar' está modificando 'valor'.
 *  - La solución sería descomentar el uso de pthread_mutex_lock/unlock y
 *    pthread_cond_wait/signal para garantizar que 'reportar' solo lea 'valor' después
 *    de que 'asignar' lo haya actualizado.
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
    /*
    pthread_mutex_lock(&bloqueoCC);
    while(!notidicar){
        pthread_cond_wait(&condiCC, &bloqueoCC);
    }
    */
    printf("\n\tEl valor es: %d\n", valor); //Impresión de valor
    //pthread_mutex:unlock(&bloqueoCC);
    return NULL; // Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se retorna NULL, es decir, un puntero vacío
}


/*Función asignar que cambia el valor declarado como entero global a 20*/
void *asignar(void *nousada){
    valor = 20; //Se cambia el valor de valor = 100 a valor = 20
    /*
    pthread_mutex_lock(&bloqueoCC);
    notificar = true;
    pthread_cond_signal(&condiCC);
    pthread_mutex_unlock(&bloqueoCC);
    */

    return NULL; // Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se retorna NULL, es decir, un puntero vacío
}

/*Función principal donde se realiza toda la operación con hilos y el llamado a la función definida anteriormente*/
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

