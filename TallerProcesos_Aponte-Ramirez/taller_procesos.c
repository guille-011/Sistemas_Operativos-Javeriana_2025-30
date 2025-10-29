/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: Implementación de procesos con fork() y comunicación con pipe()
 * Archivo: taller_procesos.c
 *
 * 
 * Fecha: 30/10/25
 *
 * Descripción general:
 *   Programa principal que lee dos archivos de enteros, crea una jerarquía
 *   de procesos (padre a primer hijo a nieto, y padre a segundo hijo)
 *   y utiliza pipes para comunicar sumas parciales entre procesos.
 *
 * Flujo de ejecución resumido:
 *   1. El padre lee dos archivos de enteros (N1 valores en archivo00, N2 en archivo01).
 *   2. Se crean dos pipes: pipe1 (comunicación nieto/segundo hijo con primer hijo)
 *                          pipe2 (comunicación nieto/segundo hijo/primer hijo con padre).
 *   3. Se hacen los forks:
 *        - Primer fork: crea primer hijo.
 *        - Si estoy en primer hijo: hago fork para crear al nieto.
 *        - Si estoy en padre: hago fork para crear el segundo hijo.
 *   4. El nieto calcula sumaA (suma de archivo00) y escribe en pipe1 y pipe2.
 *   5. El segundo hijo calcula sumaB (suma de archivo01) y escribe en pipe1 y pipe2.
 *   6. El primer hijo lee dos enteros desde pipe1 (primeraSuma y segundaSuma),
 *      calcula sumaTotal = sumarValores(primeraSuma, segundaSuma) y escribe en pipe2.
 *   7. El padre lee desde pipe2 los valores Asuma, Bsuma y sumaFinal y los imprime.
 *
 * Entradas (argv):
 *   argv[0] -> nombre del ejecutable
 *   argv[1] -> N1 (cantidad de enteros en primer archivo)
 *   argv[2] -> nombre del primer archivo (archivo00)
 *   argv[3] -> N2 (cantidad de enteros en segundo archivo)
 *   argv[4] -> nombre del segundo archivo (archivo01)
 *
 * Salidas:
 *   - Mensajes por stdout mostrando:
 *       "Proceso Padre:"
 *       "Suma Final de los arreglos: %d"
 *       "Suma A: %d, Suma B: %d"
 *   - En caso de error se usan perror(...) y exit(EXIT_FAILURE).
 **********************************************************************/

#include <stdio.h>  // Entrada y salida estándar (printf, fgets)
#include <stdlib.h> // Funciones generales de utilidad (exit, malloc)
#include <sys/types.h> // Definiciones de tipos de datos como pid_t
#include <sys/wait.h> //usar el equivalente a join() (wait()) pero en fork()
#include <unistd.h> // Funciones de manejo de procesos y pipes (read, write, close)

#include "modulo_procesos.h" //Se incluye el archivo con la declaración de las funciones

/*Funcion principal donde se desarrolla el programa solicitado en el taller*/

