/***************************************************************
 * Universidad: Pontificia Universidad Javeriana
 * Carrera: Ingeniería de Sistemas
 * Autor: Guillermo Aponte - Daniel Ramirez - David Tobar
 * Materia: Sistemas Operativos
 * Fecha: 16/11/2025
 * Archivo: ControladorDeReserva.c
 * Descripción:
 *   Este módulo implementa el controlador del sistema de 
 *   reservas de parques, gestionando la entrada y salida de 
 *   familias, recibiendo solicitudes de agentes y verificando 
 *   la disponibilidad y capacidad de los parques. 
 *
 *   Maneja hilos para simular el reloj, generar reportes por hora
 *   y procesar mensajes de agentes mediante named pipes. Coordina 
 *   la finalización de los agentes usando semáforos y asegura la 
 *   correcta liberación de memoria y el control de errores.
 ***************************************************************/

#include "ModulosDeDefinicion/ModuloControlador.h" //Se incluye el modulo que contiene las declaraciones de las funciones y estructuras

//Se declara un contador de agentes registrados para poder terminarlos a todos al final de la ejecucion usando un named semaphore
int agentesTotalesRegistrados = 0;

//Funcion principal
int main(int argc, char *argv[]) {
  //Se declara el arreglo de hilos
  pthread_t hilos[3];

  //Se declara una estructura RetornoArgumentos para guardar todos los argumentos con los que se ejecuta
  //el controlador y para facilitar el transporte de los mismos
  RetornoArgumentos argumentos;

  //Se declara un apuntador al reloj y se reserva memoria para él
  Reloj *reloj = malloc(sizeof(Reloj));

  //Se declara el arreglo de parques:
  //Cada parque corresponde a una hora diferente
  //El máximo de parques es 12 que son las horas que hayd esde las 7 a las 19
  //Ponemos 13 porque no siempre disponemos del tamaño total del arreglo y el parque #13 siempre
  //tiene un identificador que nos permite saber que terminamos de recorrerlo
  Parque parques[13];

  //Declaramos un apuntador a vacío para guardar el retorno del hilo que ejecuta la función recibirMensajes
  void *retorno;

  //Creamos el semáforo que se utiliza para terminar a todos los agentes al finalizar y manejamos posibles errores con perror
  sem_t *fd_sem = sem_open("/terminarAgentes", O_CREAT, 0660, 0);
  if (fd_sem == SEM_FAILED) {
    perror("Creando Semáforo");
    return -1;
  }

  //Declaramos una estructura paquete ya que la funcion recibirMensajes requiere de muchos datos y solo se le puede enviar 
  //un dato del tipo void* por lo cual acá empaquetamos todo lo que aquella función necesita
  Paquete *paquete = malloc(sizeof(Paquete));

  //Declaramos un entero err para guardar retornos de funciones y poder manejar los errores
  int err;

  //Hacemos un llamado a la funcion tomar argumentos la cual nos deja en la estructura variable "argumentos" toda
  //la información con la que se ejecute el controlador
  argumentos = tomarArgumentosControlador(argc, argv);
  if (argumentos.retorno == -1) {
    return -1;
  }

  //Le asignamos al reloj los atributos solicitados al ejecutar el controlador
  reloj->segHoras = argumentos.segHoras;
  reloj->horaIni = argumentos.horaIni;
  reloj->horaFin = argumentos.horaFin;

  //Inicializamos los datos de todos los parques llamando a la funcion inicializarParques
  inicializarParques(argumentos, parques);

  //Se crea el primer hilo para manipular el reloj y se manejan posibles errores
  err = pthread_create(hilos, NULL, manipularReloj, reloj);
  if (err != 0) {
    printf("Error creando hilo: %s\n", strerror(err));
    return -1;
  }

  //Se crea el segundo hilo para hacer el reporte cada hora transcurrida y se manejan posibles errores
  err = pthread_create(hilos + 1, NULL, reportePorHora, parques);
  if (err != 0) {
    printf("Error creando hilo: %s\n", strerror(err));
    return -1;
  }

  //Empaquetado de los datos que necesite la funcion recibirMensajes
  paquete->reloj = reloj;
  paquete->argumentos = argumentos;
  paquete->parques = parques;


  //Se crea el tercer hilo para recibir constantemente los mensajes de los agentes y se manejan posibles errores
  err = pthread_create(hilos + 2, NULL, recibirMensajes, paquete);
  if (err != 0) {
    printf("Error creando hilo: %s\n", strerror(err));
    return -1;
  }

  //Se espera a que los dos primeros hilos terminen
  pthread_join(hilos[0], NULL);
  pthread_join(hilos[1], NULL);

  //Cuando se garantiza que el hilo que maneja el reloj y el que hace reportes cada hora termina, se hace el reporte final.
  //Se manejan posibles errores ocurridos dentro de la funcion del reporte final
  if (reporteFinal(parques, argumentos) == -1) {
    return -1;
  }

  //Se espera a que termine el hilo 3, es decir, el que está escuchando mensajes por el pipeRecibe
  //Lo que el hilo devuelva se guarda en retorno para avisar sobre posibles errores
  pthread_join(hilos[2], &retorno);
  if ((long)retorno == -1) { // el retorno se castea a long porque es del mismo tamaño que void*
    return -1;
  }

  //Cada agente al terminar hace wait en el semáforo, por lo cual, para despertarlos a todos
  //hacemos post la misma cantidad de veces que agentes registrados haya.
  //De este modo los despertamos a todos para permitirles terminar su ejecución
  for (int i = 0; i < agentesTotalesRegistrados; i++) {
    if (sem_post(fd_sem) == -1) {
      perror("sem_post");
      return -1;
    }
  }

  //Se libera memoria para todos los datos para los cuales tuvimos que reservar memoria
  free(reloj);
  free(paquete);
  free(retorno);

  //retornamos 0 para indicar la correcta ejecucion del programa
  return 0;
}
