/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autor: Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: Lectura de archivo y cálculo de promedio
 * Fecha: 29/10/2025
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_archivo>\n", argv[0]);
        return 1;
    }

    FILE *archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    int numero;
    long suma = 0;
    int contador = 0;

    // Leer números hasta que se acabe el archivo
    while (fscanf(archivo, "%d", &numero) == 1) {
        suma += numero;
        contador++;
    }

    fclose(archivo);

    if (contador == 0) {
        printf("El archivo no contiene números válidos.\n");
        return 0;
    }

    double promedio = (double)suma / contador;
    printf("Cantidad de números leídos: %d\n", contador);
    printf("Promedio: %.2f\n", promedio);

    return 0;
}
