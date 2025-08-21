/********************************************************************
*                     Pontificia Universidad Javeriana              *
*             ===============================================       *
* Autor: G. Aponte                                                  *
* Materia: Sistemas Operativos                                      *
* Docente: J. Corredor, PhD                                         *
* Objetivo: Compilación modular                                     *
*                                                                   *
* Descripción: Interfaz de funciones                                *
*                                                                   *
********************************************************************/
#include "modulo.h" //Se incluye el modulo de funciones

int printFuntion(void) { //funcion para imprimir lo que se está mostrando
    printf("************************************\n");
    printf("*   Calculadora de Masa Corporal   *\n");
    printf("************************************\n");
    return 0;
}

float imcFuncion(float masa, float altura) { //funcion para calcular el IMC
    float IMC;
    IMC = masa / (altura * altura);
    return IMC;
}

void impMatrix(float **mA, int N) { //funcion que imprime una matriz
    for (int i = 0; i < N; i++) { //iteracion de filas
        for (int j = 0; j < N; j++) { //iteracion de columnas
            printf(" %f ", *(*(mA + i) + j)); //acceso con aritmética de punteros
        }
        printf("\n"); //salto de línea por fila
    }
}

void llenarMatriz(int N, float **mA, float **mB) { //funcion que llena dos matrices con valores aleatorios
    for (int i = 0; i < N; i++) { //iteracion de filas
        for (int j = 0; j < N; j++) { //iteracion de columnas
            *(*(mA + i) + j) = (rand() % 100) * 0.121; //llenado de la primera matriz
            *(*(mB + i) + j) = (rand() % 100); //llenado de la segunda matriz
        }
    }
}


float matrixMult(int N, float **mA, float **mB) { //multiplicación de dos matrices con resultado escalar
    float mC = 0;
    for (int i = 0; i < N; i++) { //iteracion de filas
        for (int j = 0; j < N; j++) { //iteracion de columnas
            for (int k = 0; k < N; k++) { //suma de productos
                mC += (*(*(mA + i) + k)) * (*(*(mB + k) + j)); //sumatoria en el valor mC
            }
        }
    }
    return mC; //se retorna el resultado acumulado
}
