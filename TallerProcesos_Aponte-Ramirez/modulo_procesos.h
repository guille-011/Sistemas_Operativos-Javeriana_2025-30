/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: Implementación de procesos con fork() y comunicación con pipe()
 * Archivo: modulo_procesos.h
 *
 * 
 * Fecha: 30/10/25
 *
 *Descripción general:
 *   Declaraciones de las funciones auxiliares usadas por el programa
 *   principal (taller_procesos.c):
 *     - leerArchivo: lectura de archivos de enteros a un arreglo.
 *     - sumarArreglo: suma de elementos de un arreglo.
 *     - sumarValores: suma de dos enteros.
 *     - crearPipe: crear un pipe y gestionar errores.
 *
 * Guardas de inclusión para evitar múltiples inclusiones del header.
 ******************************************************************/


 /*Se verifica si el identificador __MODULO_PROCESOS_H__ ha sido definido antes.
 Sirve para evitar que el contenido de este archivo se incluya varias veces en un mismo programa.
 Si ya estuviera definido, el compilador se saltaría todo el contenido hasta el #endif.*/
 #ifndef __MODULO_PROCESOS_H__

 /*Se define el identificador __MODULO_PROCESOS_H__.
De tal fomra se marca marca que el archivo ya fue incluido una vez, evitando duplicaciones posteriores.
En este espacio se colocan las declaraciones, estructuras, constantes o prototipos de funciones
que se necesitan compartir entre varios archivos .c del proyecto.*/
 #define __MODULO_PROCESOS_H__

 /*Se crea la función leerArchivo, que recibe el nombre de un archivo a leer, la cantidad de datos en este archivo y un apuntador a entero
 que guarda los datos*/
void leerArchivo(char* nombreArchivo, int tam, int *datos); 
/*Se declara la funcion sumaArreglo, que recibe un arreglo de enteros y el tamaño del arreglo, retorna la suma de los datos
contenidos en el arreglo*/
int sumarArreglo(int *datos, int tam);
/*Se declara la funcion sumarValores que recibe dos enteros y retorna el resultado de su suma*/
int sumarValores(int valor1, int valor2);
/*Se crea la funcioón crearpipe que recibe un apuntador a entero en el que se creará el pipe*/
void crearPipe(int *pipefd);

 #endif //Se cierra la guarda de inclusión 