/***************************************************************
 * Universidad: Pontificia Universidad Javeriana
 * Carrera: Ingeniería de Sistemas
 * Autor: Guillermo Aponte - Daniel Ramirez - David Tobar
 * Materia: Sistemas Operativos
 * Fecha: 16/11/2025
 * Archivo: Controlador.c
 * Descripción:
 *   Este módulo implementa la funcionalidad del controlador del 
 *   sistema de reservas, encargado de coordinar la interacción 
 *   entre los agentes y los parques. Administra las solicitudes 
 *   de reserva, verificando la disponibilidad de espacios y el 
 *   cumplimiento de las restricciones de aforo y horarios. 
 *   Controla el tiempo mediante un reloj interno, permitiendo 
 *   sincronizar la entrada y salida de familias de los parques 
 *   en función de las horas actuales. Gestiona la comunicación 
 *   con los agentes a través de pipes, enviando respuestas de 
 *   aceptación, reprogramación o rechazo de reservas. Además, 
 *   mantiene la contabilidad de solicitudes procesadas, 
 *   sincroniza los hilos de ejecución mediante mutex y condiciones, 
 *   y genera reportes periódicos y finales sobre la ocupación 
 *   de los parques y el estado de las reservas. Este módulo 
 *   garantiza que el sistema funcione de manera ordenada, segura 
 *   y eficiente durante toda la jornada de atención.
 ***************************************************************/

#include "../ModulosDeDefinicion/ModuloControlador.h" //Se incluye el modulo que contiene las declaraciones de las funciones y estructuras


/*Se inicializa un mutex usando las siguientes carácterísticas:

pthread_mutex_t: Es el tipo de variable usado por POSIX para representar un mutex (mutual exclusion lock), el mutex
se utiliza para proteger secciones críticas, asegurando que un solo hilo pueda ejecutar cierta región de código o acceder a cierta estructura
de datos a la vez, evitando que se generen errores durante la ejecución.

al mutex declarado se le da el nombre: bloqueoCC.

El valor asignado a mutex1 es PTHREAD_MUTEX_INITIALIZER, que es una inicialización estática del mutex provista
por POSIX, es decir, esta inicialización se realiza en tiempo de compilación en lugar de hacerlo en tiempo de ejecución,
si se quiere realizar su inicialización dinámica se tendria que usar pthread_mutex_init, que permite configurarle los valores
por defecto a diferencia de como se está haciendo en este código.

Valores por defecto dados por PTHREAD_MUTEX_INITIALIZER:

-Tipo de mutex: PTHREAD_MUTEX_DEFAULT: Mutex normal, no recursivo.
-Protocolo: PTHREAD_PRIO_NONE: No da prioridad al mutex.
-Compartición: PTHREAD_PROCESS_PRIVATE: Solo sirve dentro del mimso proceso.
-Robustez: no robusto, si un hilo muere con el mutex tomado se le asigna undefined behavior.
-Chequeo de errores: No valida errores, como por ejemplo desbloquearlo antes de bloquearlo.
-Recursividad: No recursivo.

Operaciones ofrecidas por pthread_mutex_t:

-pthread_mutex_lock(&bloqueoCC): Bloquea un proceso hasta adquirir el mutex.
-pthread_mutex_trylock(&bloqueoCC): Trara de adquirir el mutex, si no puede devuelve EBUSY.
-pthread_mutex_timedlock(&bloqueoCC, &abstime): Bloquea el proceso y espera hasta un tiempo límite.
-pthread_mutex_unlock(&bloqueoCC): Libera el mutex.*/
pthread_mutex_t reportePorHoraM = PTHREAD_MUTEX_INITIALIZER;


/*Se inicializa una variable de condición usando las siguientes características:

pthread_cond_t: Es el tipo de variable usado por POSIX para representar una condición
(condition variable). Las variables de condición permiten que los hilos se sincronicen
entre sí mediante señales, haciendo que un hilo pueda esperar (bloquearse) hasta que
otro hilo indique (despierte) que cierta condición se ha cumplido.

El valor asignado a la variable de condición es PTHREAD_COND_INITIALIZER, lo cual indica
una inicialización estática provista por POSIX. Esto significa que la configuración se
realiza en tiempo de compilación. Si se quisiera inicializar dinámicamente, se usaría
pthread_cond_init, que permite configurar atributos personalizados.

Valores por defecto dados por PTHREAD_COND_INITIALIZER:

- Tipo de atributo: PTHREAD_COND_DEFAULT
- Sin prioridad ni orden especial de activación de hilos
- No compartible entre procesos a menos que se configure explícitamente

Operaciones ofrecidas por pthread_cond_t:

- pthread_cond_wait(&condiCC, &mutex):
  Hace que el hilo espere hasta que ocurra una señal. Mientras espera, el mutex asociado
  se libera automáticamente y se vuelve a adquirir cuando la condición se cumple.

- pthread_cond_signal(&condiCC):
  Despierta a un hilo que esté esperando en la variable de condición.

- pthread_cond_broadcast(&condiCC):
  Despierta a *todos* los hilos que estén esperando en la condición.

Notas importantes:
- Siempre debe usarse junto con un mutex para evitar race conditions.
- Las variables de condición no guardan un estado por sí mismas; dependen de una condición
  lógica definida en el programa y del mutex asociado.
*/

pthread_cond_t condiReportePorHora = PTHREAD_COND_INITIALIZER;



/*Variables globales utilizadas para el manejo del código, de tal forma se garantiza la integridad de las variables con los booleanos
terminado y notificar y las variables necesarias para mostrar los resultados finales, siendo estas los enteros declarados*/
bool terminado = false;
bool notificar = false;
int solicitudesNegadas = 0;
int solicitudesAceptadas = 0;
int solicitudesReProgramadas = 0;



