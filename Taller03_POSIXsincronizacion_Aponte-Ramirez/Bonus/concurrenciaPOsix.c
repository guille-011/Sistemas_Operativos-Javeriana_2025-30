/*******************************************************************
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Carrera: Ingeniería de Sistemas
 * Autores: Guillermo Aponte, Daniel Ramírez
 * Materia: Sistemas Operativos
 * Tema: paralelismo con POSIX
 * Archivo: concurrenciaPOsix.c
 * Descripcion: Programa principal que lee desde un archivo de texto
 *              la cantidad de elementos y los valores de un vector
 *              de enteros, reserva dinámicamente la memoria para
 *              almacenarlos, recibe por línea de comandos el número
 *              de hilos a utilizar y delega en la función 
 *              maximoValor el cálculo paralelo del máximo, mostrando
 *              finalmente el resultado por pantalla y liberando los
 *              recursos utilizados.
 * Fecha: 14/11/25
 ******************************************************************/


#include "moduloConcurrenciaPosix.h"

/*Función principal que realiza el llamado a las funciones anteriores y realiza el proceso de lectura del archivo*/
int main(int argc, char *argv[]) {
        FILE *fichero; //Se declara un puntero de tipo FILE, variable necesaria para el manejo de archivos en C
        int n, nhilos, i; //Se declaran tres variables enteras, n, nhilos e i
        int *vec; //Se declara un apuntador a entero llamado vec
        int ret, maximo; //Se declaran dos variables enteras, ret y maximo

        /*Se verifica que el número de argumentos sea correcto, en caso de que no lo sea, se imprime un mensaje de error*/
        if (argc!=3) {
                fprintf(stderr, "Error en número de argumentos \n");
                exit(-1);
        }

        /*Se abre el archivo usando la función fopen, esta función recibe como parámetros el nombre del archivo y
        el trato que se le va a dar, ya sea de lectura o escritura.
  
        En este caso, se le envia el mismo nombre recibido como parámetro en la función leerArchivo y "r", para
        indicar que se quiere leer el archivo en modo de lectura. La función retorna un apuntador a FILE, este retorno
        se guarda en la variable archivo declarada anteriormente*/
        fichero = fopen(argv[1], "r");

         /*Si la función fopen retorna NULL, es porque hubo un error abriendo el archivo, por lo tanto si pasa esto
        se imprime que hubo un error abriendo el archivo y se sale del programa indicando que huo un error*/
        if (fichero == NULL) {
                perror("No se puede abrir fichero");
                exit(-1);
        }

        /*se utiliza la función fscanf, que es una función de la biblioteca stdio.h, encargada de leer datos formateados desde
        un archivo.
    
        Esta función espera los siguientes parámetros:
        Primero el apuntador a archivo desde donde se están leyendo los datos
        Segundo el indicador de tipo de datos que se leerán desde el archivo,
        en este caso es %d, correspondiente a enteros
        La variable donde se guardará lo leído desde el archivo, en este caso
        se le envia la dirección de n.
    
    
        La función retorna la cantidad de elementos leídos, en este caso lo 
        correcto sería leer 1, pues solo tiene un %d, de tal forma que lee
        uno por uno los elementos del archivo.
        De tal forma, si la función retorna un valor diferente a 1, se abandona el programa con 
        un mensaje de error*/
        ret = fscanf(fichero, "%d", &n);
        if (ret !=1) {
                fprintf(stderr, "No se puede leer tamaño\n");
                exit(-3);
        }
        nhilos = atoi(argv[2]); //Se convierte el segundo parámetro de entrada a entero y se guarda en nhilos
        vec = malloc(sizeof(int) * n); //Se reserva memoria para n enteros y se guarda en el apuntador vec
        
        /*Ciclo desde 0 hasta el total de hilos*/
        for (i=0;i!=n;++i) {
                /*Se lee cada elemento del archivo usando fscanf y se guarda en la posición i del vector vec*/
                ret = fscanf(fichero, "%d", &vec[i]);

                /*Si hay errores de lectura se indica cerando el programa*/
                if (ret !=1) {
                        fprintf(stderr, "No se puede leer elemento nro %d\n", i);
                        fclose(fichero);
                        free(vec);
                }
        }
        maximo = maximoValor(vec,n,nhilos); //Se calcula el valor máximo del vector usando la función maximoValor y se guarda en maximo
        printf("Máximo: %d\n", maximo); //se imprime el máximo encontrado
        fclose(fichero); //Se cierra el archivo
        free(vec); //Se libera la memoria reservada para el vector
        return 0;
}

