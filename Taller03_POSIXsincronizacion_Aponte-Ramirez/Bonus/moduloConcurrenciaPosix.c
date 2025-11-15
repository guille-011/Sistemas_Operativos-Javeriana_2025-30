/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: paralelismo con POSIX
 * Archivo: moduloConcurrenciaPosix.c
 * Descripcion: Implementa las funciones que permiten calcular el
 *              valor máximo de un vector de enteros de forma
 *              paralela utilizando hilos POSIX. Para ello divide
 *              el arreglo en segmentos, asigna cada segmento a un
 *              hilo que calcula su máximo parcial y, tras esperar
 *              la finalización de todos los hilos, obtiene el
 *              máximo global a partir de dichos resultados
 *              parciales.
 * 
 * Fecha: 14/11/25
 ******************************************************************/



#include "moduloConcurrenciaPosix.h"


/*Función que guarda el valor máximo encontrado en el vector de la estructura argumentos de tipo param_H*/
void *buscarMax(void *parametro){

        /*
        Se castea el valor enviado como parámetro a param_H y se guarda en un apuntador a param_H llamdado
        argumentos
        */
        param_H *argumentos = (param_H *)parametro;
        argumentos->maxparcial = argumentos->vector[argumentos->inicio]; //Se guarda en argumentos->maxparcial el primer valor del segmento del vector


        /*Ciclo desde 0 hasta argumentos-> fin*/
        for(int i=argumentos->inicio; i<argumentos->fin; i++){

                /*Según como vaya encontrando valores en el vector mas grandes que maxparcial, va remplazandoe esta variable con lo que encuentra,
                de tal forma que al final de la ejecución el valor que queda es el mas grande de todos*/
                if(argumentos->vector[i] > argumentos->maxparcial)
                        argumentos->maxparcial = argumentos->vector[i];
        }
        /*Se termina únicamente el hilo que se encuentra en ejecución, adicionalemente, se le envía como parámetro un 0, el cuál es equivalente a NULL en 
        este caso, pues se le indica a la función que el hilo determinado no debe retornar nada cuando se le haga join mas adelante en el código*/
        pthread_exit(0);
        return NULL;
}