/***************************************************************
 * Función: tomarArgumentosAgente
 * Descripción: Procesa los argumentos pasados al ejecutar el agente.
 *              Verifica los flags obligatorios (-i, -f, -s, -t, -p) y
 *              devuelve un struct RetornoArgumentos con los valores.
 ***************************************************************/
RetornoArgumentos tomarArgumentosControlador(int argc, char *argv[]) {
  RetornoArgumentos retorno;


  /*Si se envia una cantidad menor de argumentos a la necesaria se imprime un mensaje de error*/
  if (argc < 11) {
    printf("Error: Número insuficiente de argumentos.\n");
    retorno.retorno = -1;
    return retorno;
  }

  /*Se declaran dos arreglos, el primero es un arreglo de booleanos que, por cada posición, guarda true o false, según si se encuentra 
  el argumento solicitado, y uno de posiciones que guarda la posición real del arreglo argv en la que se encuentra cada argumento necesario, de la 
  siguiente forma:
  
  posición 0: flag -i
  posición 1: flag -f
  posición 2: flag -s
  posición 3: flag -t
  posición 4: flag -p
   */
  bool argumentos[5] = {false, false, false, false, false}; 
  int posiciones[5] = {-1, -1, -1, -1, -1}; 


  /*Se declara un ciclo que busca en todas los argumentos enviados como parámetro, y va guardando según las posiciones definidas anteriormente en el arreglo
  de argumentos*/
  for (int i = 0; i < 10; i++) {
    if (strcmp(argv[i], "-i") == 0) {
      argumentos[0] = true;
      posiciones[0] = i;
    } else if (strcmp(argv[i], "-f") == 0) {
      argumentos[1] = true;
      posiciones[1] = i;
    } else if (strcmp(argv[i], "-s") == 0) {
      argumentos[2] = true;
      posiciones[2] = i;
    } else if (strcmp(argv[i], "-t") == 0) {
      argumentos[3] = true;
      posiciones[3] = i;
    } else if (strcmp(argv[i], "-p") == 0) {
      argumentos[4] = true;
      posiciones[4] = i;
    }
  }

  /*Se realiza un ciclo que recorre el arreglo de argumentos, si este arreglo no muestra true en todas sus posiciones, quiere decir
  que esta flag no fue enviada, imprimiendo un mensaje de error*/
  for (int i = 0; i < 5; i++) {
    if (!argumentos[i]) {
      printf("Error: Falta el argumento.\n");
      retorno.retorno = -1;
      return retorno;
    }
  }

  /*Se guardan todas las variables enviadas como parámetro de la siguiente forma:
  
  Se declara una estructura de retorno, y en cada una de sus variables, según sea el caso, 
  se le asigna el valor de argv, en la posición posiciones[n]+1, según sea la posición que se quiere encontrar,
  en caso de que no se cumpla lo solicitado, se imprime un mensaje de error
  
  */
  
  
  retorno.horaIni = atoi(argv[posiciones[0] + 1]); //Se guarda en horaIni la conversión a entero del argumento siguiente al flag -i
  if (retorno.horaIni == 0) { //Si la conversión a entero da 0, se imprime un mensaje de error, y se retorna -1 en la variable de retorno
    printf("La hora inicial es inválida");
    retorno.retorno = -1;
    return retorno;
  }
  retorno.horaFin = atoi(argv[posiciones[1] + 1]); //Se guarda en horaFin la conversión a entero del argumento siguiente al flag -f
  if (retorno.horaFin == 0) { //Si la conversión a entero da 0, se imprime un mensaje de error, y se retorna -1 en la variable de retorno
    printf("La hora final es inválida");
    retorno.retorno = -1;
    return retorno;
  }
  retorno.segHoras = atoi(argv[posiciones[2] + 1]); //Se guarda en segHoras la conversión a entero del argumento siguiente al flag -s
  if (retorno.segHoras == 0) { //Si la conversión a entero da 0, se imprime un mensaje de error, y se retorna -1 en la variable de retorno
    printf("segHoras es inválido");
    retorno.retorno = -1;
    return retorno;
  }
  retorno.total = atoi(argv[posiciones[3] + 1]); //Se guarda en total la conversión a entero del argumento siguiente al flag -t
  if (retorno.total == 0) { //Si la conversión a entero da 0, se imprime un mensaje de error, y se retorna -1 en la variable de retorno
    printf("El valor del aforo máximo es inválido");
    retorno.retorno = -1;
    return retorno;
  }

  char *pipeRecibe = argv[posiciones[4] + 1]; //Se guarda en pipeRecibe el argumento siguiente al flag -p

  if (retorno.horaIni < 7 || retorno.horaIni > 19) { //Si la hora de inicio es menor a 7 o mayor a 19, se imprime un mensaje de error y se retorna -1 en la variable de retorno
    printf("Error: La hora de inicio debe estar entre 7 y 19.\n");
    retorno.retorno = -1;
    return retorno;
  }

  if (retorno.horaFin < 7 || retorno.horaFin > 19 ) {  //Si la hora de fin es menor a 7 o mayor a 19, se imprime un mensaje de error y se retorna -1 en la variable de retorno
    printf("Error: La hora de fin debe estar entre 7 y 19.\n");
    retorno.retorno = -1;
    return retorno;
  }
   if ( retorno.horaFin < retorno.horaIni) { //Si la hora de fin es menor a la hora de inicio, se imprime un mensaje de error y se retorna -1 en la variable de retorno
    printf("Error: La hora fin es menor que la hora de inicio.\n");
    retorno.retorno = -1;
    return retorno;
  }


  retorno.retorno = 0; //Si todo sale bien se retorna 0 en la variable de retorno
  retorno.pipeRecibe = pipeRecibe; //Se guarda en pipeRecibe el argumento siguiente al flag -p

  return retorno;
}


