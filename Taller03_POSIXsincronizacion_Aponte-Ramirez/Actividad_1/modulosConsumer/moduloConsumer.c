/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: IPC con named Semaphores y Sahred Memory
 * Archivo: moduloConsumer.c 
 * Descripcion:
 * Fecha: 14/11/25
 ******************************************************************/
#include "moduloConsumer.h"

void abrirSemaforos(sem_t** vacio, sem_t** lleno){
    //Declaración de punteros a named semaphores
    //El primer parametro es el nombre del semaforo y el segundo parámetro (0) 
    //es solo para abrirlo, no crearlo
    *vacio = sem_open("/vacio", 0); //abre un semaforo existente con nombre "/vacio"
    *lleno = sem_open("/lleno", 0); //abre un semaforo existente con nombre "/lleno"
    
    //Si cualquiera de los dos semaforos devuelve SEM_FAILED, se cumple esta condicion
    if (*vacio == SEM_FAILED || *lleno == SEM_FAILED) {
        perror("sem_open"); // imprime el mensaje de error basado en el error que haya ocurrido
        exit(EXIT_FAILURE); // termina el programa con un error
    }
}

void abrirMemoriaCompartida(int* fd_compartido){
    //Apertura de memoria compartida que ya existe ya que debe haber sido creada en otra parte
    //Esa memoria (buffer) tiene el nombre de "/memoria_compartida", se abre con permisos de lectura
    //y escritura dados por RDWR = Read & Write y se otorgan permisos 0644 (lectura y escritura para el usuario propietario):
    // En este ejemplo, el número 0640 indica lo siguiente:
    // 1, 2 y 4 significan permiso solo para ejecutar, escritura y lectura respectivamente, mientras
    //que 0 significa sin permisos
    // Si se suman los permisos se obtiene un número entre 0 y 7
    // Para este ejemplo:
    // 0: prefijo que indica que es un número en base 8
    // 6: (segundo dígito) permisos del usuario propietario 4+2 = 6 (lectura y escritura)
    // 4: (tercer dígito) permisos del grupo al que pertenece el usuario propietario 4 = solo lectura
    // 0: (cuarto dígito) permisos para otros usuarios 0 = sin permisos
    // También se maneja el error en caso de que no abra el semaforo
    *fd_compartido = shm_open("/memoria_compartida", O_RDWR, 0644);
    if (*fd_compartido < 0) { //Si devuelve -1 hubo error
        perror("shm_open"); //Imprime el respectivo error
        exit(EXIT_FAILURE); //termina la ejecucion del programa
    }
}

void mapearYConsumir(compartir_datos** compartir, sem_t* vacio, sem_t* lleno, int fd_compartido){
    //Mapeamos el espacio de memoria compartido entre procesos
    //Una vez el otro proceso ya ha creado el espacio de memoria compartido, este proceso
    //solo lo ubica y lo mapea:
    //Parámetros:
    // #1. Donde queremos crear el espacio de memoria (no se usa aqui, NULL es cualquier espacio)
    // #2. El tamaño exacto del bloque de memoria que queremos mapear
    // #3. Permisos de mapeo, es decir, que puede hacer este proceso con esa memoria compartida
    //     En este caso es leer (PROT_READ) y escribir (PROT_WRITE), el caracter | es un operador lógico OR
    //     Se usa para indicar los permisos torales ya que este parámetro espera ciertos bits que representan los permisos
    // #4. Tipo de mapeo, en este caso es MAP_SHARED para que los demás procesos puedan ver esta memoria, otra
    //     opcion es MAP_PRIVATE pero no sirve para IPC ya que los demas procesos no pueden ver los cambios
    // #5. Descriptor que se usó para abrir el bloque de memoria compartido (shm_open)
    // #6. Desde donde se quiere mapear, en este caso 0 porque queremos mapear todo
    //
    // La función mmap devuelve un puntero hacia el inicio de ese bloque de memoria por lo cual a partir
    // de este punto podemos manipular ese bloque de memoria como una estructura
    *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, fd_compartido, 0);

    //Indicamos desde que posicion de la memoria compartida vamos a recibir datos
    (*compartir)->salida = 0;

    //Ciclo for de 10 iteraciones (10 items que tiene el buffer)
    for (int i = 1; i <= 10; i++) {
        //Revisa el named semaphore
        //el semaforo "lleno" cuenta cuántas posiciones del buffer tienen un item disponible
        //Si lleno es mayor a 0, decrementa el valor de lleno y continúa
        //Si lleno es igual a 0, se bloquea el proceso hasta que algun otro proceso haga sem_post(lleno) 
        //para incrementar el valor de lleno y que este proceso pueda leer
        sem_wait(lleno);  

        //Se declara e inicializa una variable "item" para guardar lo que sea que
        //Haya en el bus desde la posicion 0 (previamente se había asignado a compartir->salida un 0)
        int item = (*compartir)->bus[(*compartir)->salida];

        //Imprime por pantalla lo que haya consumido de la memoria compartida
        printf("Consumidor: Consume %d\n", item);

        //Ahora la posicion desde donde vamos a leer de la memoria compartida aumenta en 1
        //y en caso de superar el tamaño total del buffer, se utiliza el operador % para vovler al
        //comienzo del mismo
        (*compartir)->salida = ((*compartir)->salida+1) % BUFFER;

        //Despues de haber consumido, manda la señal para aumentar vacio en 1 y poder anunciar que hay
        //una posicion de memoria más libre
        sem_post(vacio);

        //Pausa este proceso 2 segundos
        sleep(2);  
    }
}

void liberarRecursos(int fd_compartido, sem_t* vacio, sem_t* lleno, compartir_datos* compartir){
    //munmap desmapea la memoria compartida, el primer parámetro es el apuntador a esa memoria
    //y el segundo es el tamaño a desmapear que coincide con el tamaño de la estructura    
    munmap(compartir, sizeof(compartir_datos));

    //Cierra el descriptor que se usó para guardar la memoria compartida
    close(fd_compartido);

    //Cierra ambos semáforos en este proceso
    sem_close(lleno);
    sem_close(vacio);

    //Elimina del sistema los semaforos (solo sirve si todos los procesos que la usan lo ejecutan)
    //Solo hasta que todos lo hagan, el sistema operativo lo borra realmente
    sem_unlink("/lleno");
    sem_unlink("/vacio");

    //Elimina del sistema la memoria compartida con la misma lógica con la que se borran los semáforos
    shm_unlink("/memoria_compartida");
}