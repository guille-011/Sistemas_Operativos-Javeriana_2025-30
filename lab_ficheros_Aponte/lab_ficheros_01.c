
/**************************************************************************************
 * Fecha: 13/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Lectura del contenido de un fichero
 *
 * Objetivo:
 *    Implementar un programa básico que ejemplifique el proceso de lectura de un archivo, usando variables de tipo
 * FILE y funciones estándar de C para la manipulación de archivos.
 *
 * Aspectos cubiertos:
 *  - Variables de tipo FILE
 *  - Funciones estándar de lectura, fopen, fgets, fclose
 *
 *************************************************************************************/





#include <stdio.h>      // Librería estándar de entrada y salida. Permite usar funciones como printf(), scanf(), fopen(), fgets(), fclose(), etc.
#include <stdlib.h>     // Librería estándar general. Permite usar funciones como malloc(), free(), atoi(), exit(), y system().

/*Función principal, lee de un archivo e imprime su lectura*/
int main(int argc, char *argv[]) {
    
    char *nombrefichero = "../readme.txt"; //Se guarda en la cadena *nombrefichero el nombre del archivo "readme.txt"
    
  
  /*Se abre el archivo usando la función fopen, esta función recibe como parámetros el nombre del archivo y
  el trato que se le va a dar, ya sea de lectura o escritura.
  */

    FILE *fp = fopen(nombrefichero, "r"); //Se crea una variable apuntador de tipo file llamada fp y se le asigna el retorno de la función fopen
					//Este retorno representa una señal al compilador que le indica que tipo de entrada debe usar, en este caso el archivo
					//Se le envia "r" para indicarle que se abrirá en modo lectura

	/*Si hay un error al abrir el archivo, fopen devuelve NULL, en este caso se imprime un mensaje de error y se sae del programa*/
    if(fp == NULL){
        printf("Error: No se puede abrir el fichero\n");
        return 1;
    }

	/*Se imprime por pantalla que se leerá del fichero readme.txt*/
    printf("\n===============================\n");
    printf("\n Lectura de ficheros readme.txt\n");
    printf("\n===============================\n");
    
    //Lectura de fichero, máximo 256 bytes por línea
    //Lectura de línea por línea (buscar \n)

    const unsigned MAX_LENGHT = 256; //Se declara el tamaño maximo que se leerá, 256
    char buffer[MAX_LENGHT]; //Se declara la cadena de char que leerá del fichero, dandole el tamaño máximo declarado anteriormente


	/*se utiliza la función fgets, que es una función de la biblioteca stdio.h, encargada de leer datos formateados desde
    un archivo.
    
    Esta función espera los siguientes parámetros:
    Primero la cadena donde se guardará lo leído desde el archivo, en este caso
    Segundo el tamaño de dicha cadena que se leerá
    Por último se le envia el apuntador al archivo desde donde se leerá, en este caso fp
    
    
    Se realiza en un ciclo while, de tal forma que se ejecutará siempre que haya algo por leer en el archivo*/
    while(fgets(buffer, MAX_LENGHT, fp)){
        printf("%s", buffer); //Se imprime lo que se lee en la cadena buffer
    }

    fclose(fp); //se cierra el archivo como buena practica de programación
    return 0;
}

/*

Conclusiones:


En el archivo anterior se pudo evidenciar como se realiza la lectura de los archivos en lenguaje C, notándose también que esta lectura es bastante similar a la lectura desde 
consola, pues de igual forma se usa la función fgets, pero en este caso se cambian los parámetros, indicándole que se debe leer desde el archivo, siendo un proceso bastante simple
para el caso de los archivos de texto


El uso de archivos de texto y su manipulación desde el lenguaje de programación resulta útil cuando se quieren preservar datos en la memoria secundaría, pues hay aplicaciones 
de programación que requieren de esto, en las que mantener datos durante la ejecución no resulta suficiente, un ejemplo de esto podría ser el proyecto del curso, donde se deben 
manipular diferentes datos desde archivos de texto, cosa que no se podría hacer durante la ejecución simple, pues estos datos se guardan en la memoria principal,
que se borra una vez termina de ejecutarse el proceso.

Es importante también cerrar SIEMPRE el archivo, para evitar gasto posterior de recursos y tener en cuenta cuanto lee este archivo 
*/