/***************************************************************
 * Función: reporteFinal
 * Descripción: Procesa todas las variables globales, los parques
 *              enviados como parámetro y los argumentos iniciales
 *              ingresados por el usuario para generar un reporte
 *              final al concluir la jornada de reservas, basado en todo
 *              lo anterior
 ***************************************************************/
int reporteFinal(Parque *parques, RetornoArgumentos argumentos) {
  int mayorCantidad = parques->cantPersonas; //Se guarda la cantidad de personas del primer parque como la mayor cantidad
  int horasPico[12]; //Se crea un arreglo de horas pico
  horasPico[0] = argumentos.horaIni; //Se guarda la hora inicial como la primera hora pico
  int j = 1; //Se inicia el contador de horas pico en 1, considerando que habrá por lo menor una hora pico

  int menorCantidad = parques->cantPersonas; //Se guara la cantidad de personas del primer parque como la menor cantidad
  int horasMenosPico[12]; //Se crea un arreglo de horas menos pico
  horasMenosPico[0] = argumentos.horaIni; //Se guarda la hora inicial como la primera hora menos pico
  int k = 1; //Se inicia el contador de horas menos pico en 1, considerando que habrá por lo menor una hora menos pico

  Peticion *terminarEjecucion; //Se crea una estructura de tipo Peticion para enviar el mensaje de finalización


  /*Se realiza un ciclo desde i = 1, para mirar desde el segundo parque, hasta horaFin horaIni, para mirar todos los parques,
  en este ciclo se realizan las respectivas comparaciones para determinar cuál es la menor cantidad de personas que hubo en el parque
  y de igual forma guardar cuáles fueron los parques que tuvieron estas horas*/
  for (int i = 1; i < argumentos.horaFin - argumentos.horaIni; i++) {
    /*Si el parque iterado es tiene mas personas que las guardadas en la variable mayorCantidad,
    se deja j como 1, para saber que la cantidad de parques con hora pico es solo una en este caso,
    se guarda en la primera posición del arreglo horasPico la hora en tiempo real de este parque, sumando horaIni + i,
    y se sobreescribe la variable mayorCantidad con la encontrada acá*/
    if ((parques + i)->cantPersonas > mayorCantidad) { 
      j = 1;
      horasPico[0] = argumentos.horaIni + i;
      mayorCantidad = (parques + i)->cantPersonas;

    /*Se realiza el mismo proceso anterior, pero comparando si la cantidad es menor a menorCantidad, para este caso se 
    realiza la sobreescritura para los parques menos pico*/
    } else if ((parques + i)->cantPersonas < menorCantidad) {
      k = 1;
      horasMenosPico[0] = argumentos.horaIni + i;
      menorCantidad = (parques + i)->cantPersonas;

      /*En caso de que haya mas de un parque con la cantidad mayor de personas hayada hasta el momento, 
      se aumenta j en uno y se guarda en la posción iterada según j de horas pico este parque encontrado, que en este caso
      correspondería a la última posción
      este proceso se reinicia cada vez que se encuentra un parque con una cantidad mayor de personas*/
    } else if ((parques + i)->cantPersonas == mayorCantidad) {
      j++;
      horasPico[j - 1] = argumentos.horaIni + i;

      /*Se realiza lo mismo para menorCantidad, modificando la variable*/
    } else if ((parques + i)->cantPersonas == menorCantidad) {
      k++;
      horasMenosPico[k - 1] = argumentos.horaIni + i;
    }
  }

  /*Se realiza la impresión del reporte del dia, imprimiento, en primer lugar, las variables globales modificadas a lo largo de todo
  el código y, posteriormente, mediante ciclos desde 0 hasta j, que guarda la cantidad de parques con horas pico y desde 0 hasta k, que 
  guarda la cantidad de parques con horas menos pico, se imprime cuáles fueron estas horas*/
  printf("====================================================\n");
  printf("        REPORTE FINAL DEL DIA DE HOY          \n");
  printf(" CANTIDAD DE SOLICITUDES NEGADAS:         %d  \n", solicitudesNegadas);
  printf(" CANTIDAD DE SOLICITUDES ACEPTADAS:       %d  \n", solicitudesAceptadas);
  printf(" CANTIDAD DE SOLICITUDES REPROGRAMADAS:   %d  \n", solicitudesReProgramadas);
  printf(" HORAS PICO:");
  for (int i = 0; i < j; i++) {
    if (i != j - 1) {
      printf(" %d -", horasPico[i]);
    } else {
      printf(" %d\n", horasPico[i]);
    }
  }
  printf(" HORAS MENOS CONCURRIDAS:");
  for (int i = 0; i < k; i++) {
    if (i != k - 1) {
      printf(" %d -", horasMenosPico[i]);
    } else {
      printf(" %d\n", horasMenosPico[i]);
    }
  }
  printf("====================================================\n");


  /*Se envía el mensaje de finalización a los agentes, para que estos terminen su ejecución correctamente*/
  char nombreNamedPipe[256];
  strcpy(nombreNamedPipe, "/tmp/"); //Se guarada el la variable nombreNamedPipe la ruta del pipe donde se escribirá el mensaje
  strcat(nombreNamedPipe, argumentos.pipeRecibe);


    /*Se inicializa una variable fd_write con la función open de su pipe,
    abriendo el pipe enviado por el usuario como parámetro en modo de lectura y escritura.

    Las variables guardan el "Descriptor de archivo" retornado por la función open, el descriptor de archivo es un número entero
    que el sistema operativo usa para identificar cualquier recurso que pueda leer o escribir, de tal manera puede acceder
    al pipe necesario, 
    Si se retorna -1 en la función open, quiere decir que hubo un error abriendo el pipe*/ 

  int fd_write = open(nombreNamedPipe, O_RDWR); //Se abre el pipe en modo de escritura
  if (fd_write < 0) {
    perror("PipeRecibe (FINAL)");
    return -1;
  }
  terminarEjecucion = malloc(sizeof(Peticion));

  /*Se llama a la función write, la cuál recibe como parámetro fd_write, es decir, el pipe que envia el mensaje
  terminarEjecución, que es lo que se enviará sizeof(Peticion), que es el tamaño de dicho mensaje*/


  if (write(fd_write, terminarEjecucion, sizeof(Peticion)) == -1) { //Se escribe en el pipe el mensaje de finalización
    perror("Escribiendo en el pipe el mensaje final");
    return -1;
  }

  if (close(fd_write) == -1) { //Se cierra el pipe
    perror("Cerrando el pipe del mensaje final");
  }
}

