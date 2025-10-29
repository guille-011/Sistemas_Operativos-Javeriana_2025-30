/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: Implementación de procesos con fork() y comunicación con pipe()
 * Archivo: modulo_procesos.c
 *
 * 
 * Fecha: 30/10/25
 *
 * Descripción general:
 *   Implementación de las funciones auxiliares utilizadas por el programa
 *   principal (taller_procesos.c):
 *     - leerArchivo: lee N enteros desde un fichero de texto a un arreglo.
 *     - sumarArreglo: suma los elementos de un arreglo de enteros.
 *     - sumarValores: suma dos enteros.
 *     - crearPipe: crea un pipe y maneja errores.
 *
 * Contrato de funciones:
 *   void leerArchivo(char *nombreArchivo, int tam, int *datos);
 *     - Lee exactamente 'tam' enteros del archivo nombreArchivo y los escribe
 *       en el arreglo apuntado por datos.
 *     - En caso de error (abrir/leer), imprime perror(...) y hace exit(EXIT_FAILURE).
 *
 *   int sumarArreglo(int *datos, int tam);
 *     - Retorna la suma (int) de los 'tam' elementos del arreglo apuntado por datos.
 *
 *   int sumarValores(int valor1, int valor2);
 *     - Retorna la suma de los dos enteros.
 *
 *   void crearPipe(int *pipefd);
 *     - Llama a pipe(pipefd) y en caso de error imprime perror(...) y exit(EXIT_FAILURE).
 ******************************************************************/

#include "modulo_procesos.h" //Se incluye el archivo con la declaración de las funciones
#include <stdio.h>  // Entrada y salida estándar (printf, fgets)
#include <stdlib.h> // Funciones generales de utilidad (exit, malloc)
#include <unistd.h> // Funciones de manejo de procesos y pipes (read, write, close)

/*Funcion leerArchivo, se encarga de leer el archivo recibiendo su nombre, tamaño y un arreglo de enteros donde 
se guardará el contenido de este*/
void leerArchivo(char *nombreArchivo, int tam, int *datos) {
  FILE *archivo; //Se declara un puntero de tipo FILE, variable necesaria para el manejo de archivos en C
  /*Se abre el archivo usando la función fopen, esta función recibe como parámetros el nombre del archivo y
  el trato que se le va a dar, ya sea de lectura o escritura.
  
  En este caso, se le envia el mismo nombre recibido como parámetro en la función leerArchivo y "r", para
  indicar que se quiere leer el archivo en modo de lectura. La función retorna un apuntador a FILE, este retorno
  se guarda en la variable archivo declarada anteriormente*/

  archivo = fopen(nombreArchivo, "r"); 

  /*Si la función fopen retorna NULL, es porque hubo un error abriendo el archivo, por lo tanto si pasa esto
  se imprime que hubo un error abriendo el archivo y se sale del programa indicando que huo un error*/
  if (archivo == NULL) {
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }

  /*Si el archivo se abrió correctamente, se hace un ciclo desde 0 hasta el tamaño enviado como parámetro,
  correspondiente a la cantidad de elementos que hay en el archivo*/
  for (int i = 0; i < tam; i++) {

    /*se utiliza la función fscanf, que es una función de la biblioteca stdio.h, encargada de leer datos formateados desde
    un archivo.
    
    Esta función espera los siguientes parámetros:
    Primero el apuntador a archivo desde donde se están leyendo los datos
    Segundo el indicador de tipo de datos que se leerán desde el archivo,
    en este caso es %d, correspondiente a enteros
    La variable donde se guardará lo leído desde el archivo, en este caso
    se le envia el arreglo datos, siendo iterado en i, para que se guarden todos
    los elementos del archivo.
    
    
    La función retorna la cantidad de elementos leídos, en este caso lo 
    correcto sería leer 1, pues solo tiene un %d, de tal forma que lee
    uno por uno los elementos del archivo.
    De tal forma, si la función retorna un valor diferente a 1, se abandona el programa con 
    un mensaje de error*/
    if (fscanf(archivo, "%d", datos + i) != 1) {
      perror("Error al leer el archivo");
      fclose(archivo);
      exit(EXIT_FAILURE);
    }
  }

  fclose(archivo); //Se cierra el archivo cuando se ha terminado todo el proceso
}


/*Funcion sumarArreglo, que recibe un apuntador a entero y la cantidad de elementos contenidas 
en el arreglo representado con este apuntador*/
int sumarArreglo(int *datos, int tam) {
  int suma = 0; //inicializa la variable suma en 0
  for (int i = 0; i < tam; i++) { //Ciclo desde 0 hasta el tamaño del arreglo
    suma += *(datos + i); //Se recorre el arreglo como apuntador y se va sumando cada elemento a la variable suma
  }
  return suma; //Se retorna la variable suma
}

/*Funcion sumarValores que recibe dos enteros como parámetro y retorna su suma*/
int sumarValores(int valor1, int valor2) {
  return valor1 + valor2; //Retorna la suma de los dos enteros recibidos como parámetro
}

/*Funcion crearPipe que crea un pipe en un apuntador a entero enviado como parámetro*/
void crearPipe(int *pipefd) {
  /*Se usa la función pipe para crear el pipe en el apuntador enviado como parámetro, si esta
  función retorna -1 es porque ocurrió un error al momento de crear el pipe, por lo tanto se imprime un mensaje de 
  error y se cierra el programa*/
  if (pipe(pipefd) == -1) { 
    perror("Error al crear el pipe");
    exit(EXIT_FAILURE);
  }
}