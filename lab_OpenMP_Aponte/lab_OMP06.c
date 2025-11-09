/**************************************************************************************
 * Fecha: 11/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Control del número de hilos y medición de rendimiento con OpenMP y gettimeofday()
 *
 * Objetivo:
 *    Implementar un programa que realice una sumatoria matemática de manera paralela
 *    utilizando OpenMP, controlando explícitamente la cantidad de hilos empleados 
 *    mediante la función omp_set_num_threads(). Además, medir el tiempo total de 
 *    ejecución del programa con alta precisión utilizando la función POSIX 
 *    gettimeofday(), permitiendo así comparar el rendimiento entre diferentes 
 *    configuraciones de paralelismo.
 *
 * Descripción:
 *    El programa calcula la sumatoria de los senos de múltiples rangos de números,
 *    definidos por el índice de iteración. Para cada valor de i, la función fsum()
 *    realiza una sumatoria interna desde un valor inicial hasta un valor final
 *    calculado a partir de i. 
 *
 *    La ejecución del ciclo principal se paraleliza con la directiva:
 *        #pragma omp parallel for reduction(+: resSumatoria)
 *    Esto divide las iteraciones del ciclo entre varios hilos, creando copias privadas
 *    de la variable resSumatoria para cada hilo, y combinando sus resultados al final,
 *    evitando así condiciones de carrera (race conditions).
 *
 *    El usuario puede especificar por línea de comandos:
 *        - El número de hilos que desea usar (numHilos)
 *        - La cantidad de repeticiones del ciclo (rep)
 *
 *    La medición del tiempo total de ejecución se realiza mediante las funciones:
 *        - InicioMuestra(): guarda el instante inicial usando gettimeofday()
 *        - FinMuestra(): calcula e imprime el tiempo total transcurrido en microsegundos.
 *
 * Aspectos cubiertos:
 *  - Inclusión y uso de la librería omp.h para paralelismo con OpenMP
 *  - Control manual del número de hilos con omp_set_num_threads()
 *  - Uso de la cláusula reduction para combinar resultados parciales
 *  - Prevención de condiciones de carrera
 *  - Medición precisa del tiempo con funciones POSIX (gettimeofday)
 *  - Conversión de unidades de tiempo (segundos y microsegundos)
 *  - Uso de funciones matemáticas de la librería math.h
 *
 **************************************************************************************/


#include <omp.h>     // Proporciona las funciones y directivas para paralelismo con OpenMP
#include <stdio.h>   // Permite entrada y salida estándar (printf, scanf, etc.)
#include <stdlib.h>  // Contiene funciones generales del sistema (malloc, atoi, exit, etc.)
#include <math.h>    //Contiene funciones matemáticas
#include <sys/time.h> //Permite tener funciones para manipular el tiempo del sistema

/*Se utiliza un macro, es decir, un preprocesador de C, que antes de compilar, realiza un reemplazo del nombre
del macro por el operador ternario que hay en su interior. De la siguiente manera:

MIN: Es el nombre que recibe el macro, x y y son los parámetros que se le deben enviar para que realice
el reemplazo correctamente

(x<y)?x:y es un operador ternario, que compara a x con y, retorna el valor mínimo de los dos enviados como parámetro*/
#define MIN(x, y) ((x < y)?x:y)


/*Se declaran dos variables de tipo struct timeval, estructura definida en sys/time.h. 
Estas variables se usan para guardar una marca de tiempo precisa, con resolución de segundos
y microsegundos.

Se tiene una variable inicio, para cuando comienza la muestra y una variable fin para cuando
termina*/
struct timeval inicio, fin;

/*Funcion inicioMuestra que guarda en la variable inicio, la hora en que se llamó a la función*/
void InicioMuestra(){ 	

    /*Se llama a la función gettimeofday, que recibe como parámetro una variable de tipo struct timeval,
	en este caso se le envia la dirección de memoria de la variable inicio, declarada anteriormente,
	como segundo parámetro se le envia información de zona horaria, sin embargo, al no ser importante
	en este caso, se envia (void*)0, para indicar que no se envia nada en este caso*/
	gettimeofday(&inicio, (void *)0);
}

