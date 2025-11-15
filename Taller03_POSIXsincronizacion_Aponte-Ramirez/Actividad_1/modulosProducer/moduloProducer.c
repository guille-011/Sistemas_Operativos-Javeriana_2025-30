/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: IPC con named Semaphores y Sahred Memory
 * Archivo: moduloProducer.c
 * Descripcion: Implementa las funciones del módulo productor 
 *              encargadas de crear y configurar los semáforos
 *              con nombre, crear y dimensionar el bloque de 
 *              memoria compartida, mapear dicha región para 
 *              escribir secuencialmente en el búfer circular
 *              empleando sincronización con semáforos vacio/lleno
 *              y, finalmente, desmapear la memoria y eliminar 
 *              todos los recursos de IPC utilizados.
 * Fecha: 14/11/25
 ******************************************************************/
#include "moduloProducer.h"

/*Función que crea un semáforo retorna el puntero a este semáfoto*/
sem_t* crearSemaforo(const char* nombre, unsigned int valorInicial){
    /*Proceso que guarda un semáforo o lo crea de la siguiente forma:
    
    -sem_t: Tipo de dato que representa a un semáfoto en POSIX, de tal manera se puede declarar
    una variable relacionada con esto.
    -*sem: Apuntador de memoria a la variable de tipo sem_t, que representa un semáforo
    -sem_open: Función que abre o crea un semáfoto de POSIX, recibe los siguientes parámetros:
        -1. const char *name: Nombre que recibirá el semáforo en el sistema, debe empezar con '/'
        así, varios procesos pueden abrir el mismo semáforo si usan el nombre dado, en este caso, el
        nombre usado es "/vacio".
        -2. int oflag: Banderas que indican cómo se debe abrir el semáforo, algunos ejemplos de las que se le
        pueden enviar son las siguientes: 
            O_CREAT: crea el semáfoto si no existe
            O_EXCEL: Se combina con O_CREAT, lanza error si ya existe.
            O_RDWR: Permite abrir el semáforo para lectura o escritura, en este caso se ignora pues no se suele
            hacer esto en POSIX.
            O_TRUNC: Resetear los semáforos
            0: No crear, solo abrir.
        Para el caso de esta aplicación se usa O_CREATE
        -3. mode_t mode: Permisos que tendrá el semáforo, en este caso 0644, siendo 
            6: Lectura y escritura para el dueño
            4: Solo lectura para el grupo
            4: Solo lectura para otros usuarios

        -4: unsigned int value: Valor inicial del semáforo, en este caso se usa valorInicial
        sem_open devuelve un puntero a sem_t (semáforo) si todo salió bien, en caso de que ocurra un error devuelve:
        (sem_t *)SEM_FAILED

    */
    sem_t* sem = sem_open(nombre, O_CREAT, 0644, valorInicial);

    /*En caso de que el retorno de los semáforos sea un mensaje de error se imprime por consola que ocurrió un error abriendo
    alguno de los dos y se termina la ejecución del programa*/
    if (sem == SEM_FAILED) 
    { 
        perror("sem_open");
         exit(EXIT_FAILURE); 
        }
        return sem;
 }

