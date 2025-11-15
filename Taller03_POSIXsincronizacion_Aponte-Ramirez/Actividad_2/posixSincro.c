/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: MUTEXES y COND VARIABLES con POSIX
 * Archivo: posixSincro.c
 * Descripcion: Programa principal que crea y coordina los hilos 
 *              productores y el hilo spooler, inicializando los 
 *              índices del búfer, lanzando los hilos mediante 
 *              pthread_create, esperando su finalización con 
 *              pthread_join y gestionando la cancelación controlada 
 *              del hilo spooler cuando ya no quedan líneas por 
 *              imprimir en el búfer compartido.
 * Fecha: 14/11/25
 ******************************************************************/
#include "moduloSincro.h"

/*Función principal donde se realiza toda la operación con hilos y el llamado a las funciones definidas anteriormente*/
int main (int argc, char **argv){

    /*Se crean dos variables de tipo pthread_t, un arreglo de tamaño 10 y una variable normal
    que es el tipo de dato que usa pthread para identificar un hilo, almacenando en el los ID
    de todos lo hilos que son creados posteriormente por pthread_create*/
    pthread_t tid_producer [10], tid_spooler;
    int i, r; //Se decalran dos variables enteras i y r

    /*Se inicializan las variables buffer_index y buffer_print_index en 0*/
    buffer_index = buffer_print_index = 0;

    /*Creacion de dos hilos usando la función pthread_create. Esta función guarda el ID de un hilo en la dirección de memoria de tipo
        pthread_t, y hace que una función sea ejecutada por esa varaible. Los parámetros que espera la función son los siguientes:
        
        -Dirección de memoria de tipo pthread_t, que guarda el ID del hilo creado en ese momento
        -Valores por defecto que se le van a dar al hilo, como por ejemplo el tamaño de su stack, indicarle que no debe esperar join, etc.
        en este caso se le envia NULL, pues los hilos utilizados en el programa no tienen ningún valor pre-definido.
        -La función que será ejecutada por los hilos, esta función debe retornar un apuntador de memoria y solo debe recibir un parámetro
        que debe ser de tipo genérico (void *arg), pues esto es lo exigido por pthread_create, ya que si la función recibiera mas de un parámetro
        y además no fuera genérico, el programa no podría saber con certeza que debe retornar, ni pthread_join sabria con certeza que debe recibir
        en caso de que sea usado, en este caso se le envia la función spooler.
        -El valor que se le enviará a la función declarada anteriormente, este valor puede ser de cualquier tipo, pues la función recibe un 
        parámetro genérico, en este caso se le envía NULL, pues las funciones no necesitan ningún argumento.
        
        La fución pthread_create retorna 0 si el hilo fue creado correctamente, sino devuelve un código de error POSIX
        
        Se guarda su retorno en la variable r, para verificar si hubo algún error en la creación del hilo, en caso de que r
        sea diferente de 0, se imprime un mensaje de error por pantalla usando la función fprintf
        */
    if ((r = pthread_create (&tid_spooler, NULL, spooler, NULL)) != 0) {
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
    }
    int thread_no [10]; //Se declara un arreglo llamado thread_no de tamaño 10

    /*Se crea un ciclo for desde 0 a 10 para realizar el llamado repetido de la función pthread_create.
    
    En este caso se le envia como dirección de memoria todas las posicones de la variable tid_producer, de tal forma que se cree un hilo por cada una de estas
    Se le envia también la función producer, enviandole como parámetro la dirección de memoria de cada posición del arreglo thread_no, que contiene el número del
    hilo que se está creando en ese momento.

    Se verifican errores igual a como se hacia anteriormente.
    */
    for (i = 0; i < 10; i++) {
        thread_no [i] = i;
        if ((r = pthread_create (&tid_producer [i], NULL, producer, (void *) &thread_no [i])) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    }

    /*Se utiliza la función join iterando en todos los hilos creados anteriormente usando pthread_create.
    La función de join, es esperar que todos terminen y poder liberar los recursos utilizados por estos hilos,

    Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba
    ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya
    fueron liberados.

    El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
    lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le
    envía NULL en este parámetro, para que la función no guarde.
    
    Se verifican errores nuevamente*/
    for (i = 0; i < 10; i++)
        if ((r = pthread_join (tid_producer [i], NULL)) == -1) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    
    /*Se espera a que lines_to_print sea 0, dando un tiempo de pausa para que el proceso avance*/
    while (lines_to_print) sleep (1);

    /*Una vez se cumple que no hay mas líneas para escribir, se le envia una al sistema de que detenga la ejecución de
    tid_spooler la función pthread_cancel, este solicitud será atendida cuando el hilo llegue a un punto de cancelación, que en este caso
    es cuando el hilo intente adquirir el mutex en la siguiente iteración del while infinito.

    Se evalúan errores como es debido
    */
    if ((r = pthread_cancel (tid_spooler)) != 0) {
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
    }

    exit (0);
}

