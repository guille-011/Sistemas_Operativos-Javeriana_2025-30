/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: IPC con named Semaphores y Sahred Memory
 * Archivo: moduloConsumer.h
 * Descripcion:
 * Define la estructura de datos compartida y los
 *              prototipos de las funciones del módulo consumidor
 *              encargadas de abrir los semáforos y la memoria
 *              compartida, mapear la región de memoria para leer
 *              el búfer y liberar los recursos utilizados.
 * Fecha: 14/11/25
 ******************************************************************/


  /*Se verifica si el identificador __MODULOCONSUMER_H__ ha sido definido antes.
 Sirve para evitar que el contenido de este archivo se incluya varias veces en un mismo programa.
 Si ya estuviera definido, el compilador se saltaría todo el contenido hasta el #endif.*/
#ifndef __MODULOCONSUMER_H__

 /*Se define el identificador __MODULOCONSUMER_H__.
De tal fomra se marca marca que el archivo ya fue incluido una vez, evitando duplicaciones posteriores.
En este espacio se colocan las declaraciones, estructuras, constantes o prototipos de funciones
que se necesitan compartir entre varios archivos .c del proyecto.*/
#define __MODULOCONSUMER_H__

#include <stdio.h>// librería para manejo de entradas y salidas
#include <stdlib.h>// librería para usar exit()
#include <unistd.h>// librería para usar funciones como sleep() y close()
#include <sys/mman.h>// librería para usar PROT_READ | PROT_WRITE, MAP_SHARED
#include <sys/stat.h>// librería para otorgar permisos 0644
#include <fcntl.h>// librería para abrir archivos y usar banderas como O_CREAT, O_RDWR
#include <semaphore.h> // librería que permite el uso de semaforos
#define BUFFER 5   // Tamaño del buffer de la memoria compartida

//Estructura que sirve de plantilla para operar la memoria compartida
typedef struct {
    int salida; 
    int entrada;  
    int bus[BUFFER];   
} compartir_datos;

void abrirSemaforos(sem_t** vacio, sem_t** lleno);
void abrirMemoriaCompartida(int* fd_compartido);
void mapearYConsumir(compartir_datos** compartir, sem_t* vacio, sem_t* lleno, int fd_compartido);
void liberarRecursos(int fd_compartido, sem_t* vacio, sem_t* lleno, compartir_datos* compartir);


#endif