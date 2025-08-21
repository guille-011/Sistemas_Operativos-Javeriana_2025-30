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
	int n, i, *ptr, sum = 0; //declara variables de tipo entero n, i, sum, igualado a 0 y un puntero ptr que podrá acceder a una dirección de memoria 
	printf("Enter number of elements: "); //Le pide al usuario el número de números que se enviarán
	scanf("%d", &n); //Pide el numero de numeros y lo guarda en n, pasándole su dirección de memoria
	ptr = (int*)calloc(n, sizeof(int)); //Reserva el tamaño de los n enteros pasados por el usuario para que ptr pueda acceder a ellos y los inicializa en 0
	if(ptr == NULL){ //revisa si calloc falló
		printf("Error! memory not allocated."); //si calloc falla, imprime un mensaje de error
		exit(1); //Termina el programa con código de error 1
	}
	printf("Enter elements: "); //Le pide al usuario los elementos
	for(i = 0; i < n; ++i){  //Crea un ciclo que va desde 0 hasta la cantidad de elementos que desea ingresar el usuario, para recorrer las posiciones
		scanf("%d", ptr+i); //le pide al usuario el elemento y lo guarda en en la posición que se está iterando de ptr 
		sum += *(ptr+i); //Acumula en sum el valor guardado en el puntero, en la posicion i
	}
	
	printf("Sum = %d", sum); //Imprime por pantalla la suma realizada anteriormente
	free(ptr);  //libera la memoria reservada para ptr

	return 0;
}
