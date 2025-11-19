/***************************************************************
 * Universidad: Pontificia Universidad Javeriana
 * Carrera: Ingeniería de Sistemas
 * Autor: Guillermo Aponte - Daniel Ramirez - David Tobar
 * Materia: Sistemas Operativos
 * Fecha: 16/11/2025
 * Archivo: ModuloAgente.c
 * Descripción:
 *   Este módulo implementa las funciones necesarias para la gestión
 *   de agentes en un sistema de reservas. Cada agente se comunica
 *   con un controlador central mediante pipes para enviar y recibir
 *   información sobre solicitudes de reserva. El módulo incluye
 *   la lectura de archivos CSV con las peticiones de reservas, la
 *   validación de los argumentos de ejecución del agente, y la
 *   preparación de estructuras de datos para la comunicación con
 *   el controlador. Se asegura la correcta apertura y cierre de
 *   pipes y archivos, así como la gestión de memoria dinámica.
 ***************************************************************/

#include "../ModulosDeDefinicion/ModuloAgente.h" // Incluye el archivo de cabecera con declaraciones y estructuras

/***************************************************************
 * Función: tomarArgumentosAgente
 * Descripción: Procesa los argumentos pasados al ejecutar el agente.
 *              Verifica los flags obligatorios (-s, -a, -p) y
 *              devuelve un struct RetornoAgentes con los valores.
 ***************************************************************/
RetornoAgentes tomarArgumentosAgente(int argc, char *argv[]) {

    RetornoAgentes retorno;                  // Variable de retorno que contendrá los datos del agente

    // Reservar memoria dinámica para los strings
    retorno.nombre = malloc(256);            // Nombre del agente
    retorno.fileSolicitud = malloc(256);     // Archivo de solicitudes
    retorno.pipeRecibe = malloc(256);        // Nombre del pipe de comunicación

    if (argc < 7) {                          // Validación de número mínimo de argumentos
        printf("Error: Número insuficiente de argumentos.\n");
        retorno.retorno = -1;                // Indicador de error
        return retorno;
    }

    bool argumentos[3] = {false, false, false}; // Flags para saber si se recibieron -s, -a, -p
    int posiciones[3] = {-1, -1, -1};           // Guardar posición de cada flag en argv

    for (int i = 0; i < 6; i++) {           // Recorre los primeros 6 argumentos buscando los flags
        if (strcmp(argv[i], "-s") == 0) {   // Flag nombre del agente
            argumentos[0] = true;
            posiciones[0] = i;
        } else if (strcmp(argv[i], "-a") == 0) { // Flag archivo de solicitudes
            argumentos[1] = true;
            posiciones[1] = i;
        } else if (strcmp(argv[i], "-p") == 0) { // Flag pipe de comunicación
            argumentos[2] = true;
            posiciones[2] = i;
        }
    }

    // Verificar que todos los flags obligatorios fueron recibidos
    for (int i = 0; i < 3; i++) {
        if (!argumentos[i]) {
            printf("Error: Falta el argumento\n");
            retorno.retorno = -1;
            return retorno;
        }
    }

    // Copiar los valores de los argumentos a la estructura de retorno
    strcpy(retorno.nombre, argv[posiciones[0] + 1]);       // Nombre del agente
    strcpy(retorno.fileSolicitud, argv[posiciones[1] + 1]); // Archivo de solicitudes
    strcpy(retorno.pipeRecibe, argv[posiciones[2] + 1]);   // Pipe de comunicación
    retorno.retorno = 0;                                   // Todo correcto

    return retorno; // Retornar estructura con datos
}

/***************************************************************
 * Función: leerArchivo
 * Descripción: Lee un archivo CSV de solicitudes de reserva y
 *              se comunica con el controlador usando pipes. 
 *              Envía cada solicitud y recibe la respuesta.
 ***************************************************************/
