/*********************************************************
*Pontificia Universidad Javeriana
*Autor: Guillermo Aponte
*Fecha_ 09/10/2025
*Materia: Sistemas Operativos
*Tema: Creación de procesos fork
*  -identificación del proceso padre e hijo 
*********************************************************/


#include <stdio.h> //Libreria implementada para usar funciones de entrada y salida como printf
#include <stdlib.h> //Funciones generales como exit(), malloc(), etc.
#include <unistd.h> //Libreria POSIX: contiene fork(), getpid(), getppid()
#include <string.h> //Manejo de cadenas de caracteres

/*El siguiente código representa un ejemplo del uso de la función fork(), la cuál se usa prar crear un nuevo proceso en ejecución
el proceso hijo retorna 0, mientras que el proceso padre, retorna el ID de su hijo, de tal forma se pueden diferenciar ambos procesos*/

int main(){
    /*Se invoca la función fork(), la cuál sirve para la creación de un proceso, se guarda su valor de retorno en una variable procesoID
    de tipo entero, pues esto es lo que retorna fork().
    Los posibles retornos de fork son:
    0, proceso hijo
    > 0, proceso padre, el valor es el PID del hijo
    -1, error de creación */
    int procesoID = fork();


    /*Conficional para diferenciar si se encuentra en el proceso hijo o en el proceso padre
    en memoria, después de llamar a la función fork(), ambos procesos tiene una copia exacta del espacio de direcciones
    pero cambia el valor de procesoID dependiendo del proceso en el que se encuentra el programa*/
    if(procesoID == 0){
        /*Solo se imprime por pantalla cuando se encuentra en el proceso hijo, se usa %d, pues getpid() devuelve un entero correspondiente al id del proceso*/
        printf("Soy el proceso hijo, mi ID es: %d \n\n", getpid()); 
    }else if(procesoID > 0){
         /*Solo se imprime por pantalla cuando se encuentra en el proceso padre, se usa %d, pues getpid() devuelve un entero correspondiente al id del proceso*/
        printf("Soy el proceso padre, mi ID es: %d \n\n", getpid());
    }else{
        printf("Error de llamda FORK() \n"); //se añade un condicional para evidenciar si el proceso es menor a 0, si sucede esto es porque se evidencia un error de creación
        exit(EXIT_FAILURE); //se termina el programa con un código de error
    }
    /*printf ejecutado por ambos procesos, pues no se indica en cuál se debe ejecutar
    se imprime dos veces, una desde el proceso hijo y otra desde el padre */
    printf("Fin del programa \n");



    return 0; //ambos procesos retornan al sistema operativo 0, indicando que todo salió bien
}