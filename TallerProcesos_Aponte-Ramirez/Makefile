###############################################
#
# Makefile
# Script para automatizar la compilación del programa del taller de procesos
# Carrera: Ingeniería en Sistemas
# Autores: Guillermo Aponte, Daniel Ramírez
# Materia: Sistemas Operativos
# Tema: Implementación de procesos fork() para dividir procesos y pipe() para comunicación entre procesos
# Archivo: makefile
#
# Fecha: 30/10/25
###############################################

# Compilador a usar
GCC = gcc

# Nombre del módulo (archivo .c con funciones auxiliares)
MODULO = modulo_procesos

# Carpeta donde se guardarán los ejecutables y objetos compilados
DIRECTORIO = Ejecutables

# Nombre del programa principal
PROGRAMA = taller_procesos

###############################################
# Regla principal: compilar todo el proyecto y crear la carpeta de ejecutables
###############################################
taller_procesos:
	mkdir -p $(DIRECTORIO)
	# Compila el módulo y genera su archivo objeto dentro del directorio
	$(GCC) -c $(MODULO).c -o $(DIRECTORIO)/$(MODULO).o
	# Compila el archivo principal junto con el objeto del módulo y crea el ejecutable final
	$(GCC) $(PROGRAMA).c $(DIRECTORIO)/$(MODULO).o -o $(DIRECTORIO)/$(PROGRAMA)

###############################################
# Limpieza de archivos generados
###############################################
clear:
	# Elimina el ejecutable y el archivo objeto del módulo
	rm -f $(DIRECTORIO)/$(PROGRAMA) $(DIRECTORIO)/$(MODULO).o
