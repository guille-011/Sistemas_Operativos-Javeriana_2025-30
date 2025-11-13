/*#######################################################################################
#* Fecha:
#* Autores: Guillermo Aponte, Daniel Ramírez, David Tobar, Juan Daza, Profesor J. Corredor, PhD
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Hilos Pthreads "Posix" 
######################################################################################*/

#include "moduloPosix.h" // Se incluye el modulo.h para implementar las funciones definidas allí

//Se declaran las variables globales que se prometieron con "extern" en el modulo.h
//Nota: Ya que en el modulo.h se usa "extern" y que el archivo mmClasicaPosix.c incluye ese modulo,
//no es necesario declarar estas variables globales allí, basta con declararlas acá
double *matrixA, *matrixB, *matrixC;
struct timeval inicio, fin;

//Función que permite tomar el valor de la hora exacta inicial
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0); //Guarda en la variable global "inicio" el momento en el que empieza la medición
}

//Función que permite tomar el valor de la hora exacta final
void FinMuestra(){
	gettimeofday(&fin, (void *)0); //Guarda en la variable global "fin" el momento en el que termina la medición

	//Hace la medición restando los microsegundos entre el tiempo final y el inicial
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;

	//guarda en la variable "tiempo" la cantidad de microsegundos de la medición
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); 
	printf("%9.0f \n", tiempo); //imprime el tiempo que tomó hacer la multiplicación sin precisión de decimales
}

//Función que inicializa dos matrices de tamaño D
void iniMatrix(double *m1, double *m2, int D){
	for(int i = 0; i < D*D; i++, m1++, m2++){
			//a ambas matrices enviadas como parámetro, se le asigna un valor "aleatorio"
			//este valor está dado por una seed y en este caso, al no establecer alguna, se escoge por defecto
			//y en cada ejecución, se tendrán los mismos valores.
			//Esto no afecta la funcionalidad de nuestra evaluación de rendimiento, pues los bytes que representen un numero
			//tienen un efecto neutro en la complejidad del algoritmo. 
			*m1 = 1 + (double)rand()/RAND_MAX*(5.0-1.0);  //en este caso se ecogen numeros del 1 al 5
			*m2 = 5 + (double)rand()/RAND_MAX*(9.0-5.0); //en este caso se ecogen numeros del 5 al 9
		}	
}

//Funcion que imprime una matriz
void impMatrix(double *matriz, int D){
	if(D < 9){ //En caso de que el tamaño de la matriz sea menor a 9, se imprimirá
    		for(int i = 0; i < D*D; i++){//Recorre la matriz (que en realidad es memoria contigua) columnas por filas
     				if(i%D==0) printf("\n");// si se llega a un multiplo de el tamaño de la matriz se hace un salto de linea
            		printf(" %.2f ", matriz[i]); //se imprime la matriz con presicion para 2 decimales
			}	
    	printf("\n>-------------------->\n");
	}
}


//Función que multiplica las matrices 
void *multiMatrix(void *variables){ //recibe una estructura parametros que le ayuda a los hilos a saber 
									//Que parte de la matriz deben calcular
	//se hace el casteo explicito ya que la funcion que ejecutan los hilos
	//tiene como requisito un parametro del tipo apuntador a void
	struct parametros *data = (struct parametros *)variables; 
	
	int idH		= data->idH; //se guarda el id del hilo
	int nH		= data->nH; // se guarda la cantidad de hilos
	int D		= data->N; // se guarda el tamaño de la matriz

	//Esta parte reparte el trabajo entre hilos decidiendo desde que fila hasta que fila se van a encargar
	int filaI = (D * idH) / nH;  //Primero se realiza la multiplicacion y luego la division para no perder decimales ya que filaI es un entero
	int filaF = (D * (idH + 1)) / nH; //Se hace lo mismo aca pero la fila final se establece como la fila inicial del siguiente hilo (en cuanto al ID)
	double Suma, *pA, *pB; //Se declaran las variables usadas para realizar las operaciones ncesarias

    for (int i = filaI; i < filaF; i++){ //Desde la fila inicial hasta la fila final de un hilo dado
        for (int j = 0; j < D; j++){     //se recorre por columnas la segunda matriz
			pA = matrixA + i*D; 		 //pA salta de fila en fila
			pB = matrixB + j;			 //pB salta de columna en columna
			Suma = 0.0;					 //se reinicia la suma
            for (int k = 0; k < D; k++, pA++, pB+=D){//se multiplican filas por columnas avanzando hacia la derecha en columnas y hacia abajo en filas
				Suma += *pA * *pB; //se guarda la suma
			}
			matrixC[i*D+j] = Suma; //la suma se asigna al valor que le corresponde en la matriz resultante
		}
	}

	free(variables); //se libera la memoria de el apuntador "variables"
	pthread_exit(NULL); //el hilo acaba su ejecución y devuelve NULL
}