/*Función que abre el bloque de memoria compartida en la variable enviada como parámetro*/
void abrirMemoriaCompartida(int *shm_fd){
    /*Proceso que crea o abre un objeto de memoria compartida en la memoria RAM de 
    la siguiente forma:
    
    -shm_fd: Variable de tipo entero que guarda un descriptor de archivo, es decir, un número entero que el sistema operativo guarda
    para saber cuál es el bloque de meomoria compartida al que se quiere acceder, su valor depende del sistema operativo
    
    -shm_open: Función que crea el bloque de memoria compartida en el kernel, recibe lo siguientes parámetros:
        -1. const char *name: Nombre del objeto de memoria compartida, debe empezar con '/', en este caso se usa "/memoria_compartida"
        -2. int oflag: Banderas que indican cómo se debe abrir el objeto, en este caso se usa O_CREAT | O_RDWR, que indica que se debe crear
        el objeto si no existe y que se abre para lectura y escritura.
        -3. mode_t mode: Permisos que tendrá el objeto de memoria compartida, siendo
            6: Lectura y escritura para el dueño
            4: Solo lectura para el grupo
            4: Solo lectura para otros usuarios

        shm_open devuelve un descriptor de archivo (int) si todo salió bien, en caso de que ocurra un error devuelve -1        
        
        */
    *shm_fd = shm_open("/memoria_compartida", O_CREAT | O_RDWR, 0644);


    /*Se verifica que el bloque de memoria compartida se haya creado correctamente*/
    if (*shm_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /*Función que reajusta el tamaño de la memoria compartida en el kernel, pues si no se realiza esto, 
    la memoria señalada por shm_f tendría un valor útil de 0, resultadno inútil si se quiere operar después.
    
    La función recibe como primer parámetro el descriptor de archivo del bloque creado con shm_open y como segundo parámetro
    el tamaño en bytes que se le quiere dar, en este caso se le envia sizeof de la estructura compartir_datos, teniendo esta
    los siguientes parámetros_
    
    int bus[BUFFER]
    int entrada
    int salida
    
    */
    ftruncate(*shm_fd, sizeof(compartir_datos));
}

/*Función que mapea el bloque y realiza el proceso de producción de los mensajes enviados por el pipe*/
void mapearYProducir(compartir_datos** compartir, sem_t* vacio, sem_t* lleno, int shm_fd){
    /*Función utilizada para conectar el espacio de memoria reservado en la RAM para la comunicación y redimensionado con
    ftruncate con el código, de tal forma que se pueda efectuar una comunicación efectiva, determinando lo que se le quiere enviar
    y lo que se quiere recibir según sea el caso
    
    Recibe los siguientes parámetros:
        -1. void *addr: se envía NULL para indicarle al sistema operativo que se puede asignar esta memoria de manera libre,
        en caso de querer especificar una dirección exacta, se le puede enviar un puntero declarado previamente.
        -2. size_t length: Indica el tamaño total de la estructura que se quiera compartir, en este caso se le envia sizeof(compartir_datos),
        pues esta será la estructura que se compartirá.
        -3: int prot: Define los permisos del proceso en esta zona de memoria, en este caso se le envía PROT_READ | PROT_WRITE, que indica que se puede leer 
        y escribir en esta zona. 
        -4. int flags: Se indica como influirán los cambios realizados en este proceso en relación con los demás procesos, en este caso se le envía MAP_SHARED,
        indicando que los cambios que se hagan en este proceso se verán reflejados en los demás que accedan al bloque compartido, en caso de que no se quisiera 
        compartir estos cambios, se le podria enviar MAP_PRIVATE.
        -5. int fd: Descriptor de archivo del bloque de memoria compartida creado con shm_open, para indicarle a mmap el bloque que debe mapear.
        -6. off_t offset: Desplazamiento dentro del bloque de memoria compartida, en este caso se le envía 0 para indicar que se empieza desde el inicio.

        mmap devuelve un puntero a la zona de memoria mapeada si todo salió bien, en caso de que ocurra un error devuelve: void( *)MAP_FAILED.
        El retorno de mmap se guarda en el apuntador compartir de tipo compartir_datos, de tal forma que los cambios realizados se vean reflejados en esta
        dirección de memoria, como se envia como apuntador doble para qu elos cambios se guarden en su dirección de memoria, es necesario realizar un casteo.   
    */
    *compartir = (compartir_datos*) mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    (*compartir)->entrada = 0; //Inicializa en 0 la varaible entrada de buffer

    /*Cilo desde 1 a 10 donde se realizan los correspondientes cambios a la estructura compartida "compartir",
    complementándolo con la utilización de semáforos para que el proceso se realice correctamente*/
    for (int i = 1; i <= 10; i++) {

        /*Usa la función sem_wait, y le envia la variable vacio.
        
        Vacio guarda lo siguiente:
        
        Un contador que representa cunatos espacios vacíos hay en el buffer
        Lista de procesos dormidos
        Identificador del semáforo
        
        Siendo asi la función hace lo siguiente:
        Por cada iteración, se llama a sem_wait, si la variable vacio tiene un contador mayor a 0, se decrementa en 1,
        si el contador es igual a 0, el proceso se queda esperando hasta que otro proceso realice sem_post(vacio), liberando un
        espacio y permitiendole seguir*/
        sem_wait(vacio);
        (*compartir)->bus[(*compartir)->entrada] = i; //Se guarda en la posición iterada de compartir->bus el valor de i
        printf("Productor: Produce %d\n", i); //Se imprime i por consola
        (*compartir)->entrada = ((*compartir)->entrada+1) % BUFFER; //Se aumenta en 1 la variable entrada de buffer, usando módulo para que no se salga del tamaño del buffer
        
        /*Usa la función sem_post y le envia la varaible lleno.
        
        Lleno al ser un semáforo diferente a vacio, guarda lo mismo.
        
        Al usar la función sem_post, se libera un espacio en el semáforo apuntado por lleno, es decir, aumenta su contador interior,
        permitiéndole a un proceso que esté esperando seguir realizando su tarea*/
        sem_post(lleno);
        sleep(2);  //se da un pequeño tiempo de espera
    }
}

/*Función que libera los recursos utilizados*/
void liberarRecursos(compartir_datos* compartir, int shm_fd, sem_t* vacio, sem_t* lleno){
     munmap(compartir, sizeof(compartir_datos)); //desmapea la memoria compartida, enviándole el apuntador que lo estaba haciendo y su tamaño
    close(shm_fd); // Cierra el descriptor de archivo del bloque de memoria compartida
    sem_close(vacio); // Cierra el semáforo apuntado por vacio
    sem_close(lleno); // Cierra el semáforo apuntado por lleno  
	sem_unlink("/vacio"); // Elimina el semáforo del sistema
    sem_unlink("/lleno"); // Elimina el semáforo del sistema
    shm_unlink("/memoria_compartida"); // Elimina el bloque de memoria compartida del sistema
}

