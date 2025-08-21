/**************************************************************************************
 * Fecha: 21 agosto 2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: rendimiento y concurrencia
************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	int *ptr; //declara un puntero de int, llamado ptr que podrá acceder a una dirección de memoria
	ptr = malloc(15 * sizeof(*ptr)); //Reserva el tamaño de 15 enteros para que ptr pueda acceder a ellos
	if(ptr != NULL){ //verifica que malloc no falle antes de realizar cualquier operación
		*(ptr + 5) = 480; //Guarda 480 en la sexta posición del arreglo dinámico operado con ptr
	}
	free(ptr); //Se libera la memoria reservada para ptr
	return 0;
}
