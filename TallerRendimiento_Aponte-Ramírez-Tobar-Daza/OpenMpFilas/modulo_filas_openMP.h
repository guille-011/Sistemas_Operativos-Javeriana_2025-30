/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
 * Materia: Sistemas Operativos
 * Tema: Implementación de multiplicación de matrices con OpenMP
 * Archivo: modulo_filas_openMP.h
 *
 * Fecha: 05/11/2025
 *
 * Descripción general:
 *   Archivo de cabecera que contiene las declaraciones de las 
 *   funciones utilizadas en el módulo de multiplicación de matrices
 *   con OpenMP. 
 *   Incluye guardas de inclusión para evitar múltiples referencias
 *   al mismo archivo durante la compilación.
 *
 * Funciones declaradas:
 *   - InicioMuestra(): Marca el inicio de una medición de tiempo.
 *   - FinMuestra(): Calcula e imprime el tiempo total transcurrido.
 *   - impMatrix(): Imprime una matriz normal o transpuesta.
 *   - iniMatrix(): Inicializa dos matrices con valores aleatorios.
 *   - multiMatrixTrans(): Multiplica dos matrices en paralelo con
 *     OpenMP y almacena el resultado en una tercera.
 *
 * Archivos relacionados:
 *   - modulo_filas_openMP.c, Implementación de estas funciones.
 *   - mmFilasOpenMP.c, Programa principal que las utiliza.
 *******************************************************************/





/*Se verifica si el identificador __MODULO_OPENMPFILAS_H__ ha sido definido antes.
 Sirve para evitar que el contenido de este archivo se incluya varias veces en un mismo programa.
 Si ya estuviera definido, el compilador se saltaría todo el contenido hasta el #endif.*/
 #ifndef __MODULO_OPENMPFILAS_H__

 /*Se define el identificador __MODULO_OPENMPFILAS_H__.
De tal fomra se marca marca que el archivo ya fue incluido una vez, evitando duplicaciones posteriores.
En este espacio se colocan las declaraciones, estructuras, constantes o prototipos de funciones
que se necesitan compartir entre varios archivos .c del proyecto.*/
 #define __MODULO_OPENMPFILAS_H__



/*Se declara la función InicioMuestra, que toma el tiempo en que comienza la ejecución de 
un proceso*/
void InicioMuestra();

/*Se declara la función FinMuestra, que toma el tiempo en el que termina la ejecución de un proceso*/
void FinMuestra();

/*Se declara la función impMatrix, que recibe un apuntador a double, representando la matriz que se imprimirá.
el tamaño de la matriz y un parámetro t que indica si esta matriz se imprime de manera transpuesta o no*/
void impMatrix(double *matrix, int D, int t);

/*Se declara la función iniMatrix, que recibe dos apuntadores a double que representan dos matrices y su tamaño,
inicializa las matrices con valores aleatorios*/
void iniMatrix(double *m1, double *m2, int D);

/*Se declara la función multiMatrixTrans que recibe tres apuntadores a double que representan tres matrices
y su tamaño, multiplica mA y mB y el resultado lo guarda en mC*/
void multiMatrixTrans(double *mA, double *mB, double *mC, int D);

 #endif //Se cierra la guarda de inclusión 