int leerArchivo(RetornoAgentes argumentos) {
    int fd_write, fd_read, error, horaActual;          // File descriptors y variables de control
    int *horaRecibida = malloc(sizeof(int));          // Almacena la hora enviada por el controlador

    char nombreNamedPipe[256];                        // Nombre del pipe del controlador
    char pipeRecibe[256];                             // Nombre del pipe del agente
    char bufferArchivo[256];                          // Buffer para leer líneas del archivo
    char bufferAux[256];                              // Buffer auxiliar para parsear CSV

    Peticion *saludoInicial = malloc(sizeof(Peticion));   // Estructura para saludo inicial al controlador
    Peticion *solicitudReserva = malloc(sizeof(Peticion)); // Estructura para cada solicitud de reserva
    Peticion *respuesta = malloc(sizeof(Peticion));        // Estructura para recibir respuesta del controlador

    // Construcción de ruta para el pipe del controlador
    strcpy(nombreNamedPipe, "/tmp/");
    strcat(nombreNamedPipe, argumentos.pipeRecibe);

    // Construcción de ruta para el pipe del agente
    strcpy(pipeRecibe, "/tmp/");
    strcat(pipeRecibe, argumentos.nombre);

    // Abrir pipe principal para escribir
    fd_write = open(nombreNamedPipe, O_RDWR);
    if (fd_write < 0) {
        perror("Abriendo PipeRecibe"); // Mostrar error si falla
        return -1;
    }

    // Crear pipe del agente (se elimina si ya existía)
    unlink(pipeRecibe);
    if (mkfifo(pipeRecibe, 0640) == -1) {
        perror("Creando el Pipe Principal");
        return -1;
    }

    // Abrir pipe del agente para leer
    fd_read = open(pipeRecibe, O_RDWR);
    if (fd_read < 0) {
        perror("Abriendo Pipe del Agente");
        return -1;
    }

    // Abrir el archivo de solicitudes en modo lectura
    FILE *file = fopen(argumentos.fileSolicitud, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de solicitudes");
        return -1;
    }

    horaActual = 0; // Inicializa la hora
    strcpy(saludoInicial->nombreAgente, argumentos.nombre); // Nombre del agente
    saludoInicial->reserva = false;                           // Marca saludo inicial

    // Enviar saludo al controlador
    error = write(fd_write, saludoInicial, sizeof(Peticion));
    if (error == -1) {
        perror("Error escribiendo en el pipe del agente");
        return -1;
    }

    // Leer hora actual enviada por el controlador
    error = read(fd_read, horaRecibida, sizeof(int));
    if (error == -1) {
        perror("Error leyendo en el pipe del agente");
        return -1;
    }
    printf("HORA ACTUAL RECIBIDA DESDE EL CONTROLADOR %d\n", *horaRecibida);

    /*******************************
     * Procesamiento de archivo CSV
     *******************************/
    while (true) {
        if (fgets(bufferArchivo, sizeof(bufferArchivo), file) == NULL) { // Leer línea
            if (!feof(file)) { // Si no es fin de archivo, error
                perror("Leyendo del archivo");
                return -1;
            } else {
                break; // Fin del archivo
            }
        }

        bufferArchivo[strcspn(bufferArchivo, "\n")] = 0; // Quitar salto de línea
        strcpy(bufferAux, bufferArchivo); // Copiar a buffer auxiliar para tokenizar

        char *tokens = strtok(bufferAux, ","); // Primer token = nombre familia
        if (!tokens) {
            printf("Error: línea CSV inválida\n");
            continue;
        }
        strcpy(solicitudReserva->nombreFamilia, tokens);

        tokens = strtok(NULL, ","); // Segundo token = hora solicitada
        if (!tokens) {
            printf("Error: línea CSV inválida\n");
            continue;
        }
        solicitudReserva->horaSolicitada = atoi(tokens);

        tokens = strtok(NULL, ","); // Tercer token = cantidad de personas
        if (!tokens) {
            printf("Error: línea CSV inválida\n");
            continue;
        }
        solicitudReserva->cantPersonas = atoi(tokens);

        solicitudReserva->reserva = true;                        // Marca solicitud activa
        strcpy(solicitudReserva->nombreAgente, argumentos.nombre); // Agente que procesa

        // Enviar solicitud al controlador
        if (write(fd_write, solicitudReserva, sizeof(Peticion)) == -1) {
            perror("Error escribiendo en el Pipe Principal");
            return -1;
        }

        // Leer respuesta del controlador
        if (read(fd_read, respuesta, sizeof(Peticion)) == -1) {
            perror("Error leyendo del Pipe Principal");
            return -1;
        }

        // Mostrar petición y respuesta
        printf("PETICION: {\nNombreFamilia: %s\nHoraSolicitada: %d:00\nPersonasSolicitadas: %d\n} RESPUESTA: %s\n\n",
               respuesta->nombreFamilia,
               respuesta->horaSolicitada,
               respuesta->cantPersonas,
               respuesta->respuesta);

        sleep(2); // Simular tiempo de procesamiento
    }

    // Cerrar pipes 
    close(fd_read);
    close(fd_write);
    unlink(pipeRecibe); // Eliminar pipe del agente
    //Libera memoria dinámica
    free(saludoInicial);
    free(solicitudReserva);
    free(respuesta);
    free(horaRecibida);

    return 0; // Finaliza correctamente
}
