/***************************************************************
 * Universidad: Pontificia Universidad Javeriana
 * Carrera: Ingeniería de Sistemas
 * Autor: Guillermo Aponte - Daniel Ramirez - David Tobar
 * Materia: Sistemas Operativos
 * Fecha: 16/11/2025
 * Archivo: ModuloControlador.h
 * Descripción:
 *   Este módulo se encarga de la gestión central del sistema de 
 *   reservas, actuando como controlador principal que coordina 
 *   las solicitudes enviadas por los agentes y administra la 
 *   ocupación de los parques. Maneja la sincronización entre 
 *   múltiples hilos, asegurando que las reservas se procesen de 
 *   manera ordenada y segura mediante mutex y semáforos. Además, 
 *   mantiene el control del tiempo mediante un reloj interno, 
 *   permite registrar y monitorear las entradas y salidas de las 
 *   familias, controla el aforo máximo de los parques y genera 
 *   reportes periódicos y finales sobre el estado de las reservas 
 *   y la utilización de los espacios. En general, garantiza la 
 *   correcta comunicación entre los agentes y los parques, así 
 *   como el manejo eficiente y seguro de todas las operaciones 
 *   del sistema.
 ***************************************************************/

/*Se verifica si el identificador __MODULO_CONTROLADOR_H__ ha sido definido antes.
 Sirve para evitar que el contenido de este archivo se incluya varias veces en un mismo programa.
 Si ya estuviera definido, el compilador se saltaría todo el contenido hasta el #endif.*/
#ifndef __MODULO_CONTROLADOR_H__

 /*Se define el identificador __MODULO_CONTROLADOR_H__
De tal forma se marca que el archivo ya fue incluido una vez, evitando duplicaciones posteriores.
En este espacio se colocan las declaraciones, estructuras, constantes o prototipos de funciones
que se necesitan compartir entre varios archivos .c del proyecto.*/
#define __MODULO_CONTROLADOR_H__

#include <stdlib.h> // Funciones generales de utilidad (exit, malloc)
#include <stdio.h> // Entrada y salida estándar (printf, fgets) 
#include <unistd.h> // Funciones de manejo de procesos y pipes (read, write, close) 
#include <sys/types.h> // Definición de tipos de datos (pid_t, etc.) 
#include <string.h> // Manejo de cadenas (strcpy, strcmp, strlen) 
#include <sys/stat.h> // Constantes y estructuras para manejo de archivos 
#include <fcntl.h> // Control de apertura de archivos 
#include <stdbool.h> // Tipo de dato booleano (true, false)
#include <pthread.h> //manejo de hilos
#include <semaphore.h> //manejo de named semaphores

/*Se declara un mutex pthread_mutex_t llamado reportePorHoraM*/
extern pthread_mutex_t reportePorHoraM;

/*Se inicializa un condicional pthread_cond_t condiReportePorHora*/
extern pthread_cond_t condiReportePorHora;


/*Se definen las variables globales que se usarán en dos archivos .c, usando la claúsula extern
para indicarle al compilador que estas solo serán usadas una vez, pues si no se indica esto se producirían 
errores de compilación al duplicarse las variables globales para cada modulo que las use*/
extern bool terminado; 
extern bool notificar;
extern int solicitudesNegadas;
extern int solicitudesAceptadas;
extern int solicitudesReProgramadas;
extern int agentesTotalesRegistrados;

/*Estructura que guarda todos los argumenros ingresados por el usuario para poderse usar en funciones siguientes*/
typedef struct {
    int horaIni; //Hora de inicio
    int horaFin; //Hora de fin
    int segHoras; //Segundos por hora
    int total; //Aforo total
    char* pipeRecibe; //Nombre del pipe por el que se reciben las solicitudes
    int retorno; //Variable de retorno para saber si hubo errores al tomar los argumentos
} RetornoArgumentos;

/*Estructura que guarda todas las variables necesarias para el reloj*/
typedef struct {
    int horaIni; //Hora de inicio
    int segHoras; //Segundos por hora
    int horaActual; //Hora actual
    int horaFin; //Hora de fin
} Reloj;


/*Estructura que guarda todos los datos importantes de una familiar*/
typedef struct{
    int cantPersonas; //Cantidad de personas en la familia
    int horaLlegada; //Hora de llegada de la familia
    char nombre[256]; //Nombre de la familia
} Familia;


/*Estructura que guarda todos los datos importantes de un parque*/
typedef struct {
    int cantFamilias; //Cantidad de familias en el parque
    Familia* familias; //Puntero a la lista de familias en el parque
    int cuantasSalen; //Cantidad de personas que salen
    int cuantasEntran; //Cantidad de personas que entran
    int aforoMaximo; //Aforo máximo del parque
    int cantPersonas; //Cantidad actual de personas en el parque
    int hora; //Hora del parque
} Parque;


/*Estructura que guarda cada peticiópn realizada por un agente*/
typedef struct {
  bool reserva; //Indica si es una reserva o una presentación 
  char nombreAgente[256]; //Nombre del agente que envía la petición
  int horaSolicitada; //Hora solicitada para la reserva o presentación
  int cantPersonas; //Cantidad de personas en la petición
  char respuesta[256]; //Respuesta del controlador a la petición
  char nombreFamilia[256]; //Nombre de la familia en la petición
} Peticion;

/*Estructura que guarda todas las estructuras necesarias para usarse en la función que recibe las solicitudes*/
typedef struct{
    Reloj* reloj;
    RetornoArgumentos argumentos;
    Parque* parques;
} Paquete;


RetornoArgumentos tomarArgumentosControlador(int argc, char *argv[]); //Función que toma los argumentos ingresados por el usuario al iniciar el programa
void* manipularReloj(void* recibe); //Función que maneja el reloj interno del sistema
void* recibirMensajes(void* paquete); //Función que recibe las solicitudes enviadas por los agentes, las procesa y les da su respuesta
void* reportePorHora(void* parques); //Función que genera reportes por hora sobre las familias que entran y salen de los parques
void inicializarParques(RetornoArgumentos argumentos, Parque parques[]); //Función que inicializa los parques según los argumentos ingresados por el usuario
int reporteFinal(Parque* parques, RetornoArgumentos argumentos); //Función que genera el reporte final al terminar la simulación

#endif