/***************************************************************
 * Función: manipularReloj
 * Descripción: Basado en la hora de inicio y en la hora final 
 *             definidas en los argumentos iniciales, esta función
 *            maneja el avance del reloj interno del sistema
 *  
 ***************************************************************/
void *manipularReloj(void *recibe) {
  Reloj *reloj = (Reloj *)recibe; //Se guarda en la variable reloj de tipo Reloj el casteo del parámetro enviado a la consola
  reloj->horaActual = reloj->horaIni; //Se inicializa la hora actual con la hora de inicio
  while (reloj->horaActual < reloj->horaFin) { //Mientras la hora actual sea menor a la hora de fin, se realiza el siguiente ciclo
    printf("==============================================\n");
    printf("LA HORA ACTUAL ES %d:00\n", reloj->horaActual); //se imprime la hora actual que se está iterando
    
     /*Se usa la función pthread_mutex_lock para proteger una zona crítica del código que sería la que está después
    del llamado a esta función, la función recibe una variable de tipo pthread_mutex_t y con ella hace lo siguiente:

    -Le asigna a la variable mutex una flag o estado, los cuáles pueden ser LOCKED, UNLOCKED
    -Guarda en el mutex la información del hilo que lo posee
    -Guarda en el mutex una cola de los hilos que están esperando por su uso

    La función como tal hace lo siguiente:

    1. Revisa si el mutex está libre
    2. Si el mutex está libre, se lo asigna al hilo que está ejecutando el programa
    3. Si está ocupado, el hilo se queda esperando hasta que se libere el mutex*/

    pthread_mutex_lock(&reportePorHoraM); 
    notificar = true; //en la zona protegida se asigna true a la variable notificar

      /*
      Esta función despierta UN hilo que esté esperando en
      pthread_cond_wait(&cond, &mutex). Si varios hilos están
      esperando, el que despierta NO está definido (lo decide el
      scheduler, no hay orden garantizado).

       Hace lo siguiente::
       1. Busca hilos bloqueados en pthread_cond_wait 
      2. Si hay al menos uno esperando:
       - Despierta a uno.
       - El hilo despertado NO continúa de una vez:
           * Primero debe volver a adquirir el mutex que se pasó
             en pthread_cond_wait (esto se hace internamente).
             Si el mutex sigue tomado por otro hilo, espera.
  3. Si NO hay hilos esperando:
       - La señal se pierde (no se guarda para después).

  Cosas importantes:
  - Esta función NO entrega el mutex al hilo despertado.
    Solo lo despierta; el hilo debe esperar por el mutex cuando
    intente continuar.

  - No garantiza orden ni prioridad de qué hilo despierta.

  - Debe usarse con un mutex que proteja la condición lógica
    para evitar condiciones de carrera.*/
    pthread_cond_signal(&condiReportePorHora);

    /*Se usa la función pthread_mutex_unlock para liberar el mutex previamente tomado por pthread_mutex_lock.
    La función recibe como parámetro la dirección de una variable de tipo pthread_mutex_t, y realiza lo siguiente:

    -Cambia el estado del mutex de LOCKED a UNLOCKED
    -Elimina la referencia del hilo que tenía el mutex (ya no es dueño)
    -Despierta a uno de los hilos que estuviera esperando en la cola para que intente tomar el mutex

    Internamente la función hace:

    1. Verifica que el hilo que llama esta función sea el mismo que bloqueó el mutex (para evitar que otro hilo lo suelte)
    2. Marca el mutex como disponible
    3. Si hay hilos esperando, le da paso al siguiente para que pueda ejecutar su sección crítica
*/
    pthread_mutex_unlock(&reportePorHoraM);
    sleep(reloj->segHoras); //Hace que el programa espere el tiempo ingresado por el usuario correspondiente a la cantidad de segundos que representa una hora
    reloj->horaActual++; //Se aumenta en uno la hora actual
  }

  /*Cuando pasan todas las horas ingresadas por el usuario se realiza otro proceso*/
  terminado = true;  //Se cambia la variable terminado a true
  printf("\n\nLA HORA ACTUAL ES %d:00 Y SE CIERRA EL PARQUE ~~~\n", reloj->horaActual); //Se imprime l ahora final
  pthread_mutex_lock(&reportePorHoraM); //Se bloquea el mutex
  notificar = true; //Se cambia la variable notificar a true
  pthread_cond_signal(&condiReportePorHora); //Se envía la señal a los hilos que estén esperando en la variable de condición
  pthread_mutex_unlock(&reportePorHoraM); //Se desbloquea el mutex

  return NULL; //Se retorna NULL al finalizar la función
}


/***************************************************************
 * Función: recibirMensajes
 * Descripción: Esta función se encarga de recibir y procesar
 *              las solicitudes de reserva enviadas por los agentes
 *              a través de un pipe nombrado. Verifica la 
 *              disponibilidad de espacio en los parques para 
 *              las horas solicitadas y responde a los agentes 
 *              con la aceptación, reprogramación o rechazo de 
 *              las reservas.
 ***************************************************************/
