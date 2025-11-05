/**************************************************************************************
 * Fecha: 04/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Uso de interfaz de creación de hilos PTHREAD
 * Objetivo:
 *    Implementar un programa básico que ejemplifique el uso de la biblioteca PTHREAD
 *    para la creación y manejo de múltiples hilos en un sistema concurrente.
 * 
 * Descripción:
 *    El programa crea N hilos (donde N es ingresado por el usuario). Cada hilo ejecuta
 *    una función que recibe un argumento, lo procesa e imprime su identificación.
 *    Posteriormente, el programa principal espera a que todos los hilos finalicen para
 *    garantizar la correcta sincronización del flujo de ejecución.
 *
 * Aspectos cubiertos:
 *  - Manejo de argumentos desde consola (argc, argv)
 *  - Conversión de cadenas a enteros (atoi)
 *  - Creación de hilos mediante pthread_create
 *  - Paso de parámetros a funciones ejecutadas por hilos
 *  - Unión de hilos mediante pthread_join
 *  - Uso básico del tipo pthread_t
 *  - Buenas prácticas en ejecución concurrente
 *
 *************************************************************************************/

 
#include <stdio.h> //Libreria para entrada y salida estándar
#include <pthread.h> //Libreria para creación y manejo de hilos
#include <stdlib.h> //Funciones generales del sistema, manejo de memoria, números y salida del programa


/*Función que será ejecutada por cada hilo creado. La función imprime el argumento que se le pasa.
De tipo puntero estático, pues este es el formato necesario para enviarla como parámetro en la creación de un hilo,
la función tiene la claúsula static, pues esto indica que solo puede ser llamada por el archivo donde se encuentra,
no por otros archivos del programa, esto en caso de que se quiera aplicar modularidad. La función retorna un puntero, de
tal forma puede ser recogido posteriormente por pthread_join y recibe como parámetro un puntero genérico que puede 
apuntar a cualquier tipo de dato*/
static void *funcion(void *arg){  //a la función se le envia void* como parámetro, pues al ser genérico permite enviarle cualquier tipo de dato
    int tarea = *((int *) arg); //El dato enviado como parámetro a la función es casteado a entero y guardado en la variable tarea.
    printf("Tarea: -%d -\n", tarea); //Se imprime el entero tarea, que contiene el valor pasado como parámetro
    return NULL;//Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se retorna NULL, es decir, un puntero vacío
}

/*Función principal donde se realiza toda la operación con hilos y el llamado a la función definida anteriormente*/
int main(int argc, char *argv[]){ //El main espera que se le ingresen varios argumentos al ejecutarse
    if(argc < 2){ //Si se envian menos de 2 argumentos (La cantidad de hilos y el ejecutable), se imprime un mensaje de error
        printf("\t $./ejecutable NUM \n"); //Mensaje de error
        exit(0); //Salida del programa en caso de error
    }

    int N = (int) atoi(argv[1]); //Se declara el entero N y se le envia la conversión a entero del valor enviado como parámetro al main por el usuario

    /*Se crea un arreglo de tamaño N ingresado por el usuario, que guardará el total de tareas que se desea ejecutar en el programa*/
    int tareas[N];
    /*Se crea un arreglo de tipo pthread_t, que es el tipo de dato que usar pthreads para identificar un hilo, almacenando en el los ID
    de todos lo hilos que son creados posteriormente por pthread_create*/
    pthread_t hilos[N];


    /*Bucle que ejecuta una cantidad N, ingresada por el usuario en una cantidad N de hilos, creados con la función pthread_create*/
    for(int i =0; i < N; i++){
        tareas[i] = i; //Se guarda en la posición iterada del arreglo el valor entero de esta misma posición
        /*Creacion de N hilos usanto la función pthread_create. Esta función guarda el ID de un hilo en un arreglo de tipo
        pthread_t, y hace que una función sea ejecutada por ese arreglo. Los parámetros que espera la función son los siguientes:
        
        -Dirección de memoria de tipo pthread_t, que guarda el ID del hilo creado en ese momento
        -Valores por defecto que se le van a dar al hilo, como por ejemplo el tamaño de su stack, indicarle que no debe esperar join, etc.
        en este caso se le envia NULL, pues los hilos utilizados en el programa no tienen ningún valor pre-definido.
        -La función que será ejecutada por los hilos, esta función debe retornar un apuntador de memoria y solo debe recibir un parámetro
        que debe ser de tipo genérico (void *arg), pues esto es lo exigido por pthread_create, ya que si la función recibiera mas de un parámetro
        y además no fuera genérico, el programa no podría saber con certeza que debe retornar, ni pthread_join sabria con certeza que debe recibir
        en caso de que sea usado
        -El valor que se le enviará a la función declarada anteriormente, este valor puede ser de cualquier tipo, pues la función recibe un 
        parámetro genérico, en este caso se le envía el valor númerico que se está iterando y guardando en el arreglo de tareas.*/
        pthread_create(&hilos[i], NULL, funcion, tareas+i);
    }
    /*Ciclo para la unión de los hilos: esperar a que todos terminen*/
    for(int i =0; i < N; i++){
        /*Se utiliza la función join iterando en todos los hilos creados anteriormente usando pthread_create.
        La función de join, es esperar que todos terminen y poder liberar los recursos utilizados por estos hilos,
        
        Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba 
        ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya 
        fueron liberados.
        
        El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
        lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le 
        envía NULL en este parámetro, para que la función no guarde*/
        pthread_join(hilos[i], NULL);
    }


    return 0;
}