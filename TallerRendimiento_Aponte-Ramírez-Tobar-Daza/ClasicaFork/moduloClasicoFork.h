/*#######################################################################################
#* Pontificia Universidad Javeriana
#* Facultad de Ingeniería
#* Carrera: Ingeniería de Sistemas
#* Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
#* Materia: Sistemas Operativos
#* Archivo:
#*      mmClasicaFork.h
#* Descripción:
#*      Este archivo contiene los prototipos de funciones utilizadas para la 
#*      multiplicación clásica de matrices en un entorno con procesos fork().
#*      Incluye las declaraciones de funciones para medir tiempos de ejecución,
#*      inicializar matrices, imprimirlas y realizar la operación de multiplicación.
#*      Su propósito es proporcionar las interfaces necesarias para que el programa 
#*      principal y los módulos auxiliares puedan utilizar estas funciones de manera 
#*      organizada y modular.
######################################################################################*/

#include <stdio.h>          // Biblioteca estándar para operaciones de entrada y salida
#include <stdlib.h>         // Biblioteca para manejo de memoria dinámica y utilidades generales
#include <unistd.h>         // Biblioteca con funciones del sistema (fork, sleep, etc.)
#include <sys/time.h>       // Biblioteca para trabajar con estructuras y funciones de tiempo
#include <time.h>           // Biblioteca para manejo de fechas y reloj del sistema

// Inicio del include guard; evita que este archivo sea incluido más de una vez
#ifndef MMCLASICA_FORK_H
#define MMCLASICA_FORK_H

// Declara la función que inicia la medición del tiempo de ejecución
void InicioMuestra();         

// Declara la función que finaliza la medición del tiempo y muestra el tiempo transcurrido
void FinMuestra();

// Declara la función encargada de realizar la multiplicación de matrices
// Recibe matrices A, B, C, el tamaño D y el rango de filas que debe calcular
void multiMatrix(double *mA, double *mB, double *mC, int D, int filaI, int filaF);

// Declara la función para imprimir una matriz de tamaño DxD en pantalla
void impMatrix(const double *matrix, int D);

// Declara la función que inicializa matrices A y B con valores aleatorios
void iniMatrix(double *mA, double *mB, int D);

// Finaliza el include guard
#endif // __mmClasicaFork_h__