void *recibirMensajes(void *paquete) {


  /*La función recibe una estructura Paquete, que contiene las estructuras
  de reloj, argumentos y parques */
  Paquete *Parametro = (Paquete *)paquete; //Se guarda en la variable Parametro el casteo del parámetro recibido

  char nombreNamedPipe[256]; //Se crea una variable para guardar el nombre del pipe
  char horaNuevaChar[256]; //Se crea una variable para guardar la nueva hora en caso de reprogramación
  Peticion *peticion = malloc(sizeof(Peticion)); //Se crea una variable de tipo Peticion para guardar la petición recibida y se reserva memoria
  Peticion *respuesta = malloc(sizeof(Peticion)); //Se crea una variable de tipo Peticion para guardar la respuesta a enviar y se reserva memoria
  Parque *aux; //Se crea una variable de tipo Parque para recorrer los parques
  Familia *aux2; //Se crea una variable de tipo Familia para recorrer las familias dentro de los parques
  bool admitido = true; //Se crea una variable booleana para saber si la solicitud es admitida o no
  int error, a, b, c, numeroFamilia, horaNueva, fdwrite; //Se crean variables enteras necesarias para el manejo del código
  int *horaEnviar; //Se crea un puntero a entero para guardar la hora a enviar a los agentes

  /*Se usan las variables strcpy y strcat para guardar en la varaible nombreNamedPipe la ruta completa del pipe
  que se usará para recibir las solicitudes de los agentes*/
  strcpy(nombreNamedPipe, "/tmp/");
  strcat(nombreNamedPipe, Parametro->argumentos.pipeRecibe);

  /*Se borra el archivo en caso de que ya exista anteriormente para evitar errores en la ejecución del código usando la función
  unlink*/
  unlink(nombreNamedPipe);

  /*Se utiliza la función mkfifo para crear el pipe principal.
    esta función crea los archivos que utilizará el sistema operativo para todo el proceso de pipes,
    se le envia como parámetro la ruta del pipe a crear y sus permisos de acceso y tipo de archivo
    
   0640 indica que el pipe tendrá permisos de lectura/escritura
   
   Se manejan errores al mismo momento que se hace su creación, siendo así, si mkfifo retorna -1, se imprime un mensaje de error*/
  if (mkfifo(nombreNamedPipe, 0640) == -1) {
    perror("Creando el Pipe Principal");
    return (void *)-1;
  }

  /*Se declara un descriptor de archivo fdread que leerá del pipe creado anteriormente, lo declara de lectura y escritura
  para que en caso de que nadie mas lo abra en su modo contrario, no haya ningún error*/
  int fdread = open(nombreNamedPipe, O_RDWR);

  //Si hay un error abriendo el pipe, se imprime un mensaje de error y se retorna -1 en la función
  if (fdread < 0) {
    perror("PipeRecibe");
    return (void *)-1;
  }

  /*Mientras la variable global terminado se encuentre en false, es decir, mientras no se llegue a la hora final, 
  se ejecuta toda la siguiente sección del código*/
  while (!terminado) {

     /*Se llama a la función read, la cuál recibe fdread, que es el pipe que recibe los mensajes enviados por los agentes,
      en la estructura peticion donde se guardará el mensaje recibido, y sizeof(Peticion), que es el tamaño del mensaje a recibir.
            
      El retorno de read es la cantidad real de bytes leídos desde el pipe, este valor se guarda en read_bytes*/
    error = read(fdread, peticion, sizeof(Peticion));
    if (error == -1) {
      perror("Error leyendo en el pipe principal"); //Si se presenta un error leyendo se imprime un mensaje de error
      return (void *)-1;
    }
    if (terminado) { //si otro hilo cambia la variable terminado a true, se sale del ciclo
      break;
    }


    pthread_mutex_lock(&reportePorHoraM); //Usa un mutex para proteger todas las operaciones que se realizan a continuación
    if (peticion->reserva) { //Si la petición es de reserva, se realiza todo el siguiente proceso
      respuesta->cantPersonas = peticion->cantPersonas; //Se guarda en la respuesta la cantidad de personas de la petición
      respuesta->horaSolicitada = peticion->horaSolicitada; //Se guarda en la respuesta la hora solicitada de la petición
      strcpy(respuesta->nombreFamilia, peticion->nombreFamilia); //Se guarda en la respuesta el nombre de la familia de la petición
      strcpy(respuesta->nombreAgente, peticion->nombreAgente); //Se guarda en la respuesta el nombre del agente de la petición

      /*En primer lugar se verifica que la cantidad de personas de la solicitud no exceda el aforo máximo, si esto se cumple, se hacen todas
      las otras evaluaciones*/
      if (peticion->cantPersonas <= Parametro->argumentos.total) {

        /*El segundo filtro es verificar que la hora solicitada sea al menos dos horas antes del cierre del parque*/
        if (peticion->horaSolicitada < Parametro->argumentos.horaFin - 1) { 

          /*Se verifica que la hora solicitada haya sido después de que iniciara la simulación, de tal forma se 
          sabe donde se desde donde se deben empezar a evaluar los parques*/
          if (peticion->horaSolicitada > Parametro->argumentos.horaIni) {

            /*Si la hora solicitada es mayor la inical, se guarda la hora inical en a y la hora solicitada en b,
            luego se guarda la resta de las anteriores en c, para saber desde que posición del arreglo de parques se
            debe contar*/
            a = Parametro->argumentos.horaIni;
            b = peticion->horaSolicitada;
            c = b - a;
          } else {
            c = 0; //Si la hora solicitada es menor a la inicial, se empieza a evaluar desde el primer parque
          }
          aux = Parametro->parques; //Se guarda en aux la dirección del primer parque

          /*Se guarda en personasEnElParque la cantidad de personas del primer parque a evaluar mas la cantidad de personas solicitadas*/
          int personasEnElParque = (aux + c)->cantPersonas + peticion->cantPersonas; 

          /*Se evalua que el total de personas que queda después de la reserva sea menor al aforo máximo y que la hora sea mayor a la hora que se está
          evaluando actualmente, si esto se cumple se hacen las siguientes verificaciones*/
          if (personasEnElParque <= Parametro->argumentos.total && peticion->horaSolicitada >= Parametro->reloj->horaActual) {

            /*Todo el proceso siguiente corresponde a la actualización del parque ubicado en la posición indicada según la hora
            donde se realiza la asignación y del parque de la hora siguiente, pues cada familia está dos horas en el parque*/
            numeroFamilia = (aux + c)->cantFamilias; //Se guarda la cantidad de familias en el parque en numeroFamilia
            aux2 = (aux + c)->familias; //Se guiarda en aux2 la dirección del primer familia del parque
            aux2 += numeroFamilia; //Se avanza en aux2 hasta la posición correspondiente a la nueva familia que se agregará

            aux2->cantPersonas = peticion->cantPersonas; //Se guarda en la nueva familia la cantidad de personas de la petición
            aux2->horaLlegada = Parametro->argumentos.horaIni + c; //Se guarda en la nueva familia la hora de llegada correspondiente
            strcpy(aux2->nombre, peticion->nombreFamilia); //Se guarda en la nueva familia el nombre de la familia de la petición

            numeroFamilia = (aux + c + 1)->cantFamilias; //Se guarda la cantidad de familias en el siguiente parque en numeroFamilia
            aux2 = (aux + c + 1)->familias; //Se guiarda en aux2 la dirección del primer familia del siguiente parque
            aux2 += numeroFamilia; //Se avanza en aux2 hasta la posición correspondiente a la nueva familia que se agregará

            aux2->cantPersonas = peticion->cantPersonas; //Se guarda en la nueva familia la cantidad de personas de la petición
            aux2->horaLlegada = Parametro->argumentos.horaIni + c; //Se guarda en la nueva familia la hora de llegada correspondiente
            strcpy(aux2->nombre, peticion->nombreFamilia); //Se guarda en la nueva familia el nombre de la familia de la petición

            (aux + c)->cantFamilias++; //Se aumenta en uno la cantidad de familias del parque actual
            (aux + c + 1)->cantFamilias++; //Se aumenta en uno la cantidad de familias del siguiente parque
            (aux + c)->cantPersonas = personasEnElParque; //Se actualiza la cantidad de personas del parque actual
            (aux + c + 1)->cantPersonas += peticion->cantPersonas; //Se actualiza la cantidad de personas del siguiente parque
            (aux + c)->cuantasEntran += peticion->cantPersonas; //Se actualiza la cantidad de personas que entran en el parque actual
            (aux + c + 2)->cuantasSalen += peticion->cantPersonas; //Se actualiza la cantidad de personas que salen en el parque dos horas después
            
            /*Luego de todas las verificaciones anteriores, se le indica al agente que la solicitud fue aceptada sin problemas
            y se aumenta la variable global solicitudesAceptadas*/
            strcpy(respuesta->respuesta, "RESERVA OK: TODO BIEN\n"); 
            solicitudesAceptadas++;
          } else {

            /*Si la reserva no puede ser asignada para la hora solicitada según las verificaciones anteriores, se procede a verificar en todas
            las otras horas para ver si la reserva puede ser re-asignada*/
            for (int i = c; i < Parametro->argumentos.horaFin - Parametro->argumentos.horaIni - 1; i++) { //Ciclo que recorre desde la hora solicitada hasta la hora final -1
              /*Se guarda en personasEnElParque la cantidad de personas de la hora iterada mas las cantidad en la petición*/
              personasEnElParque = (aux + i)->cantPersonas + peticion->cantPersonas;

              /*Se verifica que la cantidad de personas en el parque iterado sea menor al afoto máximo y que la hora sea meyor a la actual*/
              if (personasEnElParque <= Parametro->argumentos.total && (aux + i)->hora >= Parametro->reloj->horaActual) {
                numeroFamilia = (aux + i)->cantFamilias; //Se guarda la cantidad de familias en el parque en numeroFamilia
                aux2 = (aux + i)->familias; //Se guiarda en aux2 la dirección del primer familia del parque
                aux2 += numeroFamilia; //Se avanza en aux2 hasta la posición correspondiente a la nueva familia que se agregará

                aux2->cantPersonas = peticion->cantPersonas; //Se guarda en la nueva familia la cantidad de personas de la petición
                aux2->horaLlegada = Parametro->argumentos.horaIni + i; //Se guarda en la nueva familia la hora de llegada correspondiente
                strcpy(aux2->nombre, peticion->nombreFamilia); //Se guarda en la nueva familia el nombre de la familia de la petición

                numeroFamilia = (aux + i + 1)->cantFamilias; //Se guarda la cantidad de familias en el siguiente parque en numeroFamilia
                aux2 = (aux + i + 1)->familias; //Se guarda en aux2 la dirección del primer familia del siguiente parque
                aux2 += numeroFamilia; //Se avanza en aux2 hasta la posición correspondiente a la nueva familia que se agregará

                aux2->cantPersonas = peticion->cantPersonas; //Se guarda en la nueva familia la cantidad de personas de la petición
                aux2->horaLlegada = Parametro->argumentos.horaIni + i; //Se guarda en la nueva familia la hora de llegada correspondiente
                strcpy(aux2->nombre, peticion->nombreFamilia); //Se guarda en la nueva familia el nombre de la familia de la petición

                (aux + i)->cantFamilias++; //Se aumenta en uno la cantidad de familias del parque actual
                (aux + i + 1)->cantFamilias++; //Se aumenta en uno la cantidad de familias del siguiente parque
                (aux + i)->cantPersonas = personasEnElParque; //Se actualiza la cantidad de personas del parque actual
                (aux + i + 1)->cantPersonas += peticion->cantPersonas; //Se actualiza la cantidad de personas del siguiente parque
                (aux + i)->cuantasEntran += peticion->cantPersonas; //Se actualiza la cantidad de personas que entran en el parque actual
                (aux + i + 2)->cuantasSalen += peticion->cantPersonas; //Se actualiza la cantidad de personas que salen en el parque dos horas después


                 /*Luego de todas las verificaciones anteriores, se le indica al agente que la solicitud fue reprogramada
            y se aumenta la variable global solicitudesReProgramadas*/
                strcpy(respuesta->respuesta, "RESERVA REPROGRAMADA: PARA EL MISMO DIA A LAS ");
                solicitudesReProgramadas++;
                horaNueva = (aux + i)->hora; //Se guarda en horaNueva la hora del parque donde se asignó la reserva
                sprintf(horaNuevaChar, "%d", horaNueva); //Se convierte la horaNueva a cadena de caracteres
                strcat(respuesta->respuesta, horaNuevaChar); //Se concatena la nueva hora a la respuesta
                strcat(respuesta->respuesta, ":00\n"); //Se concatena el final del mensaje a la respuesta
                admitido = true; //Se cambia la variable admitido a true
                break; //Se sale del ciclo for
              }
            }

            /*Si la familia no pudo ser reasignada, se imprime que no hay disponibilidad para todo el dia*/
            if (admitido == false) {
              strcpy(respuesta->respuesta, "RESERVA NEGADA: NO HAY DISPONIBILIDAD PARA ESA HORA NI PARA EL RESTO DEL DIA");
              solicitudesNegadas++; //Se aumenta la varaible global solicitudesNegadas
            }
          }
        } else {

          /*Si la familia solicitó su reserva una hora antes de que cierre el parque, o cuando este cierra se imprime esta respuesta*/
          strcpy(respuesta->respuesta, "RESERVA NEGADA: NO HAY BLOQUE DE DOS HORAS DISPONIBLE (PARQUE CERRADO)");
          solicitudesNegadas++; //Se aumenta la varaible global solicitudesNegadas
        }
      } else {
        /*Si la familia solicitó una cantidad mayor de personas al aforo máximo se imprime esta respuesta*/
        strcpy(respuesta->respuesta, "RESERVA NEGADA: SU FAMILIA EXCEDE EL AFORO MÁXIMO DEL PARQUE");
        solicitudesNegadas++; //Se aumenta la varaible global solicitudesNegadas
      }

      admitido = false; //Admitido se vuelve false para la siguiente iteración


      /*Si el programa no ha terminado según la hora que se esté iterando en el reloj, se hace todo lo siguiente*/
      if (!terminado) {
        /*Se usa strcpy y strcat para guardar la ruta del pipe de agente donde se enviará la respuesta correspondiente, esta ruta equivale
        al nombre del agente*/
        strcpy(nombreNamedPipe, "/tmp/");
        strcat(nombreNamedPipe, respuesta->nombreAgente);
        fdwrite = open(nombreNamedPipe, O_RDWR); // Siempre que se recibe una peticion, se abre el pipe
        if (fdwrite < 0) { //Se verifica que el pipe se haya abierto correctamente
          perror("Abriendo el pipe de un agente");
          return (void *)-1;
        }

        /*Se usa la función write para escribir la respueta en el pipe del agente correspondiente*/
        error = write(fdwrite, respuesta, sizeof(Peticion));
        if (error == -1) { //Se verifica que la escritura se haya realizado correctamente
          perror("Error escribiendo en el pipe principal: ");
          exit(EXIT_FAILURE);
        }
      }

      if (close(fdwrite) == -1) { //Se cierra el pipe y se verifican errores
        perror("Cerrando el pipe de un agente");
      }

      /*En caso de que la petición no sea una reserva, es el registro de un agente, por lo tanto, para este proceso se realiza todo el bloque que sigue*/
    } else {
      printf("UN AGENTE SE REGISTRÓ, SE LLAMA: %s\n", peticion->nombreAgente); //Se imprime el nombre del agente que se registró
      agentesTotalesRegistrados++; //Se aumenta en uno la variable global de agentes registrados
      horaEnviar = &Parametro->reloj->horaActual; //Se guarda en horaEnviar la dirección de la hora actual del reloj
      
      /*Se guarda en nombreNamedPipe la dirección del pipe donde se le enviarán mensajes al agente correspondiente, siendo esta el nombre del mismo agente */
      strcpy(nombreNamedPipe, "/tmp/"); 
      strcat(nombreNamedPipe, peticion->nombreAgente);
      fdwrite = open(nombreNamedPipe, O_RDWR); // Siempre que se recibe una peticion, se abre el pipe
      if (fdwrite < 0) { //Se verifica que el pipe se haya abierto correctamente
        perror("Abriendo el pipe de un agente");
        return (void *)-1;
      }
      error = write(fdwrite, horaEnviar, sizeof(int)); //Se escribe en el pipe la hora actual
      if (error == -1) {
        perror("Error escribiendo en el pipe principal: ");
        exit(EXIT_FAILURE);
      }
      if (close(fdwrite) == -1) { //Se cierra el pipe y se verifican errores
        perror("Cerrando el pipe de un agente");
      }
    }
    pthread_mutex_unlock(&reportePorHoraM); //Se desbloquea el mutex
  }

  close(fdread); //Se cierra el pipe de lectura

  /*Se guarda la ruta del pipe principal donde se están recibiendo todas las solicitudes para cerrarlo*/
  strcpy(nombreNamedPipe, "/tmp/"); 
  strcat(nombreNamedPipe, Parametro->argumentos.pipeRecibe);
  unlink(nombreNamedPipe);

  /*Se librea la memoria reservada*/
  free(peticion); 
  free(respuesta);

  return NULL; //Se retorna NULL
}


