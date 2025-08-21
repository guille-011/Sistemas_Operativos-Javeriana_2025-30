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
	char *q = NULL; //Declara un puntero de char, llamado q, que podrá acceder a una dirección de memoria y la inicializa con NULL
	printf("Requesting space for \"Goodbye\"\n"); //Imprime por pantalla que se guardará espacio para "Goodbye"
	q = (char *)malloc(strlen("Goodbye")+1); //Reserva el tamaño de "Goodbye" mas un espacio para que q acceda a ellos


	if(!q){ //verifica que malloc no falle antes de hacer cualquier operación
		perror("Failed to allocate space because"); //Si no se guarda correctamente el puntero, se imprime un mensaje de error
		exit(1); //Si ocurre el error se sale del programa y se da el código de error 1
	}	
	
	printf("About to copy \"Goodbye\" to q at address %p\n", q); //Se declara que se va a copiar Goodbye en q, luego imprime la dirección de memoria
	strcpy(q, "Goodbye"); //Copia en q la cadena de carácteres "Goodbye"
	printf("String copied\n"); //Indica que se copió el mensaje
	printf("%s\n", q); //Se imprime el mensaje copiado en q

	free(q); //se libera la memoria reservada para q
	
return 0;
}
