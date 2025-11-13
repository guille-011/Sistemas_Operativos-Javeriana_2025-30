/*#######################################################################################
#* Fecha:
#* Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Hilos Pthreads "Posix" 
######################################################################################*/

//incluye el modulo donde se definieron las estructuras y las funciones
//dado que el modulo.h define las variables globales como "extern" 
//y que ya son incluídas en el modulo.c, no se requiere volver a definirlas acá
#include "moduloPosix.h" 


//Función principal
int main(int argc, char *argv[]){
	 //Es obligatorio enviar mas de 2 parámetros, de los cuales el segundo y el tercero son el tamaño de la matriz y el numero de hilos que se desea crear
	if (argc < 3){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		exit(0);//En caso contrario acaba la ejecución del programa
	}

	//se convierte en entero el argumento del tamaño de las matrices
    int N = atoi(argv[1]); 

	//Se convierte a entero el argumento del numero de hilos que se desea utilizar
    int n_threads = atoi(argv[2]); 

	//Se crea el arreglo de identificadores de hilos
    pthread_t p[n_threads];

	//reserva memoria de tamaño N*N para las 3 matrices
	matrixA  = (double *)calloc(N*N, sizeof(double));
	matrixB  = (double *)calloc(N*N, sizeof(double));
	matrixC  = (double *)calloc(N*N, sizeof(double)); //Aca se guarda el resultado de la multiplicación

	//Se inicializan las dos matrices a multiplicar
	iniMatrix(matrixA, matrixB, N);

	//Se imprimen ambas matrices si su tamaño no es superior a una matriz de 9x9
	impMatrix(matrixA, N);
	impMatrix(matrixB, N);

	//Se toma el tiempo inicial
	InicioMuestra();

	//Para la cantidad total de hilos, se ejecutan esa cantidad de iteraciones en el ciclo for
    for (int j=0; j<n_threads; j++){
		//Se reserva memoria para crear una copia diferente de la estructura "parametros" para cada hilo
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j; //Se guarda el id del hilo
		datos->nH  = n_threads; //Se guarda el tamaño total de hilos
		datos->N   = N; //Se guarda el tamaño de las matrices

		//Para cada iteración, se crea un hilo y se guarda su ID en el arreglo de hilos, con atributos por defecto, para ejecutar la funcion de multiplicacion de matrices
		//y de parámetro a la función se envía su respectiva copia de la estructura "parámetros"
        pthread_create(&p[j],NULL,multiMatrix,(void *)datos); 
	}

	//Se hace join para cada hilo creado y lo que devuelve no se guarda en ningun lado
    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL); //Se liberan recursos de los hilos y se hace esperar al programa principal a que todos terminen

	FinMuestra(); //Toma el tiempo de finalización del algoritmo
	
	impMatrix(matrixC, N); //Imprime el resultado si su tamaño es menor a 9x9 

	//Liberación de Memoria de las 3 matrices
	free(matrixA);
	free(matrixB);
	free(matrixC);

	//Se devuelve 0 para indicar la correcta ejecución del programa
	return 0;
}