/***************************************************************
 * Función: reportePorHora
 * Descripción: Función reportePorHora que se encarga de generar
 *              reportes por hora sobre las familias que entran
 *              y salen de los parques, basándose en la información
 *              almacenada en las estructuras de Parque y Familia.
 ***************************************************************/
void *reportePorHora(void *parques) {
  Parque *parquesActuales = (Parque *)parques; //Se guarda en parquesActuales el casteo del parámetro recibido
  Familia *aux; //Se crea una variable de tipo Familia para recorrer las familias dentro de los parques
  int i = 0; //Se crea una variable entera i para recorrer los parques
  bool resultados = false; //Se crea una variable booleana para saber si hay resultados que mostrar

  /*Se crea un ciclo que se estará ejecutando infinitamente hasta que se detecte el hilo de reloj comunique que se ha
  terminado la simulación */
  while (true) {
    pthread_mutex_lock(&reportePorHoraM);
    while (!notificar) {

      /*Mientras que notificar sea false, el hilo esprará con cond_wait en el siguiente bloque*/
      pthread_cond_wait(&condiReportePorHora, &reportePorHoraM);
      /*Si terminado es true, se termina el ciclo while que espera por la notificación*/
      if (terminado == true) {
        break;
      }
    }
    /*Si terminado es true se termina el ciclo infinito que ejecutaba este hilo*/
    if (terminado == true) {
      pthread_mutex_unlock(&reportePorHoraM);
      break;
    }

    /*Mientras se siga ejecutando el ciclo se realiza lo siguiente*/
    printf("La cantidad de personas que entran es %d\n", (parquesActuales + i)->cuantasEntran); //Se imprime la cantidad de personas que entran en el parque de la hora actual
    
    /*Se verifica que la hora del parque iterado no sea -1*/
    if ((parquesActuales + i)->hora != -1) {
      aux = (parquesActuales + i)->familias; //Se guarda en aux la dirección del primer familia del parque

      /*Se realiza un ciclo desde 0 hasta la cantidad de familias en el parque evaluado, en donde por cada iteración se aumenta la familia quie se está mirando*/
      for (int j = 0; j < (parquesActuales + i)->cantFamilias; j++, aux++) {
        if (aux->horaLlegada == (parquesActuales + i)->hora) { //Si la hora de llegada de la familia es igual a la hora del parque actual, se imprime el nombre de la familia
          if (!resultados) { //Si resultados es false se imprime un mensaje
            printf("Las familias que entran son:\n");
          }
          printf("- %s\n", aux->nombre); //Se imprime el nombre de la familia
          resultados = true; //Se cambia resultados a true
        }
      }
    }
    /*Si resultados nunca fue cambiado a true, se indica que no entraron familias en este parque*/
    if (resultados == false) {
      printf("No hay familias entrando\n");
    }
    resultados = false; //Se reinicia resultados a false para la siguiente iteración

    /*Se realiza OTRA VEZ el proceso realizado anteriormente, pero en este caso teniendo en cuenta las familias que salen,
    de tal forma, se mira las familias que entraron dos horas antes, para saber cuáles estarian salinedo en la hora actual*/


    printf("La cantidad de personas que salen es %d\n", (parquesActuales + i)->cuantasSalen);
    if ((parquesActuales + i)->hora != -1) {
      if (i > 1) {
        aux = (parquesActuales + i - 1)->familias;
        for (int j = 0; j < (parquesActuales + i - 1)->cantFamilias; j++, aux++) {
          if (aux->horaLlegada == (parquesActuales + i)->hora - 2) {
            if (!resultados) {
              printf("Las familias que salen son:\n");
            }
            printf("- %s\n", aux->nombre);
            resultados = true;
          }
        }
      }
    }

    /*Si resultados nunca fue cambiado a true, se indica que no salieron familias en este parque*/
    if (resultados == false) {
      printf("No hay familias saliendo\n");
      printf("==============================================\n\n\n");
    } else {
      printf("==============================================\n\n\n");
    }
    /*se reinician las variables declaradas anteriormente y se aumenta i para mirar la siguiente familia*/
    resultados = false;
    notificar = false;
    i++;
    pthread_mutex_unlock(&reportePorHoraM); //Se desbloquea el mutex
  }

  return NULL; //Se retorna NULL al finalizar la función
}

