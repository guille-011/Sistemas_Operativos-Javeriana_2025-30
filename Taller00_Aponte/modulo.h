/********************************************************************
*                     Pontificia Universidad Javeriana              *
*             ===============================================       *
* Autor: G. Aponte                                                  *
* Materia: Sistemas Operativos                                      *
* Docente: J. Corredor, PhD                                         *
* Objetivo: Compilación Modular                                     *
*                                                                   *
* Descripción: Modulo funciones                                     *
*                                                                   *
********************************************************************/



#ifndef __MODULO_H__ //include guard para evitar que el archivo se incluya mas de una vez durante la ejecución
#define __MODULO_H__



#include <stdio.h> //librerias 
#include <stdlib.h>
#include <time.h>

int printFuntion(void); //declaracion funcion print
float imcFuncion(float masa, float altura); //declaracion funcion imc
void impMatrix(float **mA, int N);//declaracion imprimir matriz
float matrixMult(int N, float **mA, float **mB);//declaracion multiplicar matriz
void llenarMatriz(int N, float **mA, float **mB);//declaracion llenarmatriz

#endif


