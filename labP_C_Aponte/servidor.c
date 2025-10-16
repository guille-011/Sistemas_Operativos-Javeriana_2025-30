/******************************************************************* 
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Implementación del patrón Productor-Consumidor usando Pipe Named (FIFO)
 * Archivo: servidor.c
 *
 * Descripción general:
 *    Este programa actúa como servidor en un esquema de comunicación
 *    entre procesos mediante pipes nombrados (FIFOs). Permite recibir
 *    mensajes de texto desde un proceso cliente, procesarlos (invertir
 *    la cadena) y enviar una respuesta de vuelta al cliente.
 *
 * Funcionalidades principales:
 *    - Crea los pipes nombrados para comunicación cliente-servidor.
 *    - Espera y recibe mensajes enviados por el cliente.
 *    - Procesa cada mensaje (invierte la cadena recibida).
 *    - Envía la respuesta de vuelta al cliente a través del pipe.
 *    - Permite finalizar la comunicación cuando se recibe el mensaje "end".
 *
 * Dependencias:
 *    - Librerías estándar de C: stdio.h, stdlib.h, string.h, unistd.h
 *    - Librerías de manejo de archivos y pipes: sys/types.h, sys/stat.h, fcntl.h
 *
 * Notas:
 *    - Se recomienda ejecutar este servidor antes de iniciar el cliente.
 *    - El flujo de comunicación es secuencial: el servidor procesa un
 *      mensaje a la vez y responde al cliente.
 ******************************************************************/

#include <stdlib.h> // Funciones generales de utilidad (exit, malloc)
#include <stdio.h> // Entrada y salida estándar (printf, fgets) 
#include <unistd.h> // Funciones de manejo de procesos y pipes (read, write, close) 
#include <sys/types.h> // Definición de tipos de datos (pid_t, etc.) 
#include <string.h> // Manejo de cadenas (strcpy, strcmp, strlen) 
#include <sys/stat.h> // Constantes y estructuras para manejo de archivos 
#include <fcntl.h> // Control de apertura de archivos (open,
/*Debido a que el código anterior tenia errores, pues habia un solo pipe de lectura y escritura, se hizo este cambio para 
solucionar la problemática, creando un nuevo pipe encargado de la lectura en el cliente y escritura en servidor
pues el que estaba anteriormente se encargaba de ambas cosas*/
#define FIFO_CS "/tmp/PIPE_CS" //Pipe que escribe desde cliente y lee desde servidor
#define FIFO_SC "/tmp/PIPE_SC" //pipe que escribe desde servidor y lee desde cliente

void reverse_string(char *); //se declara la función reverse_string que recibe un puntero a char

