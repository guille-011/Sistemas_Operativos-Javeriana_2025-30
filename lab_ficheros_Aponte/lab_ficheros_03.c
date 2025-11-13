
/**************************************************************************************
 * Fecha: 13/11/2025
 * Autor: Guillermo Aponte
 * Materia: Sistemas Operativos
 * Tema: Lectura del contenido de un fichero
 *
 * Objetivo:
 *    Implementar un programa básico que ejemplifique el proceso de lectura de un directorio, leyendo
 *  todos sus ficheros, para imprimer su tipo, sus permisos y su hora de modificación
 * Aspectos cubiertos:
 *  - Variables de tipo FILE
 *  - Funciones estándar de lectura, fopen, fgets, fclose
 * 
 *
 *************************************************************************************/




#include <stdio.h>      // Librería estándar para entrada/salida (printf, scanf, fopen, fclose, etc.).
#include <string.h>     // Librería para manejo de cadenas (strcmp, strcpy, strlen, strcat, etc.).
#include <time.h>       // Librería para manejo de tiempo y fechas (time, ctime, localtime, difftime, etc.).
#include <dirent.h>     // Librería para manipular directorios (opendir, readdir, closedir).
#include <fcntl.h>      // Librería para control de archivos a bajo nivel (open, O_CREAT, O_RDONLY, etc.).
#include <unistd.h>     // Librería POSIX para operaciones del sistema (read, write, close, fork, etc.).
#include <sys/stat.h>   // Librería para obtener información de archivos (stat, fstat) y permisos (S_ISDIR, S_ISREG).
#include <sys/types.h>  // Define tipos de datos del sistema usados por otras librerías del sistema (mode_t, pid_t, etc.).

/* Función main que muestra todos los ficheros de un directorio, señalando sus permisos, y si es un fichero simple
o también es un diretorio*/
int main()
{
    int er;                                   // Se declara una variable entera llamada er
    char nomdir[100], nomfich[100], resp[30]; // Se declaran 3 cadenas de caracteres de tamaño máximo 100, 100 y 30 respectivamente
    struct stat atr;  // Se crea una estructura stat llamada atr, que se usa para guardar los datos de un archivo
    DIR *d; //Se crea un apuntador a directorio llamado d
    struct dirent *rd1;  // Se crea un apuntador a dirent llamdo direc, se usa para leer el contenido de un directorio
    time_t fecha; //se crea una variable de tipo time_t llamda fecha, esta variable guarda la fecha y hora en el formato del sistema, almacenando los segundos desde 1970

    printf("Nombre directorio\n"); //Se le pide al usuario el nombre del directorio
    fgets(nomdir, sizeof(nomdir), stdin); //Se usa la función gets par pedir el directorio por consola, guardándolo en la cadena nomdir
    /* Se quita el \n del nombre del directorio*/
    nomdir[strlen(nomdir) - 1] = '\0';
    fecha = time(&fecha); //Se usa la función time para guardar la fecha en la variable fecha de tipo time_t esta función retorna el tiempo actual
    if ((d = opendir(nomdir)) == NULL) //Si no se puede abrrir el directorio se imprime un mensaje de error
    {
        printf("No existe ese directorio \n");
        return -1;
    }
    else
    {
        
    /*Se crea un ciclo asignando a direc el retorno de la variable readdir, enviandole como parámetro el puntero de tipo DIR, esto indica que siempre que se esté
    leyendo se debe realziar lo solicitado, es decir, siempre que sea diferente a NULL, readdir retorna los datos de los ficheros que se encuentren en el
    directorio enviado como parámetro, y cuando no hay nada que leer devuelve NULL*/
        while ((rd1 = readdir(d)) != NULL)
        {

            /*Se realiza una comparación para ignorar las dos entradas "." y "..", pues en linux siempre aparecen estas como archivos, pese a que no lo son
            en realidad, "." representa el propio directorio y ".." el directorio padre, siendo así, seria incorrecto ostrarlos en nuestro código*/
            if ((strcmp(rd1->d_name, ".") != 0) && (strcmp(rd1->d_name, "..") != 0))
            {
                /*usa la función strcpy para copiar en nomfich el nombre del fichero que se está evaluando*/
                strcpy(nomfich, nomdir);
                strcat(nomfich, "/"); //Le concatena a nomfich "/"
                strcat(nomfich, rd1->d_name); //Le concatena a nomfich el nombre del archivo que se está leyendo en ese momento, guardando su ruta
                printf("fichero :%s:", nomfich); //Imprime la carpeta evaluada y el nombre del directorio ingresado por el usuario seguiodo del directorio leido en el programa
                
                /*Se usa la función stat para guardar los datos de la ruta enviada en nomfich en la variable atr, que contiene los datos de un fichero,
                
                su retorno se guarda en la variable er, este retorno puede ser 0  -1 según sea el caso, retorna 0 cuando se lee correctamente*/
                er = stat(nomfich, &atr);

                /*Se imprime el modo del archivo, es decir el tipo de archivo y sus permisos, para esto se usa el operador "%#o", que indica
                que será un número impreso en base octal, además, el # indica que se imprimirá con un prefijo "0"*/
                printf("modo :%#o:", atr.st_mode);
                
                
                /*Se verifican permisos de propietario de la siguiente forma:
                
                atr.str_mode contiene todos los bits de permiso y tipo de archivo, el operador & compara bit a bit esta estructura con los bits en octal
                0400, que representa el bit de lectura del propietario, por lo tanto, si devuelve 0 es porque se encontró y por ende si tiene permisos de
                lectura, por lo tanto se imprime esto por consola*/
                if ((atr.st_mode & 0400) != 0)
                    printf(" permiso R para propietario\n");

                else
                    printf(" No permiso R para propietario\n"); // si no tiene permisos se imprime 
                
                /*Se llama a un marco de tipo S_ISxxx(), en este caso S_ISDIR identifica si un fichero es directorio, se le envia la variable que está
                guardando los datos del directorio*/
                if (S_ISDIR(atr.st_mode))
                    printf(" Es un directorio \n");

                
                /*Se llama a un marco de tipo S_ISxxx(), en este caso S_ISREG identifica si un fichero es regular, se le envia la variable que está
                guardando los datos del directorio*/    
                if (S_ISREG(atr.st_mode))
                    /* ficheros modificados en los últimos 10 dias 
                    de la siguiente forma:
                    
                    fecha guarda el tiempo en segundos actual desde 1970, para saber que no han pasado 10 días, se realiza una operación aritmetica
                    que resulta en el tiempo en segundos que transcurre después de 10 dias, se compara con atr.st_mtime que guarda el tiempo en segundos desde
                    que se creó el archivo o desde que se modificó*/
                    if ((fecha - 10 * 24 * 60 * 60) < atr.st_mtime)
                    {
                        /*Se imprime el nombre del fichero, la fecha de acceso y el tiempo en segundos desde que se modificó*/
                        printf("FICHERO:%s: fecha acceso %s, en sgdos %ld\n", rd1->d_name, ctime(&atr.st_mtime), atr.st_mtime);
                    }
            }
        } 
        closedir(d); //Se cierra el directorio
    }
    return 0;

} 


/*

Conclusiones:

En el anterior código se logró evidenciar algunos de los datos importantes a los que se puede acceder usando las funciones de C, en este ejemplo
mostrando los permisos del archivo y la fehca de modificación, solo de los archivos resientes (menos de 10 dias), demostrando cuán flexible puede ser el lenguaje
con todas las operaciones realizables con estas librerias., 

Estas funcionalidades de C permiten realizar programas que puedan realizar acciones según los permisos de los directorios, impidinedo posibles errores.

*/
