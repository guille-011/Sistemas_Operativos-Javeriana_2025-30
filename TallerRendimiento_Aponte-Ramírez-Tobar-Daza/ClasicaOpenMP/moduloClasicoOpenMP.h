/*#######################################################################################
 #* Fecha: 29 Oct 2025
 #* Autor: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
 #* Programa:
 #* 	 Multiplicación de Matrices algoritmo clásico usando OpenMP
 #* Versión:
 #*	 Paralelismo con OpenMP - Archivo de interfaz (header)
######################################################################################*/

#ifndef MMCLASICAOPENMP_H
#define MMCLASICAOPENMP_H

#include <sys/time.h>

extern struct timeval inicio, fin;

void InicioMuestra();
void FinMuestra();
void impMatrix(double *matrix, int D);
void iniMatrix(double *m1, double *m2, int D);
void multiMatrix(double *mA, double *mB, double *mC, int D);

#endif // MMCLASICAOPENMP_H
