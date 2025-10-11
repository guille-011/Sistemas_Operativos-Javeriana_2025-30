/*********************************************************
*Pontificia Universidad Javeriana
*Autor: Guillermo Aponte
*Fecha_ 09/10/2025
*Materia: Sistemas Operativos
*Tema: Creación de procesos fork
*   -identificación del proceso padre e hijo 
*   -Procesos duplicados
*   -Comunicación entre procesos con pipe()
*********************************************************/


#include <stdio.h> //Libreria implementada para usar funciones de entrada y salida como printf
#include <stdlib.h> //Funciones generales como exit(), malloc(), etc.
#include <unistd.h> //Libreria POSIX: contiene fork(), getpid(), getppid()
#include <string.h> //Manejo de cadenas de caracteres



int main(){
    /*Creacion de tubería de comunicación, sirve como buffer en memoria del kernel para comunicación entre procesos emparentados, denominados como (IPC)
    la tubería puede tener diferentes valores según el uso que se le vaya a dar:
    pipefd[0] ===== extremo de lectura
    pipefg[1] ===== extremo de escritura */
    int pipefd[2];

    /*Variable para guardar el PID del proceso, la variable está declarada como pid_t, tipo de dato definido especificamente para representar el ID
    de un proceso, casi siempre es un entero internamente, pero se define así por POSIX para garantizar portabilidad entre sistemas UNIX/Linux*/
    pid_t pHijo_PID; 
    /*se llama a la función pipe, para crear la tubería de comunicación, se le manda como parámetro el arreglo de enteros definido anterioemente, pues en este se
    contendrán los descriptores del archivo pipe, automáticamente, después de llamarlo, se verifica que no se produzca error, pues si esta función retorna -1
    es porque no se pudo crear el pipe*/
    if(pipe(pipefd) == -1){
        perror("PIPE"); //se imprime el error "PIPE" en caso de que no se pueda crear la tubería
        exit(EXIT_FAILURE); //se termina el programa con un código de error
    }
    /*Se invoca la función fork(), la cuál sirve para la creación de un proceso, se guarda su valor de retorno en una variable pHijo_PID
    Los posibles retornos de fork son:
    0, proceso hijo
    > 0, proceso padre, el valor es el PID del hijo
    -1, error de creación */
    pHijo_PID = fork();
    /*si el valor de pHijo_PID es menor a 0, se evidencia un error en la creación del proceso, por lo tanto se imprime un mensaje de error*/
    if(pHijo_PID == -1){
        perror("FORK"); //se indica que el error es de la función FORK
        exit(EXIT_FAILURE); //se termina el programa con un código de error
    }
    /*Condicional para verificar si el programa se encuentra en el proceso hijo, pues fork() retorna 0 en este caso*/
    if(pHijo_PID== 0){
        /* El hijo solo va a LEER, así que cierra el extremo de escritura */

        close(pipefd[1]); //Funcion que cierra el extremo de escritura del pipe
        /*Capacidad de vector caracteres, representa la memoria local del hijo, es decir, el tamaño del mensaje que resivirá*/
        char mensaje[100]; 

        /*Se llama a la función read, se le envia como parámetros pipefd[0], indicando que debe mirar el extremo de lectura del pipe,
        mensaje, indicando que será en esta dirección de memoria donde se guardará el mensaje y sizeof(mensaje), indicando que esta es la cantidad máxima de bytes
        que se leerán, para prevenir un desborde de memoria por leer muchos bytes.
        
        El retorno de la función read, se guarda en lecturaB, siendo sus posibles retornos los siguientes:
        >0 ====== Número de bytes leídos correctamente
        0 ===== fin de datos, el otro proceso cerró el pipe
        -1 ====== Error de lectura*/
        int lecturaB = read(pipefd[0], mensaje, sizeof(mensaje)); 
        /*Verificacion de lectura del mensaje, si la variable lecturaB guardó -1, quiere decir que hubo un error de lectura*/
    if(lecturaB == -1){
        perror("LECTURA"); //se indica que el error es de lectura
        exit(EXIT_FAILURE); //se termina el programa con un código de error
    }

    /*Impresión del mensaje leido, se usa %.*s, para indicar que se imprimirá una cadena de caracteres, pero con una longitud específica, de la sigueinte forma:
    %Indica una espicificación de formato a imprimir
    . Indica que habrá precisión en dicha impresión, es decir un límite de caracteres
    * hace referencia a la cantidad de caracteres que se imprimirá, lo cuál se envia como parámetro
    s Indica que el dato impreso será una cadena de caracteres
    lecturaB se envía como parámetro, pues esta guarda el retorno de la función read, es decir, el número de bytes leídos
    mensaje se envía como parámetro, pues es la cadena que guardó la cadena leída por la función read*/
    
    printf("HIJO recibe: ==# %.*s #==\n", lecturaB, mensaje);
    close(pipefd[0]); //Cierre de entrada para lectura, pues el proceso hijo ya terminó de leer el mensaje
        
    
    /*Condicional para verificar si el programa se encuentra en el proceso padre, pues fork() retorna el id del proceso del hijo en este caso*/

    }else{

        /*envio del mensaje desde el padre*/
         /* El padre solo va a ESCRIBIR, así que cierra el extremo de lectura */

        close(pipefd[0]); //Funcion que cierra el extremo de escritura del pipe
        /*Se declara una cadena de caracteres llamada mensaje y se guarda el mesaje que se enviará por el pipe*/
        char mensaje[] = "Hijo soy tu papa, trae el pan y las gaseosas"; 
        /*Se llama a la función write, se le envia como parámetros pipefd[1], indicando que debe usar el extremo de escritura del pipe,
        mensaje, indicando que será en esta direccion de memoria de donde se escribirá el mensaje y strlen(mensaje), indicando que esta es la cantidad de bytes
        que se escribirán.
        
        El retorno de la función write, se guarda en escrituraB, siendo sus posibles retornos los siguientes:
        >0 ====== Número de bytes realmente escritos
        -1 ====== Error de escritura*/
        int escrituraB = write(pipefd[1], mensaje, strlen(mensaje));
        /*Verificacion de escritura del mensaje, si la variable escrituraB guardó -1, quiere decir que hubo un error de escritura*/
        if(escrituraB == -1){
        perror("ESCRITURA"); //se indica que el error es de escritura
        exit(EXIT_FAILURE); //se termina el programa con un código de error
    }
    close(pipefd[1]); //Cierre de salida para escritura, pues el proceso padre ya terminó de escribir el mensaje
    }
    
    /*printf ejecutado por ambos procesos, pues no se indica en cuál se debe ejecutar
    se imprime dos veces, una desde el proceso hijo y otra desde el padre */
    printf("Fin del programa \n");

    return 0; //ambos procesos retornan al sistema operativo 0, indicando que todo salió bien
}

