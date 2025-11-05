/**************************************************************************************
 * Fecha: 04/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Creación básica de hilos usando la biblioteca PTHREAD
 * 
 * Objetivo:
 *    Mostrar de forma introductoria el funcionamiento de la interfaz PTHREAD para la
 *    creación, ejecución y sincronización de hilos en un programa en C.
 *
 * Descripción:
 *    El programa crea dos hilos independientes, cada uno encargado de imprimir un 
 *    mensaje distinto. Se utiliza la función pthread_create para iniciar los hilos,
 *    enviando un mensaje como argumento a la función ejecutada por cada hilo.
 *    Finalmente, mediante pthread_join, el programa espera a que ambos hilos
 *    terminen su ejecución antes de finalizar.
 *
 * Aspectos cubiertos:
 *  - Creación de hilos con pthread_create
 *  - Paso de argumentos a funciones de hilo mediante void*
 *  - Conversión/casteo de apuntadores para recibir datos en la función del hilo
 *  - Sincronización de hilos mediante pthread_join
 *  - Uso básico del tipo pthread_t para identificación de hilos
 *
 **************************************************************************************/

 
#include <stdio.h> //Libreria para entrada y salida estándar
#include <pthread.h> //Libreria para creación y manejo de hilos
#include <stdlib.h> //Funciones generales del sistema, manejo de memoria, números y salida del programa


/*Función que será ejecutada por cada hilo creado.La función imprime el argumento que se le pasa.
De tipo puntero, pues este es el formato necesario para enviarla como parámetro en la creación de un hilo.
La función retorna un puntero, de
tal forma puede ser recogido posteriormente por pthread_join y recibe como parámetro un puntero genérico que puede 
apuntar a cualquier tipo de dato*/
void *print_function(void *ptr){ //a la función se le envia void* como parámetro, pues al ser genérico permite enviarle cualquier tipo de dato
    char *mensaje; //variable mensaje de tipo apuntador a char
    mensaje = (char *) ptr; //El dato enviado como parámetro a la función es casteado a char y guardado en la variable mensaje.
    printf("Mensaje desde función: %s\n", mensaje); //Impresión de la variable mensaje declarada anterioemente
    return NULL; //Como la función no debe devolver nada en específico, sino que solo debe indicar que terminó, se agrega el retorno de NULL
}
/*Función principal donde se realiza toda la operación con hilos y el llamado a la función definida anteriormente*/
int main(int argc, char *argv[]){
    /*Se crean dos variables de tipo pthread_t, que es el tipo de dato que usa pthread para identificar un hilo, almacenando en el los ID
    de todos lo hilos que son creados posteriormente por pthread_create*/
    pthread_t hilo1, hilo2;

    
    char *mensaje01 = "Soy el hilo 1"; //se declara mensaje01 como apuntador a char y se le asigna un mensaje
    char *mensaje02 = "Soy el hilo 2"; //se declara mensaje02 como apuntador a char y se le asigna un mensaje

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
    int men01 = pthread_create(&hilo1, NULL, print_function, (void *) mensaje01);
    int men02 = pthread_create(&hilo2, NULL, print_function, (void *) mensaje02);
    /*Se imprime los retornos de los dos llamados a la función pthread_create y el hilo que produjo este llamado
    para veiricar que todo esté bien*/
    printf("=Hilo 1=: %d\n", men01);
    printf("=Hilo 2=: %d\n", men02);

    /*Se agrega la función join en todos los hilos creados anteriormente usando pthread_create, pues sin esta los hilos 
    pueden morir antes de ejecutarse, por eso no se imprimia el mensaje en todas las ejecuciones, cosa que se corrije con esto.
        La función de join, es esperar que todos terminen y poder liberar los recursos utilizados por estos hilos,
        
        Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba 
        ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya 
        fueron liberados.
        
        El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
        lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le 
        envía NULL en este parámetro, para que la función no guarde*/
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

}