/***************************************************************
 * Función: inicializarParques
 * Descripción: Esta función inicializa los parques según los
 *              argumentos proporcionados, asignando horas,
 *              aforo máximo y reservando memoria para las familias.
 ***************************************************************/
void inicializarParques(RetornoArgumentos argumentos, Parque parques[]) {
  int cantidadDeParques = argumentos.horaFin - argumentos.horaIni;

  /*Ciclo que recorre la cantidad máxima de parques posibles, es decir, 12 horas*/

  for (int i = 0; i < 13; i++) {
    parques[i].hora = -1; // Para saber hasta donde llegan los parques
    // En el caso de tener 12 parques (el maximo) de las 7h a las 19h y el 13 siempre lleva -1
  }

  /*Cilo que recorre la cantidad real de parques para la simulación*/
  for (int i = 0; i < cantidadDeParques; i++) {
    parques[i].hora = i + argumentos.horaIni; //Se asigna la hora inicial a cada parque
    parques[i].aforoMaximo = argumentos.total; //Se asigna el aforo máximo a cada parque
    parques[i].cantFamilias = 0; //Se inicializa la cantidad de familias en 0
    parques[i].cuantasEntran = 0; //Se inicializa la cantidad de personas que entran en 0
    parques[i].cuantasSalen = 0; //Se inicializa la cantidad de personas que salen en 0
    parques[i].cantPersonas = 0; //Se inicializa la cantidad de personas en 0
    parques[i].familias = malloc(argumentos.total * sizeof(Familia)); //Se reserva memoria para la cantidad máxima de familias posibles en el parque
  }
}