int maximoValor(int *vec, int n, int nhilos) {
        /*Se realizan dos validaciones de tal forma que el código no se rompa en caso de que se envíen valores incorrectos
        o inesperados al llamar la función con n y nhilos.
        
        Si el usuario ingresa un número menor a 1 cuando le indica al programa la cantidad de hilos a usar, se corre el riesgo
        de que hayan errores al momento de reservar memoria para un arreglo de esta cantidad, generando errrores graves de
        compilación y pudiendo afectar todo el resto de lógica del código, de tal forma, en caso de que el número sea así,
        se convierte automáticamente en 1, pues es un valor neutro que permite solucionar el error
        
        
        Si el usuario ingresa mas hilos que la cantidad de elementos que tiene el archivo, automáticamente se transforma la cantidad
        de estos hilos en el total de elementos, esto para garantizar la eficiencia al momento de realizar paralelismo, pues si 
        simplemente se crearan todos los hilos ingresados, habrían algunos que no estarían realizando su función, al no poder
        ser trabajados por el procesador, aumentando el overheap y reduciendo el rendimiento*/
        if (nhilos <= 0)
                nhilos = 1;
        if (nhilos > n)
                nhilos = n;

         /*Se crean una variable de tipo pthread_t, un apuntador a pthread_t llamado hilos,
        que es el tipo de dato que usa pthread para identificar un hilo, almacenando en el los ID
        de todos lo hilos que son creados posteriormente por pthread_create.
        
        
        Se le reserva la cantiad de memoria equivalente a un total de nhilos de tipo pthread_t usando
        la función malloc
        */
        pthread_t *hilos = malloc(sizeof(pthread_t) * nhilos);

        /* Se crea un apuntaodr de tipo param_H llamado argumentos y se le reserva la cantiad de memoria equivalente
        a nhilos elementos de tamaño param_H usando la función malloc*/
        param_H  *argumentos = malloc(sizeof(param_H) * nhilos);


        /*Se verifica que se haya creado correctamente la memoria*/
        if (hilos == NULL || argumentos == NULL) {
                fprintf(stderr, "Error reservando memoria para hilos/argumentos\n");
                exit(-1);
        }

        /*Se declaran las variables que se usarán posteriormente para que cada hilo sepa las cantidad de elementos
        que debe manejar, de la siguiente forma:
        
        
        tamBase divide el total de elementos entre el total de hilos a usarse, así se puede saber cuantos elementos procesará cada hilo
        resto guarda el módulo de la división anterior, así, si la división no es exacta, se sabe cuantos elementos se deben guardar para 
        tener en cuentra posteriormente
        inicio guarda en donde empieza cada hilo, se va operando conforme avanza el ciclo*/
        int tamBase = n / nhilos;   // tamaño base de cada segmento
        int resto   = n % nhilos;   // algunos hilos tendrán 1 elemento más
        int inicio  = 0;

        /*Ciclo desde 0 hasta el total de hilos, para crear todos los hilos necesarios para realizar lo solicitado por el programa*/
        for (int h = 0; h < nhilos; h++) {

                /*Se declara una variable tamSegmento, se le asigna el tamaño de tamBase, y en caso de que h sea menor a la variable resto,
                se le suma 1 al tamaño, resultando en el hilo con mas elementos, si h es mayor a resto, no se le suma nada*/
                int tamSegmento = tamBase + (h < resto ? 1 : 0); 

                argumentos[h].inicio = inicio; //Se le asigna a la variable inicio de la estructura argumentos en la posición h el valor de inicio
                argumentos[h].fin    = inicio + tamSegmento; //Se le asigna a la variable fin de la estructura argumentos en la posición h el valor de inicio + tamSegmento
                argumentos[h].vector = vec; //Se le asigna a la variable vector de la estructura argumentos en la posición h el valor del vector enviado como parámetro



                  /*Creacion de dos hilos usando la función pthread_create. Esta función guarda el ID de un hilo en la dirección de memoria de tipo
                pthread_t, y hace que una función sea ejecutada por esa varaible. Los parámetros que espera la función son los siguientes:
        
                -Dirección de memoria de tipo pthread_t, que guarda el ID del hilo creado en ese momento
                -Valores por defecto que se le van a dar al hilo, como por ejemplo el tamaño de su stack, indicarle que no debe esperar join, etc.
                en este caso se le envia NULL, pues los hilos utilizados en el programa no tienen ningún valor pre-definido.
                -La función que será ejecutada por los hilos, esta función debe retornar un apuntador de memoria y solo debe recibir un parámetro
                que debe ser de tipo genérico (void *arg), pues esto es lo exigido por pthread_create, ya que si la función recibiera mas de un parámetro
                y además no fuera genérico, el programa no podría saber con certeza que debe retornar, ni pthread_join sabria con certeza que debe recibir
                en caso de que sea usado.
                -El valor que se le enviará a la función declarada anteriormente, este valor puede ser de cualquier tipo, pues la función recibe un 
                parámetro genérico, en este caso se le envía NULL, pues las funciones no necesitan ningún argumento.
        
                La fución pthread_create retorna 0 si el hilo fue creado correctamente, sino devuelve un código de error POSIX
        
                Se guarda su retorno en la variable err, para verificar si hubo algún error en la creación del hilo, en caso de que err
                sea diferente de 0, se imprime un mensaje de error por pantalla usando la función perror
                */
                int err = pthread_create(&hilos[h], NULL, buscarMax, (void*)&argumentos[h]);
                if (err != 0) {
                        perror("Error creando hilo");
                        free(hilos);
                        free(argumentos);
                        exit(-1);
                }

                inicio += tamSegmento; //Se actualiza inico para el siguiente hilo sumando tamSegmento
        }

        /*Se utiliza la función join iterando en todos los hilos creados anteriormente usando pthread_create.
         La función de join, es esperar que todos terminen y poder liberar los recursos utilizados por estos hilos,

        Cuando el hilo enviado como parámetro a pthread_join retorna que ya terminó de realizar la función que estaba
        ejecutanto, pthread_join realiza otro retorno que indica al sistema que los resursos utilizados por este hilo ya
        fueron liberados.

        El segundo parámetro de pthread_join corresponde a una variable genérica donde se guardará
        lo que se espera recibir del hilo, en este caso, como el hilo no retorna nada, simplemente se le
        envía NULL en este parámetro, para que la función no guarde.
        */
        for (int h = 0; h < nhilos; h++) {
                pthread_join(hilos[h], NULL);
        }

        /*Como cada posición de argumentos guarda el máximo encontrado en una zona determinada del vector, ahora, hace falta encontrar
        el máximo total de TODO el vector, para esto se declara una variable maximo y se le asigna el tamaño de la primera posición,
        posteriormente se realiza un ciclo que recorra el total de hilos, y cada vez que encuentre un valor mayor a maximo, actualiza esta 
        variable, cuando acabe el ciclo quedará el máximo de todo el vector*/
        int maximo = argumentos[0].maxparcial;
        for (int h = 1; h < nhilos; h++) {
                if (argumentos[h].maxparcial > maximo)
                        maximo = argumentos[h].maxparcial;
        }

        /*Libera memoria*/
        free(hilos);
        free(argumentos);

        return maximo; //Se retorna el máximo encontrado
}