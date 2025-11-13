
/**************************************************************************************
 * Fecha: 13/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Lectura de las caracteristicas de directorios mediante funciones en C
 *
 * Objetivo:
 *    Implementar un programa básico que ejemplifique el proceso de lectura de manipulacion e interaccion con directorios,
 * pudiendo comprender carácteristicas básicas de estos como su contenido, su tamaño y los datos que contiene
 *
 * Aspectos cubiertos:
 *  - Variables de tipo DIR
 *  - Funciones estándar de la libreria dirent.h como opendir, readdir, closedir
 *
 *************************************************************************************/

#include <sys/types.h>  // Define tipos de datos especiales del sistema (pid_t, mode_t, off_t, etc.) usados en estructuras del sistema operativo.
#include <sys/stat.h>   // Permite acceder a información sobre archivos (tamaño, permisos, tipo, fechas). Se usa junto a la función stat().
#include <sys/mman.h>   // Permite mapear archivos en memoria o usar memoria compartida. Usada en funciones como mmap() y munmap().
#include <fcntl.h>      // Define constantes y funciones para abrir archivos a bajo nivel (open(), O_RDONLY, O_WRONLY, O_CREAT, etc.).
#include <stdio.h>      // Librería estándar de entrada y salida. Se usa para funciones como printf(), fopen(), fgets(), fclose(), etc.
#include <unistd.h>     // Proporciona acceso a llamadas al sistema POSIX (read(), write(), close(), fork(), getpid(), etc.).
#include <stdlib.h>     // Librería general. Contiene funciones para manejo de memoria, procesos y conversiones numéricas.
#include <string.h>     // Librería para manipular cadenas de caracteres. Incluye funciones como strcpy(), strcmp(), strcat(), strlen().
#include <dirent.h>     // Librería para trabajar con directorios. Permite usar opendir(), readdir(), closedir() y la estructura dirent.


/*Función principal que busca un directorio ingresado por el usuario y imprime por consola su contenido y su tamaño*/
int main()
{

    /*Se crea una variable de apuntador a DIR, esta variable guarda la dirección de un directorio para su posterior uso*/
    DIR *d;
    char nomdir[90], nomfich[90];                      // Se declaran dos cadenas de caracteres de tamaño máxima 90
    struct stat datos;                                 // Se crea una estructura stat llamada datos, que se usa para guardar los datos de un archivo
    struct dirent *direc;                              // Se crea un apuntador a dirent llamdo direc, se usa para leer el contenido de un directorio
    printf("Introduzca el Nombre de un Directorio: "); // Se le pide al usuario el nombre del directorio
    fgets(nomdir, sizeof(nomdir), stdin);              // Se lee por consola el nombre del directorio y se guarda en la cadena *direc
    nomdir[strlen(nomdir) - 1] = '\0';                 /*Eliminamos el \n del Nombre del Fichero*/
    if ((d = opendir(nomdir)) == NULL)
    {                                        // Se usa la función opendir para leer del fichero ingresado por el usuario, su retorno de guarda en la variable d
        printf("El directorio no existe\n"); // En caso de que la función opendir retorne NULL, quiere decir que no se pudo encontrar el directorio, se imprime el mensaje de error
        return -1;
    }

    /*Se crea un ciclo asignando a direc el retorno de la variable readdir, enviandole como parámetro el puntero de tipo DIR, esto indica que siempre que se esté
    leyendo se debe realziar lo solicitado, es decir, siempre que sea diferente a NULL, readdir retorna los datos de los ficheros que se encuentren en el
    directorio enviado como parámetro, y cuando no hay nada que leer devuelve NULL*/
    while ((direc = readdir(d)) != NULL)
    {
        strcpy(nomfich, nomdir);                                                 // Se copia en la cadena nomfich el nombre del directorio
        strcat(nomfich, "/");                                                    // Se concatena a nomfich una barra diagonal para indicar que se está accediendo a un archivo dentro del directorio
        strcat(nomfich, direc->d_name);                                         // Se concatena a nomfich el nombre del archivo que se está leyendo


        /* Se usa la función stat para obtener los datos del archivo, enviandole como parámetros el nombre del archivo y la dirección de la estructura datos
        que guaradrá todos los datos que se encuentren de este archivo */
        stat(nomfich, &datos);                                                  
        if (S_ISREG(datos.st_mode))                                              // Se usa la macro S_ISREG para verificar si el archivo es un archivo regular
            printf("Nombre: %s\t| Tamaño: %ld\n", direc->d_name, datos.st_size); // Se imprime el nombre del archivo y su tamaño en bytes
    } /*Fin del While*/
    closedir(d); // Se cierra el directorio como buena práctica de programación
}

/*


Conclusiones:


El programa anterior demuestra el poder que tiene el lenguaje C para manipular archivos y directorios en el sistema operativo, permitiendo al usuario
interactuar directamente con este desde algún proceso ejecutándose, diferente a la interfaz grafica del sistema operativo
o de la consola de comandos. Incluse permitiendole a otros programas poder manipular los ficheros del sistema operativo.

Esta práctica es muy común en programas que requieren manipular gran cantidad de archivos, como los editores de texto, IDEs, navegadores web, entre otros,
resultando bastante útil para el desarrollo de software que requiera hacer esto.

Resulta importante considerar la ética en este caso, pues el hecho de tener programas con el poder de manipular estos archivos podría conllevar
prácticas malignas como el hacking por ejemplo, por tal motivo, se debe considerar la legalidad en el marco del desarrollo de software.


Hay que recordar cerrar siempre el directorio, para evitar el gasto innecesario de recursos
*/
