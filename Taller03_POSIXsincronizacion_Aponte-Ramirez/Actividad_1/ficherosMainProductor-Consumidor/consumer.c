/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: IPC con named Semaphores y Sahred Memory
 * Archivo: consumer.c
 * Descripcion: Implementa el proceso consumidor que abre los 
 *              semáforos POSIX con nombre y la memoria compartida,
 *              mapea la región de memoria para leer los datos 
 *              producidos y, tras consumirlos, libera todos los 
 *              recursos antes de finalizar.
 * Fecha: 14/11/25
 ******************************************************************/
#include "../modulosConsumer/moduloConsumer.h" //incluimos el modulo de declaración de funciones

//Funcion principal
int main() {
    sem_t *vacio;
    sem_t *lleno;
    abrirSemaforos(&vacio, &lleno);

    int fd_compartido;
    abrirMemoriaCompartida(&fd_compartido);

    compartir_datos* compartir;
    mapearYConsumir(&compartir, vacio, lleno, fd_compartido);
    
    liberarRecursos(fd_compartido, vacio, lleno, compartir);


    //Devolvemos 0 para indicar la correcta ejecución del programa
    return 0;
}
