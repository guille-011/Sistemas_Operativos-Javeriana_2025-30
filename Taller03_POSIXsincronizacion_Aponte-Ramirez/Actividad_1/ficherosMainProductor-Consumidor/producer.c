/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: IPC con Named Semaphores y Shared Memory
 * Archivo: producer.c
 * Descripción: Implementa el proceso productor que crea los 
 *              semáforos POSIX con nombre y la memoria compartida,
 *              mapea la región compartida, genera los datos en el 
 *              búfer circular y, al finalizar, libera todos los 
 *              recursos utilizados.
 * Fecha: 14/11/25
 ******************************************************************/

#include "../modulosProducer/moduloProducer.h"

int main() {
    
    /*Declaración de las variables utilizdas*/
    int shm_fd;                      // Descriptor del bloque de memoria compartida
    compartir_datos* compartir = NULL; // Apuntador a la estructura en memoria compartida
    sem_t *vacio, *lleno;             // Semáforos de control

    /*Creación de los semáfotos */
    vacio = crearSemaforo("/vacio", BUFFER);  // Contador de espacios vacíos
    lleno = crearSemaforo("/lleno", 0);       // Contador de espacios llenos

    
    //2. Crear o abrir la memoria compartida
     
     abrirMemoriaCompartida(&shm_fd);

  
     //* 3. Mapear la memoria compartida y comenzar a producir
    
    mapearYProducir(&compartir, vacio, lleno, shm_fd);

    
     //4. Liberar los recursos utilizados
     
    liberarRecursos(compartir, shm_fd, vacio, lleno);

    return 0;
}
