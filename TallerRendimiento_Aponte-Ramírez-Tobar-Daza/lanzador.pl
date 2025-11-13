#!/usr/bin/perl
#**************************************************************
#         		Pontificia Universidad Javeriana
#     Autores: Guillermo Aponte - Juan Daza - Daniel Ramírez - David Tobar, Profesor J. Corredor, PhD
#     Fecha: 
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/

$Path = `pwd`; # guarda el path del path actual
chomp($Path); # elimina el salto de línea al final de la variable 

# nombre de los ejecutables a correr
$NombreClasicoPosix = "mmClasicaPosix"; 
$NombreClasicoFork = "mmClasicaFork";
$NombreClasicoOpenMP = "mmClasicaOpenMP";
$NombreFilasOpenMP = "mmFilasOpenMP";

@Size_Matriz = ("4800"); # tamaños de matriz a evaluar
@Num_Hilos = ("1", "2", "4", "8", "12", "32"); # numero de hilos a evaluar
$Repeticiones = 30;# numero de repeticiones para cada configuracion de tamaño de matriz y cantida de hilos


# triple for para ejecutar el programa con todas las combinaciones de tamaño de matriz, cantidad de hilos y cantidad de repeticiones
foreach $size (@Size_Matriz){ 
	foreach $hilo (@Num_Hilos) {
		#crea la variable que guarda el nombre del archivo de salida .dat
		$fileClasicoPosix = "$Path/$NombreClasicoPosix-".$size."-Hilos-".$hilo.".dat"; 
		$fileClasicoFork = "$Path/$NombreClasicoFork-".$size."-Hilos-".$hilo.".dat"; 
		$fileClasicoOpenMP = "$Path/$NombreClasicoOpenMP-".$size."-Hilos-".$hilo.".dat"; 
		$fileFilasOpenMP = "$Path/$NombreFilasOpenMP-".$size."-Hilos-".$hilo.".dat"; 

		# elimina los archivos si ya existen
		printf("rm $fileClasicoPosix\n"); 
		system("rm $fileClasicoPosix"); 

		printf("\nrm $fileClasicoFork\n"); 
		system("rm $fileClasicoFork");

		printf("\nrm $fileClasicoOpenMP\n"); 
		system("rm $fileClasicoOpenMP");

		printf("\nrm $fileFilasOpenMP\n"); 
		system("rm $fileFilasOpenMP");

		for ($i=0; $i<$Repeticiones; $i++) {
			# ejecuta el programa y guarda la salida en el archivo.dat e imprime en consola el comando que se acaba de ejecutar
			printf("$Path/Ejecutables/$NombreClasicoPosix $size $hilo \n"); 
			system("$Path/Ejecutables/$NombreClasicoPosix $size $hilo  >> $fileClasicoPosix"); 
			
			printf("$Path/Ejecutables/$NombreClasicoFork $size $hilo \n");
			system("$Path/Ejecutables/$NombreClasicoFork $size $hilo  >> $fileClasicoFork");
			
			printf("$Path/Ejecutables/$NombreClasicoOpenMP $size $hilo \n");
			system("$Path/Ejecutables/$NombreClasicoOpenMP $size $hilo  >> $fileClasicoOpenMP");
			
			printf("$Path/Ejecutables/$NombreFilasOpenMP $size $hilo \n");
			system("$Path/Ejecutables/$NombreFilasOpenMP $size $hilo  >> $fileFilasOpenMP");
		}
		system("$Path/Ejecutables/calculadoraPromedios $fileClasicoPosix  >> $fileClasicoPosix");
		system("$Path/Ejecutables/calculadoraPromedios $fileClasicoFork  >> $fileClasicoFork");
		system("$Path/Ejecutables/calculadoraPromedios $fileClasicoOpenMP  >> $fileClasicoOpenMP");
		system("$Path/Ejecutables/calculadoraPromedios $fileFilasOpenMP  >> $fileFilasOpenMP");

		#cierra los archivos de esas configuraciones
		close($fileClasicoPosix);
		close($fileClasicoFork);
		close($fileClasicoOpenMP);
		close($fileFilasOpenMP);
	}
}
