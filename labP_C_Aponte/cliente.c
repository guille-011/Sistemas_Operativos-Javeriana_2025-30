/******************************************************************* 
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Implementación del patrón Productor-Consumidor usando Pipe Named (FIFO)
 * Archivo: cliente.c
 *
 * Descripción general:
 *    Este programa actúa como cliente en un esquema de comunicación
 *    entre procesos mediante un pipe nombrado (FIFO). Permite enviar
 *    mensajes de texto al proceso servidor y recibir una respuesta
 *    de manera secuencial.
 *
 * Funcionalidades principales:
 *    - Solicita al usuario ingresar mensajes por teclado.
 *    - Envía cada mensaje al servidor a través del pipe.
 *    - Recibe la respuesta del servidor y la muestra en pantalla.
 *    - Permite finalizar el programa al ingresar "end".
 *
 * Dependencias:
 *    - Librerías estándar de C: stdio.h, stdlib.h, string.h, unistd.h
 *    - Librerías de manejo de archivos y pipes: sys/types.h, sys/stat.h, fcntl.h
 *
 * Notas:
 *    - Se recomienda ejecutar primero el servidor que crea y lee del FIFO.
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

int main() {
    int fd_write, fd_read; //se declaran dos enteros que guardarán el valor retornado al abrir las pipes de lectura y escritura
    int end_process; //se declara un entero que guardará el valor de la comparación entre la cadena ingresada y la cadena "end"
    int read_bytes; //se declara un entero que guardará la cantidad de bytes leídos desde la pipe
    char readbuf[80]; //se declara el buffer que guardará la cadena ingresada por el usuario y la cadena recibida desde el servidor
    char end_str[] = "end"; //se declara la cadena que indicará el fin del proceso

    printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");

    /*Se inicializan las variables fd_write y fd_read con la función open de sus respectivos pipes,
    fd_write accede a FIFO_CS, que es la que envia mensajes desde cliente a servidor, por ende esta función
    se inicializa como O_WRONLY, es decir, UNICAMENTE de escritura.
    fd_read accede a FIFO_SC, que es la que envia mensajes desde el servidor al cliente, por ende, se inicializa
    como O_RDONLY, es decir, UNICAMENTE de lectura

    Las variables guardan el "Descriptor de archivo" retornado por la función open, el descriptor de archivo es un número entero
    que el sistema operativo usa para identificar cualquier recurso que pueda leer o escribir, de tal manera puede acceder
    al pipe necesario, fd_write guarda FIFO_CS y fd_read guarda FIFO_SC
    Si se retorna -1 en la función open, quiere decir que hubo un error abriendo el pipe*/  
    fd_write = open(FIFO_CS, O_WRONLY); 
    fd_read  = open(FIFO_SC, O_RDONLY);

    /*Se verifica que no haya habido errores abriendo el pipe, en caso de que si, se envia un mensaje de error*/
    if (fd_write < 0 || fd_read < 0) {
        perror("Error opening FIFOs");
        exit(1);
    }

    /*Se crea un ciclo que se repita INFINITAMENTE o, hasta que el usuario ingrese la palabra "end"
    Mientras que la palabra sea diferente a end, se escribirá en el buffer lo que sea que haya ingresado el usuario,
    posteriormente se le enviará al servidor mediante el pipe.
    Si la palabra es diferente a end, se lee la respuesta del servidor y se muestra por pantalla tanto el mensaje
    enviado como el mensaje recibido del servidor.
    Si la palabra es end, se lee nuevamente del pipe el mensaje enviado por el servidor, se cierran las pipes
    y se termina el ciclo*/

    while (1) {
        printf("Enter string: ");
        /*se verifica que se lea correctamente el mensaje, si fgets retorna NULL, es decir, si no se lee bien el mensaje
        se termina el ciclo*/
        if (!fgets(readbuf, sizeof(readbuf), stdin)) break; 
        int stringlen = strlen(readbuf); //se inicializa la variable stringlen con la longitud de la cadena ingresada
        /*Cuando se realiza la lectura en fgets, se guarda el enter (\n) ingresado por el usuario, esto puede generar errores
        por ende, se reeemplaza este enter por el caracter nulo, para que el servidor no lea el salto de línea*/
        readbuf[stringlen - 1] = '\0'; 
        end_process = strcmp(readbuf, end_str); //se inicializa la variable end_process con el resultado de la comparación entre la cadena ingresada y "end"

        if(end_process != 0){ //Se entra al ciclo si end_process es diferente de 0, es decir, si la cadena ingresada es diferente a "end"
            
            /*Se llama a la función write, la cuál recibe como parámetro fd_write, es decir, el pipe que envia el mensaje
            al servidor, readbuf, el mensaje que se enviará y strlen(readbuf), que es el tamaño de dicho mensaje*/
            write(fd_write, readbuf, strlen(readbuf));
            /*Se imprime por pantalla el mensaje enviado y su tamaño*/
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
            /*Se llama a la función read, la cuál recibe fd_read, que es el pipe que recibe el mensaje enviado desde el servidor,
            readbuf, que es la cadena donde se guardará el mensaje y sizeof(readbuf) que es el tamaño máximo de esa
            cadena.
            
            El retorno de read es la cantidad real de bytes leídos desde el pipe, este valor se guarda en read_bytes*/
            read_bytes = read(fd_read, readbuf, sizeof(readbuf)); 
            readbuf[read_bytes] = '\0'; //se agrega el caracter nulo al final de la cadena leída para evitar errores al imprimirla
            /*Se imprime por pantalla el mensaje recibido desde el servidor y su tamaño*/
            printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));

        }else { 

         /*En caso de que el usuario escriba end, se usa la función write para enviarle la cadena al servidor
         de esta forma, puede saber que el usuario quiere terminar el proceso*/
         write(fd_write, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf)); //se imprime por pantalla el mensaje enviado y su tamaño
         
         /*Se usa la función close para cerrar los pipes, la función libera los descriptores de archivo y cuerra la comunicación*/
         close(fd_write);
         close(fd_read); 
         break; //termina el ciclo si el usuario ingresó "end"
        }
       } 
       return 0;
}
