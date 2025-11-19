/***************************************************************
 * Universidad: Pontificia Universidad Javeriana
 * Carrera: Ingeniería de Sistemas
 * Autor: Guillermo Aponte - Daniel Ramirez - David Tobar
 * Materia: Sistemas Operativos
 * Fecha: 18/11/2025
 * Archivo: ModuloAgente.h
 *   Este módulo define las estructuras y prototipos de funciones
 *   necesarios para la gestión de agentes y sus peticiones de
 *   reservas en el sistema.
 ***************************************************************/
/*Se verifica si el identificador __MODULO_AGENTES_H__ ha sido definido antes.
Sirve para evitar que el contenido de este archivo se incluya varias veces en un mismo programa.
Si ya estuviera definido, el compilador se saltaría todo el contenido hasta el #endif.*/
#ifndef __MODULO_AGENTES_H__

/*Se define el identificador __MODULO_AGENTES_H__
De tal forma se marca que el archivo ya fue incluido una vez, evitando duplicaciones posteriores.
En este espacio se colocan las declaraciones, estructuras, constantes o prototipos de funciones
que se necesitan compartir entre varios archivos .c del proyecto.*/
#define __MODULO_AGENTES_H__

#include <fcntl.h> // Control de apertura de archivos
#include <pthread.h>
#include <stdbool.h>   // Tipo de dato booleano (true, false)
#include <stdio.h>     // Entrada y salida estándar (printf, fgets)
#include <stdlib.h>    // Funciones generales de utilidad (exit, malloc)
#include <string.h>    // Manejo de cadenas (strcpy, strcmp, strlen)
#include <sys/stat.h>  // Constantes y estructuras para manejo de archivos
#include <sys/types.h> // Definición de tipos de datos (pid_t, etc.)
#include <unistd.h>    // Funciones de manejo de procesos y pipes (read, write, close)
#include <semaphore.h> //manejo de named semaphores

typedef struct {
  char *nombre; //Nombre del agente
  char *fileSolicitud; // Archivo de solicitudes que maneja
  char *pipeRecibe; //Pipe para recibir datos
  int retorno; //Código de retorno de alguna operación
} RetornoAgentes;

typedef struct {
  bool reserva; // Booleano que indica si la reserva fue aceptada
  char nombreAgente[256]; // Nombre del agente que procesa la reserva
  int horaSolicitada; //Hora solicitada para la reserva
  int cantPersonas; //Cantidad de personas para la reserva
  char respuesta[256]; // Respuesta del agente (aceptado/rechazado)
  char nombreFamilia[256]; //Nombre de la familia que realiza la reserva
} Peticion;

// Prototipos de funciones:

RetornoAgentes tomarArgumentosAgente(int argc, char *argv[]); //Procesa los argumentos pasados al agente desde la línea de comandos y retorna un struct RetornoAgentes.
int leerArchivo(RetornoAgentes argumentos); //Permite leer la información de un archivo de solicitudes.
#endif
