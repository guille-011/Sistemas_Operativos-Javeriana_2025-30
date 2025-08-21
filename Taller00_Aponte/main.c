/********************************************************************
*                     Pontificia Universidad Javeriana              *
*             ===============================================       *
* Autor: G. Aponte                                                  *
* Materia: Sistemas Operativos                                      *
* Docente: J. Corredor, PhD                                         *
* Objetivo: Compilación modular                                     *
*                                                                   *
* Descripción: Main                                                 *
*                                                                   *
********************************************************************/


#include "modulo.h" // se incluye el módulo de funciones

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("$./ejecutable valor1 valor2 valor3\n");
        exit(0);
    }

    float altura = atof(argv[1]); // valores pasados como parámetros
    float masacorpotal = atof(argv[2]);
    int N = atoi(argv[3]);

    printFuntion(); // llamada a la función de impresión

    float valorIMC = imcFuncion(masacorpotal, altura); // cálculo de IMC
    printf("El indice de masa corporal es %f\n", valorIMC);
    printf("*********************************\n\n\n");

    printf("**********************************\n");
    printf("           matrices              *\n");
    printf("**********************************\n");

    // Reservar memoria para matrices dinámicas
    float **m1 = (float **)malloc(N * sizeof(float *));
    float **m2 = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) {
        *(m1 + i) = (float *)malloc(N * sizeof(float));
        *(m2 + i) = (float *)malloc(N * sizeof(float));
    }

    // Llenar e imprimir matrices
    llenarMatriz(N, m1, m2);

    printf("Matriz A:\n");
    impMatrix(m1, N);
    printf("Matriz B:\n");
    impMatrix(m2, N);

    // Multiplicación (escalares acumulados)
    float mC = matrixMult(N, m1, m2);
    printf("Resultado escalar: %.2f\n", mC);

    // Liberar memoria
    for (int i = 0; i < N; i++) {
        free(*(m1 + i));
        free(*(m2 + i));
    }
    free(m1);
    free(m2);

    return 0;
}

