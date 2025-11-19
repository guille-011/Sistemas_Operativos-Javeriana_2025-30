###############################################################
# Universidad: Pontificia Universidad Javeriana
# Carrera: Ingeniería de Sistemas
# Autor: Guillermo Aponte - Daniel Ramirez - David Tobar
# Materia: Sistemas Operativos
# Fecha: 16/11/2025
# Archivo: Makefile
# Descripción:
#   Este archivo Makefile automatiza la compilación del simulador 
#   de reservas de parques.
###############################################################

#Makefile dedicado a la automatización de la compilación del simulador de reservas

#En la raíz del proyecto existe un directorio donde se guardan los módulos de definiciones
#de estructuras, variables globales, importe de librerías y prototipos de funciones

#Además, en la raíz del proyecto se encuentra otro directorio dedicado a guardar los módulos que
#contienen las implementaciones de las funciones

#En la misma raíz del proyecto se encuentran los ficheros que contienen el main de los controladores
#y el del controlador.

#Para ejecutar, asegurarse de ejecutar la regla clean y luego all.
#Una vez ejecutados esos dos comandos, los archivos de solicitud que leen los agentes
#se deben localizar en el mismo Path de donde se ejecute el comando para correr el ejecutable 

GCC = gcc
FLAGS = -lpthread #Se necesita para utilizar la biblioteca pthread
EJECUTABLES = Ejecutables#Crea un directorio de ejecutables donde se guardarán ordenadamente 
DIRECTORIOMODULOS = ModulosDeImplementacion#Directorio donde estan los modulos de implementacion

MODULOAGENTE = ModuloAgente
MODULOCONTROLADOR = ModuloControlador

PROGRAMAS = AgenteDeReservas ControladorDeReservas


all: $(PROGRAMAS)
#En caso de error creando el pipe principal, ejecutar rm -f /tmp/<nombre del pipe>

AgenteDeReservas:
	mkdir -p $(EJECUTABLES)

	$(GCC) -c $(DIRECTORIOMODULOS)/$(MODULOAGENTE).c -o $(EJECUTABLES)/$(MODULOAGENTE).o
	$(GCC) $@.c $(EJECUTABLES)/$(MODULOAGENTE).o -o $(EJECUTABLES)/$@ $(FLAGS)
	
ControladorDeReservas:
	mkdir -p $(EJECUTABLES)

	$(GCC) -c $(DIRECTORIOMODULOS)/$(MODULOCONTROLADOR).c -o $(EJECUTABLES)/$(MODULOCONTROLADOR).o
	$(GCC) $@.c $(EJECUTABLES)/$(MODULOCONTROLADOR).o -o $(EJECUTABLES)/$@ $(FLAGS)

clean:
	$(RM) -rf $(EJECUTABLES)

