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
	int *ptr, i, n1, n2; //Declara variables de tipo entero n1, n2 y un puntero ptr que podrá acceder a una dirección de memoria
	printf("Enter size: "); //Pide al usuario un tamaño
	scanf("%d", &n1); //Guarda el numero dado por el usuario en la variable n1, pasándole su direccion de memoria
	ptr = (int*)malloc (n1*sizeof(int)); //reserva el tamaño de los n1 enteros pasado por el usuario para que ptr pueda acceder a ellos
	printf("Address of previously allocated memory: "); //Imprime por pantalla que se mostrarán las direcciones de memoria apartadas
	for(i = 0; i < n1; i++){ //Hace un ciclo desde 0 hasta la cantidad de elementos que reservó el usuario
		printf("\n\np = %p\n", ptr+i); //Muestra por pantalla las direcciones de memoria reservadas por ptr como arreglo dinámico
	}
	printf("Enter the new size: "); //Le pide al usuario un nuevo tamaño
	scanf("%d", &n2); //Guarda el nuevo númeor pasado por el usuario en la variable n2. pasándole su dirección de memoria
	ptr = realloc(ptr, n2 * sizeof(int)); //Cambia la memoria reservada por el nuevo tamaño n2 pasado por el usuario para que ptr pueda acceder a ellos
	printf("Addresses of newly allocated memory: "); //Imprime por pantalla que se mostrarán las nuevas direcciones de memoria apartadas
	for(i = 0; i< n2; i++){ //Hace un ciclo desde 0 hasta la cantidad de elementos que reservó el usuario
		printf("\n\np = %p\n", ptr+i); //Muestra por pantalla las direcciones de memoria reservadas por ptr como arreglo dinámico
	}
	free(ptr); //Libera la memoria reservada para ptr

	return 0;
} 



