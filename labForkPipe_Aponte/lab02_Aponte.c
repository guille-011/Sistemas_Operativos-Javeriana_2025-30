/*********************************************************
*Pontificia Universidad Javeriana
*Autor: Guillermo Aponte
*Fecha_ 09/10/2025
*Materia: Sistemas Operativos
*Tema: Creación de procesos fork
*  -identificación del proceso padre e hijo 
    -Procesos duplicados
*********************************************************/


#include <stdio.h> //Libreria implementada para usar funciones de entrada y salida como printf
#include <stdlib.h> //Funciones generales como exit(), malloc(), etc.
#include <unistd.h> //Libreria POSIX: contiene fork(), getpid(), getppid()
#include <string.h> //Manejo de cadenas de caracteres


int main(){
    //mensaje antes de la creación del proceso, se ejecutará solo una vez
    printf("\n\nPrograma principal \n");

    /*Se invoca la función fork(), la cuál sirve para la creación de un proceso, se guarda su valor de retorno en una variable procesoID
    de tipo entero, pues esto es lo que retorna fork().
    Los posibles retornos de fork son:
    0, proceso hijo
    > 0, proceso padre, el valor es el PID del hijo
    -1, error de creación */
    int procesoID = fork();


    /*Conficional para diferenciar si se encuentra en el proceso hijo o en el proceso padre
    en memoria, después de llamar a la función fork(), ambos procesos tiene una copia exacta del espacio de direcciones
    pero cambia el valor de procesoID dependiendo del proceso en el que se encuentra el programa.
    Según el valor de procesoID que se tenga en el programa realizaŕa instreucciones diferentes*/
    if(procesoID == 0){
        /*************************************
         *          PROCESO HIJO            *
         *************************************/
        printf("\nSoy el proceso hijo, mi ID es: %d \n\n", getpid());
        /*Si se encuentra en el proceso hijo, se imprimen los números del 0 al 5 de manera ascendente, indicando que el valor se está imprimiendo
        desde el proceso hijo*/
    for(int i = 0; i <= 5; i++){
        printf("Valor Hijo: ==# %d #==\n", i);

    }

    }else if(procesoID > 0){
        /*************************************
         *          PROCESO PADRE           *
         *************************************/

        printf("Soy el proceso padre, mi ID es: %d \n\n", getpid());
        
        /*Si se encuentra en el proceso padre, se imprimen los números del 5 al 0 de manera Descendente, indicando que el valor se está imprimiendo
        desde el proceso padre*/
    for(int i = 5; i >= 0; i--){
        printf("Valor Padre: ==# %d #==\n", i);
    }   
    } else{
        /*************************************
         *      ERROR EN CREACIÓN FORK      *
         *************************************/
        printf("Error de llamada FORK() \n"); //si el valor de procesoID es menor a 0, se evidencia un error en la creación del proceso, por lo tanto se imprime un mensaje de error
        exit(EXIT_FAILURE); //se termina el programa con un código de error
    }

    /*printf ejecutado por ambos procesos, pues no se indica en cuál se debe ejecutar
    se imprime dos veces, una desde el proceso hijo y otra desde el padre */

    printf("Fin del programa \n");


    return 0;  //ambos procesos retornan al sistema operativo 0, indicando que todo salió bien
}