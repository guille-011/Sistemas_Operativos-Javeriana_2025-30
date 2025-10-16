###############################################
#
# Makefile
# Script para automatizar la compilación de los programas cliente y servidor
# Carrera: Ingeniría en Sistemas
# Autor: Guillermo Aponte
# Materia: Sistemas Operativos
# Tema: Implementación del patrón Productor-Consumidor usando Pipe Named (FIFO)
# Archivo: makefile
#
###############################################

# Compilador a usar
GCC = gcc

#Banderas de compilación, -Wall activa advertencias importantes, -Wextra activa advertencias adicionales y -g incluye información de depuración en el ejecutable
FLAGS = -Wall -Wextra -g

#Carpeta donde se guardarán los ejecutables
EJECUTABLES = ejecutables
# Lista de programas que se generarán
PROGRAMAS = cliente servidor

###############################################
# Regla principal: compilar todo
###############################################
all: $(PROGRAMAS)

###############################################
# Crear carpeta bin si no existe
###############################################
$(EJECUTABLES):
	mkdir -p $(EJECUTABLES)

###############################################
# Regla por defecto para compilar el cliente
###############################################
cliente: $(EJECUTABLES)
	$(GCC) $(FLAGS) cliente.c -o $(EJECUTABLES)/cliente

###############################################
# Regla por defecto para compilar el servidor
###############################################
servidor: $(EJECUTABLES)
	$(GCC) $(FLAGS) servidor.c -o $(EJECUTABLES)/servidor

###############################################
# Limpieza de archivos ejecutables
#############################################
clean:
	$(RM) -r $(EJECUTABLES)


	