int main(int argc, char *argv[]) {
  /*Si al momento de ejecutar el programa se le envian menos de 5 argumentos se imprime un mensaje de error y se cierra el programa.
  Los argumentos esperados son los siguientes:
  
  0-El nombre del ejecutable
  1-El tamaño del primer archivo
  2-El nombre del primer archivo
  3-El tamaño del segundo archivo
  4-El nombre del segundo archivo*/
  if (argc < 5) { 
    printf("Error: Número insuficiente de argumentos.\n");
    return 1;
  }


  int N1 = atoi(argv[1]); //Se declara la variable entero N1 y se le asigna la conversion a entero del segundo argumento recibido por el main
  char *archivo00 = argv[2]; //se declara la variable apuntador a char archivo00 y se le asigna el tercer argumento recibido por el main
  int N2 = atoi(argv[3]); //Se declara la variable entero N2 y se le asigna la conversion a entero del cuarto argumento recibido por el main
  char *archivo01 = argv[4]; //se declara la variable apuntador a char archivo01 y se le asigna el quinto argumento recibido por el main
  int sumaTotal; //Se declara la variable sumaTotal de tipo entero
  int sumaB; //Se declara la variable sumaB de tipo entero
  int sumaA; //Se declara la variable sumaA de tipo entero

  /*Se declaran dos apuntadores a entero, datos00
  y datos01, encargados de contener los datos guardados en los archivos a manera de arreglos.
  Usando la función malloc, se reserva la cantidad de memoria correspondiente al total de enteros contenidos 
  en sus respectivos archivos, para datos00, se reserva la cantidad de N1 enteros y para datos01 la cantidad
  de N2 enteros */
  int *datos00 = malloc(N1 * sizeof(int)); 
  int *datos01 = malloc(N2 * sizeof(int));

  /*Se llama la función leerArchivo dos veces, una para guardar el contenido de archivo00 en el arreglo datos00
  y otra para guardar el contenido de archivo01 en el arreglo datos01*/
  leerArchivo(archivo00, N1, datos00);
  leerArchivo(archivo01, N2, datos01);

  /*Se declaran tres variables de tipo pid_t, que es un tipo de variable dedicadas unicamente a guardar el ID de un proceso
  estas variables son:
  
  pPrimerHijo: Proceso encargado de calcular la suma total desde los dos arreglos
  pGrandHijo: Proceso encargado de calular sumaA, proveniente del archivo00
  pSegundoHijo: Proceso encargado de calcular sumaB, proveniente del archivo01
  */
  pid_t pPrimerHijo = -1;
  pid_t pGrandHijo = -1;
  pid_t pSegundoHijo = -1;

  /*Se declaran dos arreglos que enteros que contendrán dos pipes utilizados para comunicar procesos en el código
  luego de declarar estos arreglos, se le envían como parámetro a la función crearPipe definida en modulo_procesos.c,
  para que en esta función se creen los pipes como tal y se almacenen en los arreglos acá declarados
  
  El pipe1 es el utilizado para enviar mensajes desde el proceso nieto y desde el segundo hijo para que los lea
  el primer hijo
  El pipe2 es el utilizado para enviar mensajes desde los dos hijos y desde el nieto para que los lea el proceso
  padre*/
  int pipe1[2];
  int pipe2[2];
  crearPipe(pipe1);
  crearPipe(pipe2);



/*Se invoca la función fork(), la cuál sirve para la creación de un proceso, se guarda su valor de retorno en una variable pPrimerHijo.
    Los posibles retornos de fork son:
    0, proceso hijo
    > 0, proceso padre, el valor es el PID del hijo
    -1, error de creación */
  pPrimerHijo = fork();

  /*Si la función fork retorna -1, se indica que el error fue creando al primer hijo y se sale del programa con un error*/
  if (pPrimerHijo == -1) {perror("Error creando el primer hijo");exit(EXIT_FAILURE);}
  /*Si pPrimerHijo guarda 0, es porque el programa se encuentra ejecutando al primer hijo, por lo tanto se realizan las primeras operaciones
  correspondientes para este hijo, es decir crear a su propio hijo*/
  if (pPrimerHijo == 0) {
    pGrandHijo = fork(); //Se utiliza la función fork nuevamente para crear un proceso hijo del primer hijo, es decir, un nieto del proceso padre, guardado en pGrandHijo

    /*Si la función fork para la creación de pGrandHijo retorna -1, se infica que el error fue creando al proceso nieto y se 
    abandona el programa con un error*/
    if (pGrandHijo == -1) {perror("Error creando el nieto (hijo del primer hijo)");exit(EXIT_FAILURE);}

  /*Si pPrimerHijo guarda un valor mayor a 0 es porque el programa se encuentra ejecutando el proceso padre, por lo tanto se realizan las 
  primeras operaciones que debe hacer el padre, es decir crear al segundo hijo*/
  } else if (pPrimerHijo != 0) {
    pSegundoHijo = fork(); //Se usa la función fork para crear el segundo hijo del proceso padre, guardado en pSegundoHijo

    /*Si la función fork para la creación de pSegundoHijo retorna -1, se infica que el error fue creando al segundo hijo y se 
    abandona el programa con un error*/
    if (pSegundoHijo == -1) {perror("Error creando el segundo hijo");exit(EXIT_FAILURE);}
  }



  
  if (pPrimerHijo == 0) { //Se identifica que el programa se encuentre ejecutando al primer hijo
    if (pGrandHijo == 0) { //Luego de identificar que esté en el primer hijo, se verifica si está en el proceso nieto

      /*Si el programa se encuentra en el proceso nieto, se cierran los extremos de lectura de ambos pipes*/
      close(pipe1[0]);
      close(pipe2[0]);
       /*Se calcula la sumaA usando la función sumarArreglo definida en modulo_procesos.c y se le envían como parámetros
       el arreglo que contiene la información de archivo00 y la cantidad de datos que tiene este arreglo*/
      sumaA = sumarArreglo(datos00, N1); 

      /*Se llama a la función write, dos veces se le envia como parámetros los extremos de escritura de los pipes,
        &sumaA, indicando que será en esta direccion de memoria de donde se escribirá el mensaje y sizeof(int), indicando que se escribirá la cantidad
        de bytes correspondiente a un entero.
        
        Los retornos de write son:
        >0 ====== Número de bytes realmente escritos
        -1 ====== Error de escritura
        
        Por lo tanto, si la función retorna -1, se imprime un mensaje de error indicando en cuál pipe ocurrió
        y se cierra el programa con un error*/
      if (write(pipe1[1], &sumaA, sizeof(int)) == -1) {perror("Error del nieto escribiendo en el pipe1");exit(EXIT_FAILURE);}
      if (write(pipe2[1], &sumaA, sizeof(int)) == -1) {perror("Error del nieto escribiendo en el pipe2");exit(EXIT_FAILURE);}

      /*Se cierran los extremos de escritura de ambos pipes*/
      close(pipe1[1]);
      close(pipe2[1]);
    } else { //Se verifica que el programa esté en el primer hijo pero no en el nieto
      close(pipe1[1]); //Se cierra el extremo de escritura del pipe1
      int primeraSuma; //Se declara la variable primeraSuma de tipo entero
      int segundaSuma; //Se declara la variable segundaSuma de tipo entero

      /*Se llama a la función read dos veces, se le envia como parámetros pipe1[0], indicando que debe mirar el extremo de lectura del primer pipe,
        en el primer llamado se le envia &primeraSuma y en el segundo se le envia &segundaSuma,
        indicando que será en estas direcciones de memoria donde se guardará lo leído del pipe
        y sizeof(int), indicando que se leerá la cantidad
        de bytes correspondiente a un entero.
        
        El retorno de la función read, se guarda en lecturaB, siendo sus posibles retornos los siguientes:
        >0 ====== Número de bytes leídos correctamente
        0 ===== fin de datos, el otro proceso cerró el pipe
        -1 ====== Error de lectura
        
        Por lo tanto, si la función retorna -1, se imprime un mensaje de error indicando en cuál pipe ocurrió
        y se cierra el programa con un error*/
      if (read(pipe1[0], &primeraSuma, sizeof(int)) == -1) {perror("Error del primer hijo leyendo del pipe1");exit(EXIT_FAILURE);}
      if (read(pipe1[0], &segundaSuma, sizeof(int)) == -1) {perror("Error del primer hijo leyendo del pipe1");exit(EXIT_FAILURE);}
      close(pipe1[0]); //Se cierra el extremo de lectura del pipe

      /*Usando los enteros leídos del pipe y guardados en primeraSuma y en segundaSuma, se le envían como parámetro
      a la función sumarValores y se guarda su retorno en la función sumaTotal*/
      sumaTotal = sumarValores(primeraSuma, segundaSuma);

      close(pipe2[0]); //Se cierra el extremo de lectura del pipe2

      /*Se usa la función usleep para dar un retardo de 100 milisegundos antes de escribir en el pipe2, de tal forma, el primer
      hijo es el último proceso que escribe en el pipe*/

      usleep(100000); 
      /*Se llama a la función write, se le envia como parámetros el extremo de escritura de pipe2,
        &sumaTotal, indicando que será en esta direccion de memoria de donde se escribirá el mensaje y sizeof(int), indicando que se escribirá la cantidad
        de bytes correspondiente a un entero.
        
        si la función retorna -1, se imprime un mensaje de error indicando en cuál pipe ocurrió
        y se cierra el programa con un error*/
      if (write(pipe2[1], &sumaTotal, sizeof(int)) == -1) {perror("Error del primer hijo escribiendo en el pipe2");exit(EXIT_FAILURE);}
      close(pipe2[1]);
    }
  } else if (pPrimerHijo != 0) { //Se verifica que el programa no esté ejecutando al primer hijo
    if (pSegundoHijo == 0) { //Luego de verificar que el programa no está ejecutando al primer hijo, se identifica si se está ejecutando el segundo

       /*Si el programa se encuentra en el segundo hijo, se cierran los extremos de lectura de ambos pipes*/
      close(pipe1[0]);
      close(pipe2[0]);
      /*Se calcula la sumaB usando la función sumarArreglo definida en modulo_procesos.c y se le envían como parámetros
       el arreglo que contiene la información de archivo01 y la cantidad de datos que tiene este arreglo*/
      sumaB = sumarArreglo(datos01, N2);

      /*Se llama a la función write, se le envia como parámetros el extremo de escritura de pipe1,
        &sumaB, indicando que será en esta direccion de memoria de donde se escribirá el mensaje y sizeof(int), indicando que se escribirá la cantidad
        de bytes correspondiente a un entero.
        
        si la función retorna -1, se imprime un mensaje de error indicando en cuál pipe ocurrió
        y se cierra el programa con un error*/
      if (write(pipe1[1], &sumaB, sizeof(int)) == -1) {perror("Error del segundo hijo escribiendo en el pipe1");exit(EXIT_FAILURE);}
      close(pipe1[1]); //Se cierra el extremo de lecetura de pipe1
      
      /*Se usa la función usleep para dar un retardo de 50 milisegundos antes de escribir en el pipe2, de tal forma, el segundo
      hijo es el segundo proceso que escribe en el pipe*/
      usleep(50000); 
       /*Se llama a la función write, se le envia como parámetros el extremo de escritura de pipe2,
        &sumaB, indicando que será en esta direccion de memoria de donde se escribirá el mensaje y sizeof(int), indicando que se escribirá la cantidad
        de bytes correspondiente a un entero.
        
        si la función retorna -1, se imprime un mensaje de error indicando en cuál pipe ocurrió
        y se cierra el programa con un error*/
      if (write(pipe2[1], &sumaB, sizeof(int)) == -1) {perror("Error del segundo hijo escribiendo en el pipe2");exit(EXIT_FAILURE);}
      close(pipe2[1]); //Se cierra el extremo de lecetura de pipe2
    } else { //Se verifica que el programa se encuentre ejecutando al proceso padre
      close(pipe1[0]); //Se cierra el extremo de lectura del pipe1
      close(pipe1[1]); //se cierra el extremo de escritura del pipe1
      close(pipe2[1]); //se cierra el extremo de escritura del pipe2
      int sumaFinal; //se declara una variable que guarde el valor de la suma final
      int Asuma, Bsuma; //se declaran dos variables que guardes los valores de sumaA y sumaB, llamándolas diferente para evitar errores de compilación

      /*Se llama a la función read tres veces, se le envia como parámetros pipe2[0], indicando que debe mirar el extremo de lectura del segundo pipe,
        en el primer llamado se le envia &Asuma, en el segundo se le envia &Bsuma y en el tercero &sumaFinal,
        indicando que será en estas direcciones de memoria donde se guardará lo leído del pipe
        y sizeof(int), indicando que se leerá la cantidad
        de bytes correspondiente a un entero.
        
        Si la función retorna -1, se imprime un mensaje de error indicando en cuál pipe ocurrió
        y se cierra el programa con un error*/
      if (read(pipe2[0], &Asuma, sizeof(int)) == -1) {perror("Error del padre leyendo del pipe2");exit(EXIT_FAILURE);}
      if (read(pipe2[0], &Bsuma, sizeof(int)) == -1) {perror("Error del padre leyendo del pipe2");exit(EXIT_FAILURE);}
      if (read(pipe2[0], &sumaFinal, sizeof(int)) == -1) {perror("Error del padre leyendo del pipe2");exit(EXIT_FAILURE);}
      close(pipe2[0]); //Se cierra el extremo de lectura del pipe2

      /*Se imprimen varios mensajes según lo solicitado.
      
      En primer lugar se indica que se está imprimiento el mensaje desde el proceso padre
      Luego se imprime el resultado de la suma final de los arreglos
      Finalmente se imprimen los resultados de sumaA y de suma B*/
      printf("Proceso Padre:\n");
      printf("Suma Final de los arreglos: %d\n", sumaFinal);
      printf("Suma A: %d, Suma B: %d\n", Asuma, Bsuma);
    }
  }

  /*Se verifica que el programa se encuentre ejecutando el primer hijo, en caso de que esto pase,
  se utiliza la función wait,
  
  La función wait se usa únicamente para hacer que un proceso padre espera a que termine su hijo,
  se le envia como parámetro un apuntador a entero donde se quiera guardar el código de salida del hijo,
  pero como en este caso no resulta importante conocer esto, simplemente se le envía NULL
  */
  if (pPrimerHijo == 0 && pGrandHijo > 0) {wait(NULL);} //el primer hijo espera a al nieto 

  /*Se llama a la función wait dos veces, sin indicarle que proceso la debe ejecutar, de tal forma
  se usan para hacer que el proceso padre espere a que terminen sus dos hijos*/
  wait(NULL); 
  wait(NULL);


  free(datos00); //Se libera la memoria reservada para datos00
  free(datos01); //Se libera la memoria reservada para datos01



  
  return 0;
}