/*Funcion finMuestra que calcula el tiempo total de ejecución en microsegundos de un programa*/
void FinMuestra(){ 
    /*Se llama a la función gettimeofday, que recibe como parámetro una variable de tipo struct timeval,
	en este caso se le envia la dirección de memoria de la variable fin, declarada anteriormente,
	como segundo parámetro se le envia información de zona horaria, sin embargo, al no ser importante
	en este caso, se envia (void*)0, para indicar que no se envia nada en este caso*/
	gettimeofday(&fin, (void *)0); 

    /*Se calcula el tiempo total transcurrido desde que inició la muestra hasta que finalizó,
	para esto se resta la hora a la que se llamó la función InicioMuestra, contenido en la variable inicio
	de la hora a la que se llamó a la función FinMuestra, contenida en la variable fin*/
	fin.tv_usec -= inicio.tv_usec; 
	fin.tv_sec  -= inicio.tv_sec; 

    /*Se convierten los segundos y microsegundos a segundos totales, multiplicando los segundos por 
	1000000 para pasarlos a microsegundos y sumándole los microsegundos anteriormente obtenidos
	luego, este valor se castea a double y se guarda en la variable tiempo*/

	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); 
	printf("%9.0f \n", tiempo); //Se imprime la variable tiempo como números flotantes de caracteres y con ancho de 9 caracteres
}

/*Función sumatoria que realiza las sumatoria de todos los senos entre inicio = i*(i+1)/2 y fin = inicio + i
la guarda en una variable valorSum y la retorna*/
double fsum(int i){
    int inicio = i*(i+1)/2; //Se guarda en inicio el valor de i*(i+1)/2
    int fin = inicio+i; //Se guarda en fin el valor de inicio + i
    double valorSum = 0.0; //Se declara la variable valorSum y se iguala a 0
    /*Cilo for que itera j desde inicio hasta fin*/
    for(int j = inicio; j < fin; j++){
        /*Se guarda en valorSum la sumatoria del valor retornado por la funcion sin de la libreria math.h, la función
        recibe un número entero, el cuál interpreta como ángulo en radianes, y luego, utilizando una serie polinómica,
        calcula el valor aproximado del seno de este ángulo y retorna un valor de double con este resultado*/
        valorSum += sin(j); 
    }

    return valorSum; //Se retorna valorSum
}

/*Función principal que realiza el llamado a las demás funciones*/
int main(int argc, char *argv[]) {

    /*Se imprime un mensaje de error si la función tiene menos de 3 argumentos*/
    if(argc < 3){
        printf("Error \n\t $Ejecutable numHilos\n");
        exit(0);
    }
    double resSumatoria = 0; //Se declara una variable resSumatoria y se le da el valor de 0
   
    int numHilos = (int) atoi(argv[1]); //Se declara una variable numHilos y se le da el valor del primer argumento enviado por el usuario
    int rep = (int) atoi(argv[2]); //Se declara una variable rep y se le da el valor del segundo argumetno ingresado por el usuario
  
    /*Función de OpenMp utilizada para fijar la cantidad de hilos que será usada en el siguiente bloque de código paralelizado
    recibe un entero que representa este número.
    
    El siguiente bloque de código que implemente un pragma para usar paralelismo usará máximo la cantidad de hilos ingresada en esta
    función
    
    En caso de que se coloque un número mayor al que tiene disponible el sistema operativo, se gestiona con "times slicing", alternando
    los hilos en los núcleos disponibles en el sistema operativo, generando cambios de contexto que reducen el rendimiento real*/
    omp_set_num_threads(numHilos);

    printf("Número de hilos fijados: %d \n", numHilos);//Se imprime por pantalla la cantidad de hilos fijada por omp_get_max_threads
    InicioMuestra(); //Se llama a la función inicioMuestra para guardar cuando empezó la ejecución del programa
     /*Se llama a una directiva del compilador mediante #pragma
    Una directiva le dice al compilador que debe realizar algo diferente durante la compilación del código, de tal forma,
    no se cambia el código en si pero si la manera en como se ejecutará posteriormente a compilarlo.
    
    Luego de la directiva, se llama a la otra parte, compuesta por las siguientes directivas:

    
    omp: indica que debe seguir una instrucción de OpenMP
    parallel for: indica que el siguiente ciclo for que se ejecute en el programa se debe realizar de manera paralela, donde
    cada hilo debe realizar una parte de las iteraciones del ciclo de forma que el trabajo se reparte automáticamente.
    reduction(+: resSumatoria): Clausula de OpenMp utilizada par combinar los resultados parciales de varios hilos en una 
    sola variable compartida, sin causar condiciones de carrera. De la siguiente forma: 
    Se crea una copia privada de resSumatoria para cada hilo.
    Cuando termina de ejecutarse, OpenMp combina todas las copias locales y las guarda en resSumatoria*/
    #pragma omp parallel for reduction(+: resSumatoria)
    for(int i = 0; i < rep; i++){
        /*Se guarda en resSumatoria la suma de todos los resultados retornados por la funcion fsum, enviándole como parámetro el número iterado*/
        resSumatoria += fsum(i);
    
    }
    printf("Tiempo de muestra \n");
    FinMuestra(); //Se llama a la función finMuestra para guardar cuando acabó la ejecución del programa
    printf("Resultado Sumatoria función SENO: %0.2f \n", resSumatoria); //Se imprime el reultado de la sumatoria del llamado a la funcións seno
    return 0;
}
