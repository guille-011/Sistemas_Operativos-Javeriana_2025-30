/***************************************************************
 * Universidad: Pontificia Universidad Javeriana
 * Carrera: Ingeniería de Sistemas
 * Autor: Guillermo Aponte - Daniel Ramirez - David Tobar
 * Materia: Sistemas Operativos
 * Fecha: 16/11/2025
 * Archivo: AgenteDeReservas.c
 * Descripción:
 *   Este módulo implementa la funcionalidad principal de un agente
 *   dentro del sistema de reservas. Cada agente se encarga de leer
 *   las solicitudes de reservas desde un archivo, procesarlas y 
 *   comunicarse con el controlador mediante pipes para enviar 
 *   peticiones y recibir respuestas de aceptación, reprogramación 
 *   o rechazo. Además, los agentes esperan la señal de finalización 
 *   del controlador utilizando un semáforo, asegurando que la 
 *   ejecución de cada agente termine de manera coordinada. Este 
 *   módulo también se encarga de manejar errores en la lectura de 
 *   argumentos, apertura de semáforos y comunicación con pipes, 
 *   garantizando la correcta liberación de memoria y la finalización
 *   ordenada del programa.
 ***************************************************************/

#include "ModulosDeDefinicion/ModuloAgente.h" //Se incluye el modulo que contiene las declaraciones de las funciones y estructuras

//Funcion principal
int main(int argc, char *argv[]) {

  //Se abre el semáforo que ya debe haber creado el controlador y se indican posibles errores
  sem_t *fd_sem = sem_open("/terminarAgentes", 0);
  if (fd_sem == SEM_FAILED) {
    perror("Abriendo el semaforo");
    return -1;
  }

  //Se declara una estructura para guardar y acomodar todos los argumentos que se le envíen al
  //agente en su ejecución.
  //Además se manejan posibles erres en la toma de argumentos
  RetornoAgentes argumentos = tomarArgumentosAgente(argc, argv);
  if (argumentos.retorno == -1) {
    return -1;
  }

  //Se llama a la función que lee del archivo de solicitudes y se manejan posibles errores que retorne esta función
  if (leerArchivo(argumentos) == -1) {
    return -1;
  }

  //Una vez el semáforo acabe, va a hacer wait en el semáforo que creó el controlador.
  //Dicho controlador inicia en 0 por lo cual todos los agentes quedan bloqueados en este punto
  //hasta que el controlador hace los "post" necesarios para terminar la ejecución de todos
  sem_wait(fd_sem);

  //Se imprime el mensaje solicitado en el enunciado
  printf("AGENTE %s TERMINA.\n", argumentos.nombre);

  //Se libera memoria de todos los datos que necesitaban una reserva
  free(argumentos.nombre);
  free(argumentos.fileSolicitud);
  free(argumentos.pipeRecibe);

  //Devolvemos 0 para indicar la correcta ejecución del programa
  return 0;
}