int main() {
    int fd_read, fd_write; //se declaran dos enteros que guardarán el valor retornado al abrir las pipes de lectura y escritura
    char readbuf[80]; //se declara el buffer que guardará la cadena recibida desde el cliente y la cadena enviada desde el servidor
    char end[] = "end"; //se declara la cadena que indicará el fin del proceso
    int to_end; //se declara un entero que guardará el valor de la comparación entre la cadena ingresada y la cadena "end"
    int read_bytes; //se declara un entero que guardará la cantidad de bytes leídos desde la pipe
   
    /*Se utiliza la función mkfifo para crear los pipes de cliente a servidor y de servidor a cliente
    esta función crea los archivos que utilizará el sistema operativo para todo el proceso de pipes,
    se le envia como parámetro la ruta del pipe a crear y sus permisos de acceso y tipo de archivo
    
    S_IFIFO indica que se va a crear un FIFO (pipe con nombre), 0640 indica que el pipe tendrá permisos de
    lectura/escritura*/
    mkfifo(FIFO_CS, S_IFIFO|0640);
    mkfifo(FIFO_SC, S_IFIFO|0640);
   
   /*Se inicializan las variables fd_write y fd_read con la función open de sus respectivos pipes,
    fd_read accede a FIFO_CS, que es la que envia mensajes desde cliente a servidor, por ende esta función
    se inicializa como O_RDONLY, es decir, UNICAMENTE de lectura, que es lo que necesitamos en este caso.
    fd_write accede a FIFO_SC, que es la que envia mensajes desde el servidor al cliente, por ende, se inicializa
    como O_WRONLY, es decir, UNICAMENTE de escritura

    Las variables guardan el "Descriptor de archivo" retornado por la función open, el descriptor de archivo es un número entero
    que el sistema operativo usa para identificar cualquier recurso que pueda leer o escribir, de tal manera puede acceder
    al pipe necesario, fd_write guarda FIFO_CS y fd_read guarda FIFO_SC
    Si se retorna -1 en la función open, quiere decir que hubo un error abriendo el pipe*/  
    fd_read  = open(FIFO_CS, O_RDONLY);
    fd_write = open(FIFO_SC, O_WRONLY);

    /*Se verifica que no haya habido errores abriendo el pipe, en caso de que si, se envia un mensaje de error*/
    if (fd_read < 0 || fd_write < 0) {
        perror("Error opening FIFOs");
        exit(1);
    }

   /*Se crea un ciclo que se repita INFINITAMENTE o, hasta que el servidor lea la palabra "end" del pipe de cliente a servidor
    Mientras que la palabra sea diferente a end, se escribirá en el buffer lo que sea que se haya leído desde el pipe, y se 
    muestra por pantalla
    se verifica que esta palabra sea end.
    Si la palabra es end, se cierran los pipes y se termina el ciclo.
    Si la palabra es diferente a end, se usa la función reverse_string para invertir la cadena leída desde el buffer
    posteriormente se imprime por pantalla el mensaje enviado y su tamaño, luego se envía la cadena invertida al cliente
    y se termina el ciclo*/
    while (1) {

      /*Se llama a la función read, la cuál recibe fd_read, que es el pipe que recibe el mensaje enviado desde el cliente,
      readbuf, que es la cadena donde se guardará el mensaje y sizeof(readbuf) que es el tamaño máximo de esa
      cadena.
            
      El retorno de read es la cantidad real de bytes leídos desde el pipe, este valor se guarda en read_bytes*/
        read_bytes = read(fd_read, readbuf, sizeof(readbuf) - 1);
        readbuf[read_bytes] = '\0'; //se agrega el caracter nulo al final de la cadena leída para evitar errores al usarla
         /*Se imprime por pantalla el mensaje recibido desde el cliente y su tamaño*/
        printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf)); 
        to_end = strcmp(readbuf, end); //se inicializa la variable to_end con el resultado de la comparación entre la cadena ingresada y "end"
      /*Si la comparación del buffer y end es igual a 0, es decir, si son iguales se entra a un condicional*/
        if (to_end == 0) { 
         /*Se usa la función close para cerrar los pipes, la función libera los descriptores de archivo y cuerra la comunicación*/
         close(fd_read);
         close(fd_write);
         break; //termina el ciclo si el usuario ingresó "end"
      }

        reverse_string(readbuf); //se invierte la cadena leída desde el buffer usando la función reverse_string
        /*Se imprime por pantalla el mensaje enviado y su tamaño*/
        printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", readbuf, (int) strlen(readbuf)); 
        /*Se llama a la función write, la cuál recibe como parámetro fd_write, es decir, el pipe que envia el mensaje
         al cliente, readbuf, el mensaje que se enviará y strlen(readbuf), que es el tamaño de dicho mensaje*/
        write(fd_write, readbuf, strlen(readbuf));
        sleep(2); //Se pausa el servidor por 2 segundos antes de volver a iniciar el ciclo, para darle tiempo al cliente de procesar la información
    }

    return 0;
}

/*Función que invierte una cadena, recibe como parámetro un puntero a char*/

void reverse_string(char *str){ 
   int last, first; // Variables para controlar índices de inicio y fin de la cadena
   char temp;  // Variable temporal para intercambiar caracteres
   last = strlen(str) - 1; //Se guarda en last el último indice de la cadena, antes de el caracter nulo
   
   first = 0; //se guarda en first el primer índice de la cadena

   /*Bucle para intercambiar los caracteres*/
   while (first < last) { 
      temp = str[first]; //guarda temporalmente el caracter en la posición first
      str[first] = str[last];  //Pone el caracter en la posición final en la posición inicial
      str[last] = temp; //Pone el caracter guardado temporalmente en la posición final
      /*Se actualizan los índices para el siguiente intercambio*/
      first++; 
      last--; 
   } 
}