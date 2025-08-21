
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
        char *p; //Declara un puntero de char, llamado p, que podrá acceder a una dirección de memoria
        char  *q= NULL; //Declara un puntero de char llamado q, que podrá accederr a una dirección de memoria y le asigna el valor NULL
        
	p = (char*)malloc(100*sizeof(char)); // Reserva el tamaño de 100 caracteres para que p acceda a ellos
        q = (char*)malloc(100*sizeof(char));  // Reserva el tamaño de 100 caracteres para que q acceda a ellos
        printf("Address of p = %p\n", p); //Imprime en pantalla la direccion de p
        strcpy(p, "Hello, Im the best in Operating Systems!!!"); //Guarda en la direccion de p una cadena de caracteres
        printf("%s\n",p); //Muestra el mensaje guardado en p
        printf("About to copy \"Goobye\" to q\n"); //Muestra por pantalla el mensje que se va a copiar en q
        strcpy(q, "Goobye"); //Guarda en la direccion de q una cadena de caracteres 
        printf("String copied\n");
        printf("%s\n", q); //imprime el mensaje copiado en q

	free(p); //libera la memoria reservada para p
	free(q); //libera la memoria reservada para q

return 0;

}
